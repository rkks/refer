/*
 *  Universal TUN/TAP device driver.
 *  Copyright (C) 1999-2000 Maxim Krasnyansky <max_mk@yahoo.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  $Id: tun.c,v 1.4 2000/10/24 05:13:08 maxk Exp $
 */

#include <linux/module.h>

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/sched.h>
#include <linux/malloc.h>
#include <linux/poll.h>
#include <linux/fcntl.h>
#include <linux/init.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/if.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>

#include <asm/system.h>
#include <asm/uaccess.h>

#include "if_tun.h"

#ifdef TUN_DEBUG
static int debug=0;
#endif

/* Open mask */
char tun_open_mask[TUN_MAX_DEV/8];

/* Network device part of the driver */

/* Net device open. */
static int tun_net_open(struct device *dev)
{
#ifdef TUN_DEBUG  
   struct tun_struct *tun = (struct tun_struct *)dev->priv;

   DBG(KERN_INFO "%s: tun_net_open\n", tun->name);
#endif

   dev->start = 1;

   return 0;
}

/* Net device close. */
static int tun_net_close(struct device *dev)
{
#ifdef TUN_DEBUG  
   struct tun_struct *tun = (struct tun_struct *)dev->priv;

   DBG(KERN_INFO "%s: tun_net_close\n", tun->name);
#endif

   dev->start = 0;

   return 0;
}

/* Net device start xmit */
static int tun_net_xmit(struct sk_buff *skb, struct device *dev)
{
   struct tun_struct *tun = (struct tun_struct *)dev->priv;

   DBG(KERN_INFO "%s: tun_net_xmit %d\n", tun->name, skb->len);

   if( dev->tbusy )
      return 1;

   /* Queue frame */ 
   skb_queue_tail(&tun->txq, skb);
   if( skb_queue_len(&tun->txq) >= TUN_TXQ_SIZE )
      dev->tbusy = 1;

   if( tun->flags & TUN_FASYNC )
#ifdef NEW_FASYNC 
      kill_fasync(&tun->fasync, SIGIO, POLL_IN);
#else     
      kill_fasync(&tun->fasync, SIGIO);
#endif

   /* Wake up process */ 
   wake_up_interruptible(&tun->read_wait);

   return 0;
}

static void tun_net_mclist(struct device *dev)
{
#ifdef TUN_DEBUG
   struct tun_struct *tun = (struct tun_struct *)dev->priv;

   DBG(KERN_INFO "%s: tun_net_mclist\n", tun->name);
#endif

   /* Nothing to do for multicast filters. 
    * We always accept all frames */

   return;
}

static struct enet_statistics *tun_net_stats(struct device *dev)
{
   struct tun_struct *tun = (struct tun_struct *)dev->priv;
   return &tun->stats;
}

/* Initialize net device */
int tun_net_init(struct device *dev)
{
   struct tun_struct *tun = (struct tun_struct *)dev->priv;
   
   DBG(KERN_INFO "%s: tun_net_init\n", tun->name);

   dev->open = tun_net_open;
   dev->hard_start_xmit = tun_net_xmit;
   dev->stop = tun_net_close;
   dev->get_stats = tun_net_stats;

   switch( tun->flags & TUN_TYPE_MASK ) {
      case TUN_TUN_DEV:
         /* Point-to-Point TUN Device */

	 dev->hard_header_len=0;
	 dev->addr_len=0;
	 dev->mtu = 1500;
 	 /* Type PPP seems most suitable */
	 dev->type = ARPHRD_PPP; 
	 dev->flags = IFF_POINTOPOINT | IFF_NOARP | IFF_MULTICAST;
	 dev->tx_queue_len = 10;

         dev_init_buffers(dev);

	 break;
      case TUN_TAP_DEV:
   	 /* Ethernet TAP Device */
   	 
         dev->set_multicast_list = tun_net_mclist;

	 /* Generate Ethernet address 
	  * Should be random enough  */
   	 *(unsigned short *)dev->dev_addr = htons(0x00FF);
   	 *(unsigned long *)(dev->dev_addr + sizeof(short)) = 
		htonl( (long)((jiffies & 0x00ffffff)<<8) | (dev->base_addr+1) );

   	 ether_setup(dev);

	 break;
   }

   return 0;
}

/* Character device part */

/* Poll */
static unsigned int tun_chr_poll(struct file *file, poll_table * wait)
{  
   struct tun_struct *tun = (struct tun_struct *)file->private_data;

   DBG( KERN_INFO "%s: tun_chr_poll\n", tun->name);

   poll_wait(file, &tun->read_wait, wait);
 
   if( skb_queue_len(&tun->txq) )
      return POLLIN | POLLRDNORM;

   return POLLOUT | POLLWRNORM;
}

/* Write */
static ssize_t tun_chr_write(struct file * file, const char * buf, 
			 size_t count, loff_t *pos)
{
   struct tun_struct *tun = (struct tun_struct *)file->private_data;
   struct sk_buff* skb;

   DBG(KERN_INFO "%s: tun_chr_write %d\n", tun->name, count);

   /* Don't allow too large frames */
   count = MIN(TUN_MAX_FRAME, count);
 
   if( !(skb = dev_alloc_skb(count+2)) ){
      printk("%s: Can't allocate memory, dropping packet.\n", tun->name);
      tun->stats.rx_dropped++;
      return -ENOMEM;
   }
 
   skb_reserve(skb, 2);
   if( copy_from_user(skb_put(skb, count), buf, count) ) {
      dev_kfree_skb(skb);
      return -EFAULT;
   }

   skb->dev = &tun->dev;
   switch( tun->flags & TUN_TYPE_MASK ) {
      case TUN_TUN_DEV:
         skb->mac.raw = skb->data;
         skb->protocol = __constant_htons(ETH_P_IP);
         break;
      case TUN_TAP_DEV:
         skb->protocol = eth_type_trans(skb, &tun->dev);
         break;
   }

   if( tun->flags & TUN_NOCHECKSUM )
      skb->ip_summed = CHECKSUM_UNNECESSARY;
 
   netif_rx(skb);
   
   tun->stats.rx_packets++;
   tun->stats.rx_bytes += count;

   return count;
}

/* Read */
static ssize_t tun_chr_read(struct file * file, char * buf, 
			size_t count, loff_t *pos)
{
   struct tun_struct *tun = (struct tun_struct *)file->private_data;
   struct wait_queue wait = { current, NULL };
   struct sk_buff *skb;
   ssize_t ret=0, len;

   DBG(KERN_INFO "%s: tun_chr_read\n", tun->name);

   add_wait_queue(&tun->read_wait, &wait);
   while( count ){
      current->state = TASK_INTERRUPTIBLE;

      /* Read frames from device queue */
      if( !(skb=skb_dequeue(&tun->txq)) ) {
	 if( file->f_flags & O_NONBLOCK ) {
	    ret = -EAGAIN;
 	    break;
	 }
	 if( signal_pending(current) ) {
	    ret = -ERESTARTSYS;
 	    break;
	 }

	 /* Nothing to read, let's sleep */
	 schedule();
         continue;
      }
      tun->dev.tbusy = 0;

      /* Notify net layer that we are ready to 
       * handle more frames */	
      mark_bh(NET_BH);

      /* Copy frame to user space buffer. 
       * If it doesn't fit strip it */
      len = MIN(skb->len, count); 
      if( copy_to_user(buf, skb->data, len) )
	 ret = -EFAULT;
      else
	 ret = len;

      dev_kfree_skb(skb);

      tun->stats.tx_packets++;
      tun->stats.tx_bytes += len;

      break; 
   }
   current->state = TASK_RUNNING;
   remove_wait_queue(&tun->read_wait, &wait);

   return ret;
}

static loff_t tun_chr_lseek(struct file * file, loff_t offset, int origin)
{
   return -ESPIPE;
}

static int tun_chr_ioctl(struct inode *inode, struct file *file, 
		     unsigned int cmd, unsigned long arg)
{
   struct tun_struct *tun = (struct tun_struct *)file->private_data;

   DBG(KERN_INFO "%s: tun_chr_ioctl\n", tun->name);

   switch( cmd ){
      case TUNSETNOCSUM:
	 /* Disable/Enable checksum on net iface */
         if( arg ) 
	    tun->flags |= TUN_NOCHECKSUM;
	 else
	    tun->flags &= ~TUN_NOCHECKSUM;
	 DBG(KERN_INFO"%s: checksum %s\n",tun->name, arg?"disabled":"enabled");
	 break;
#ifdef TUN_DEBUG
      case TUNSETDEBUG:
	 tun->debug = arg;
	 break;
#endif
      default:
         return -EINVAL;
   }
   return 0;
}

static int tun_chr_fasync(int fd, struct file *file, int on)
{
   struct tun_struct *tun = (struct tun_struct *)file->private_data;
 
   DBG(KERN_INFO "%s: tun_chr_fasync %d\n", tun->name, on);

   if( on ) {
      /* Enable FASYNC mode 
       * We don't need any fasync queue since only one process 
       * is allowed to open TUN device 
       */
      if( !file->f_owner.pid ) {
         file->f_owner.pid  = current->pid;
         file->f_owner.uid  = current->uid; 
         file->f_owner.euid = current->euid;
      }
      tun->fasync.magic   = FASYNC_MAGIC;
      tun->fasync.fa_file = file;
      tun->fasync.fa_fd   = fd;
      tun->fasync.fa_next = NULL;

      tun->flags |= TUN_FASYNC;
   } else {
      /* Disable FASYNC mode */
      tun->flags &= ~TUN_FASYNC;
   }	

   return 0;
}

static int tun_chr_open(struct inode *inode, struct file * file)
{
   unsigned int minor = MINOR(inode->i_rdev);
   unsigned int index = minor & TUN_MINOR_MASK;
   struct tun_struct *tun = NULL; 

   if( minor > TUN_MAX_DEV ){
      DBG1(KERN_ERR "tun: Device minor is too large %d\n", minor);
      return -ENODEV;
   }

   DBG1(KERN_INFO "tun%d: tun_chr_open\n", index);

   /* Only one process is allowed to open */	
   if( test_and_set_bit(minor, tun_open_mask) )
      return -EBUSY;    

   DBG1(KERN_INFO "tun%d: Allocating device\n", index);
   
   if( !(tun=kmalloc(sizeof(struct tun_struct), GFP_KERNEL)) ) {
      clear_bit(minor, tun_open_mask);
      return -ENOMEM;
   }

   file->private_data = tun;

   memset(tun, 0, sizeof(struct tun_struct));

   /* Set device type */
   if( minor < TUN_TAP_MINOR ){
      /* TUN device */
      tun->flags |= TUN_TUN_DEV;
      sprintf(tun->name, "tun%d", index);
   } else {
      /* TAP device */
      tun->flags |= TUN_TAP_DEV;
      sprintf(tun->name, "tap%d", index);
   } 

   /* Initialize and register net device */
   skb_queue_head_init(&tun->txq);
    	
   tun->dev.name  = tun->name;
   tun->dev.init  = tun_net_init;
   tun->dev.base_addr = index;
   tun->dev.priv = tun; 

   if( register_netdev(&tun->dev) ){
      printk(KERN_ERR "%s: Can't register net device\n", tun->name);
      
      file->private_data = NULL;
      clear_bit(minor, tun_open_mask);

      kfree(tun);
      return -ENODEV;
   }

   MOD_INC_USE_COUNT;
	
   return 0;
}

static int tun_chr_close(struct inode *inode, struct file *file)
{
   struct tun_struct *tun = (struct tun_struct *)file->private_data;
   unsigned int minor = MINOR(inode->i_rdev);
   register struct sk_buff *skb;

   DBG(KERN_INFO "%s: tun_chr_close\n", tun->name);

   dev_close(&tun->dev);

   /* Drop TX queue */
   while( (skb = skb_dequeue(&tun->txq)) )
      dev_kfree_skb(skb);

   unregister_netdev(&tun->dev);

   kfree(tun);
   file->private_data = NULL;

   clear_bit(minor, tun_open_mask);

   MOD_DEC_USE_COUNT;
   return 0;
}

static struct file_operations tun_fops = {
   tun_chr_lseek,
   tun_chr_read,
   tun_chr_write,
   NULL,		/* readdir */
   tun_chr_poll,
   tun_chr_ioctl,
   NULL,		/* mmap */
   tun_chr_open,
   NULL,		/* flush */
   tun_chr_close,
   NULL,		/* fsync */
   tun_chr_fasync		
};

__initfunc(int tun_init(void))
{
   printk(KERN_INFO "Universal TUN/TAP device driver %s " 
		    "(c) Maxim Krasnyansky\n", TUN_VER);

   if( register_chrdev(TUN_MAJOR,"tun", &tun_fops) ){
      printk(KERN_ERR "tun: Can't register char device %d\n", TUN_MAJOR);
      return -EIO;
   }

#ifdef MODULE
   return 0;
#else
   /* If driver is not module, tun_init will be called from Space.c.
    * Return non-zero to not register fake device. */	
   return 1;
#endif
}

#ifdef MODULE

int init_module(void)
{
   return tun_init();
}

void cleanup_module(void)
{
   unregister_chrdev(TUN_MAJOR,"tun");
}

#endif
