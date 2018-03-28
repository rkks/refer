/*
** serial_module.c for  in /home/nico/lang/kernel/kld-serialdriver
** 
** Made by nicolas
*/

#include <sys/types.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/bus.h>
#include <sys/errno.h>
#include <sys/conf.h>
#include <sys/module.h>
#include <sys/uio.h>
#include <machine/bus.h>
#include <machine/resource.h>
#include <sys/rman.h>

#include <isa/isareg.h>
#include <isa/isavar.h>

static int		mysio_attach(device_t dev);
static int		mysio_probe(device_t dev);
static int		mysio_detach(device_t dev);
static void		mysio_intr(void *arg);
static d_open_t		mysio_open;
static d_close_t	mysio_close;
static d_read_t		mysio_read;
static d_write_t	mysio_write;


static device_method_t	mysio_methods[] = 
{
  /* Device interface */
  DEVMETHOD(device_probe,	mysio_probe),
  DEVMETHOD(device_attach,	mysio_attach),
  DEVMETHOD(device_detach,	mysio_detach),

  { 0, 0 }
};

struct cycbuf
{
#define SIZEOFBUF	256
  char			rbuf[SIZEOFBUF];	/* Cyclic buffer */
  char*			ptr;			/* Current pos in input buffer */
  unsigned		count;			/* Number of bytes used */
  struct mtx		lock;			/* Lock */
};

struct mysio_softc
{
  int			h_port;		/* Ressource handler */
  struct resource*	port;		/* Ressource */
  bus_space_tag_t	bst;		/* Read/Write bus usage */
  bus_space_handle_t	bsh;		/* Read/Write bus usage */
  int			h_irq;		/* Ressource handler */
  struct resource*	irq;		/* Ressource */
  void*			cookie;		/* Intr cookie */
  struct cdev*		dev;		/* /dev/ file */
  int			opened;		/* Open status */
  void*			waddr;		/* tsleep ident */
  struct cycbuf		in_buf;		/* Input buffer */
  struct cycbuf		out_buf;	/* Input buffer */ 
};

static driver_t	mysio_driver = 
{
  "mysio",
  mysio_methods,
  sizeof(struct mysio_softc),
};

static devclass_t	mysio_devclass;

static struct cdevsw mysio_cdevsw = {
  .d_version	= D_VERSION,
/*   .d_flags	= D_NEEDGIANT, /\* Only one thread in the same time in the driver *\/ */
  .d_open	= mysio_open,
  .d_close	= mysio_close,
  .d_read	= mysio_read,
  .d_write	= mysio_write,
  .d_name	= "mysio",
};

/* The offsets of UART registers on the bus
*/
#define UART_TX			0	/* Transmitter Holding Buffer */
#define UART_RX			0	/* Receiver Buffer */
#define UART_DLL		0	/* Divisor Latch Low Byte */
#define UART_IER		1	/* Interrupt Enable Register */
#define UART_DLH		1	/* Divisor Latch High Byte */
#define UART_IIR		2	/* Interrupt Identification Register */
#define UART_FCR		2	/* Fifo Control Register */
#define UART_LCR		3	/* Line Control Register */
#define UART_MCR		4	/* Modem Control Register */
#define UART_LSR		5	/* Line Status Register */
#define UART_MSR		6	/* Modem Status Register */
#define UART_SR			7	/* Scratch Register */

/* Baudrates
 */
#define BAUDRATE_MAX		115200
#define BAUDRATE		BAUDRATE_MAX

/* LSR values
 */
#define UART_DATA_READY		0x01
#define UART_OVERRUN		0x02
#define UART_EMPTY_HOLDING_REG	0x10
#define UART_EMPTY_TRANSMITTER	0x20	/* Transmitter Holding Register Empty */

/* IER values
 */
#define UART_ERXRDY		0x1
#define UART_ETXRDY		0x2
#define UART_ERLS		0x4
#define UART_EMSC		0x8

/* Parity enum
 */
#define UART_NO_PARITY		0x00
#define UART_ODD_PARITY		0x08
#define UART_EVEN_PARITY	0x18

/* Word length
 */
#define UART_5BITS_WORD		0x00
#define UART_6BITS_WORD		0x01
#define UART_7BITS_WORD		0x02
#define UART_8BITS_WORD		0x03

/* Stop bit length
 */
#define UART_1_STOP_BIT		0x00
#define UART_2_STOP_BITS	0x04

/* DLAB on
 */
#define UART_DLAB		0x80

/* Enable FIFO
 */
#define UART_ENABLE_FIFO	0xC7

/* Turn on DTR, RTS, and OUT2
 */
#define UART_ENABLE_MODEM	0x0B


/*****************************************************************************/
/* Helpers                                                                   */
/*****************************************************************************/
#define MYSIO_SLEEP(t)		{ \
				  int wake_reason = tsleep(sc->waddr, PCATCH|PWAIT, "idle", t); \
				  if (wake_reason && wake_reason != EWOULDBLOCK) \
				    return wake_reason; /* Software intr */ \
				}
#define INPUT_GETNEXTCHAR(sc)	cycbuf_getnextchar(&sc->in_buf)
#define INPUT_ADDCHAR(sc, c)	cycbuf_addchar(&sc->in_buf, c)
#define INPUT_COUNT(sc)		cycbuf_count(&sc->in_buf)
#define INPUT_MAXSIZE(sc)	sizeof(sc->in_buf.rbuf)
#define OUTPUT_GETNEXTCHAR(sc)	cycbuf_getnextchar(&sc->out_buf)
#define OUTPUT_ADDCHAR(sc, c)	cycbuf_addchar(&sc->out_buf, c)
#define OUTPUT_COUNT(sc)	cycbuf_count(&sc->out_buf)
#define OUTPUT_MAXSIZE(sc)	sizeof(sc->out_buf.rbuf)

static void	cycbuf_init(struct cycbuf* buf)
{
  bzero((void*)buf, sizeof(struct cycbuf));
  buf->ptr = buf->rbuf;
  mtx_init(&(buf->lock), "mysio", NULL, MTX_SPIN);
}

static void	cycbuf_destoy(struct cycbuf* buf)
{
  if (mtx_initialized(&(buf->lock)))
    mtx_destroy(&(buf->lock));
}

static unsigned	cycbuf_count(struct cycbuf* buf)
{
  unsigned ret;
  mtx_lock(&(buf->lock));
  {
    ret = buf->count;
  }
  mtx_unlock(&(buf->lock));
  return ret;
}

static u_int8_t	cycbuf_getnextchar(struct cycbuf* buf)
{
  u_int8_t out;

  mtx_lock(&(buf->lock));
  {  
    KASSERT(buf->count != 0, ("cycbuf_getnextchar called without cycbuf_count check"));
    /* Get char and update data
     */
    out = *(buf->ptr);
    (buf->count)--;
    (buf->ptr)++;
    if (buf->ptr == buf->rbuf + SIZEOFBUF) /* End of buf ? */
      buf->ptr = buf->rbuf;
  }
  mtx_unlock(&(buf->lock));
  return out;
}

static void	cycbuf_addchar(struct cycbuf* buf, char in)
{
  mtx_lock(&(buf->lock));
  {  
    /* Point to end of buf
     */
    char* slot = buf->ptr;
    for (int i = buf->count; i; i--)
      if (++slot == buf->rbuf + SIZEOFBUF) slot = buf->rbuf;
    /* Update data
     */
    *slot = in;
    if (buf->count < sizeof(buf->rbuf)) /* Buffer is full ? */
      buf->count++;
  }
  mtx_unlock(&(buf->lock));
}

/*****************************************************************************/
/* Probe                                                                     */
/*****************************************************************************/
static struct isa_pnp_id sio_ids[] = {
  {0x0005d041, "Standard PC COM port"},					/* PNP0500 */
  {0x0105d041, "16550A-compatible COM port"},				/* PNP0501 */
  {0x0205d041, "Multiport serial device (non-intelligent 16550)"},	/* PNP0502 */
  {0x1005d041, "Generic IRDA-compatible device"},			/* PNP0510 */
  {0x1105d041, "Generic IRDA-compatible device"},			/* PNP0511 */
  {0}
};

static int	mysio_probe(device_t dev)
{
  /* Check isapnp ids 
   */
  if (ISA_PNP_PROBE(device_get_parent(dev), dev, sio_ids) == ENXIO)
    return ENXIO;
  return 0;
}

/*****************************************************************************/
/* Attach                                                                    */
/*****************************************************************************/
static int	mysio_attach(device_t dev)
{
  /* Check 
   */
  u_long flags = device_get_flags(dev);
  u_long unit = device_get_unit(dev);
  u_long irq_start, irq_count;
  if (bus_get_resource(dev, SYS_RES_IRQ, 0, &irq_start, &irq_count))
    return ENXIO;
  u_long port_start = 0, port_count = 0;
  if (bus_get_resource(dev, SYS_RES_IOPORT, 0, &port_start, &port_count))
    return ENXIO;
  /* Init softc
   */
  struct mysio_softc* sc;
  sc = (struct mysio_softc*)device_get_softc(dev);
  bzero((void*)sc, sizeof(struct mysio_softc));
  sc->waddr = sc; /* Wait unique address */
  cycbuf_init(&(sc->in_buf));
  cycbuf_init(&(sc->out_buf));
  /* Alloc resource
   */
  sc->port = bus_alloc_resource(dev, SYS_RES_IOPORT, &sc->h_port, 0, ~0, 1, RF_SHAREABLE | RF_ACTIVE);
  if (sc->port == NULL)
    goto mysio_attach_failed;
  sc->bst = rman_get_bustag(sc->port);
  sc->bsh = rman_get_bushandle(sc->port);
  sc->irq = bus_alloc_resource(dev, SYS_RES_IRQ, &sc->h_irq, 0, ~0, 1, RF_SHAREABLE | RF_ACTIVE);
  if (sc->irq == NULL)
    goto mysio_attach_failed;
  /* Init hardware
   */
  bus_space_write_1(sc->bst, sc->bsh, UART_IER, 0); /* Turn off interrupt */
  bus_space_write_1(sc->bst, sc->bsh, UART_LCR, UART_DLAB); /* Set DLAB on */
  bus_space_write_1(sc->bst, sc->bsh, UART_DLL, ((short)(BAUDRATE_MAX / BAUDRATE)) & 0xFF); /* Set baud rate - Divisor Latch Low Byte */
  bus_space_write_1(sc->bst, sc->bsh, UART_DLH, ((short)(BAUDRATE_MAX / BAUDRATE)) >> 8); /* Set baud rate - Divisor Latch High Byte */
  bus_space_write_1(sc->bst, sc->bsh, UART_LCR, UART_8BITS_WORD|UART_NO_PARITY|UART_1_STOP_BIT); /* Set the line status 
												    (8 Bits, No Parity, 1 Stop Bit) 
												    Set DLAB off */
  bus_space_write_1(sc->bst, sc->bsh, UART_FCR, UART_ENABLE_FIFO); /* Enable FIFO */
  bus_space_write_1(sc->bst, sc->bsh, UART_MCR, UART_ENABLE_MODEM); /* Turn on DTR, RTS, and OUT2 */
  bus_space_write_1(sc->bst, sc->bsh, UART_IER, UART_ERXRDY); /* Turn on read interrupt */
  /* Attach intr handler
   */
  if (BUS_SETUP_INTR(device_get_parent(dev), dev, sc->irq, INTR_TYPE_TTY, mysio_intr, sc, &sc->cookie))
    goto mysio_attach_failed;
  /* Build dev
   */
  sc->dev = make_dev(&mysio_cdevsw, unit, UID_ROOT, GID_WHEEL, 0644, "mysio%d", (int)unit);
  if (sc->dev == NULL)
    goto mysio_attach_failed;
  /* Assign sc in dev
   */
  sc->dev->si_drv1 = sc;
  sc->dev->si_drv2 = NULL;
  /* Succeed
   */
  (void) device_printf(dev, "Attach succeed at /dev/mysio%d (IRQ=0x%x, IOPORT=0x%x-0x%x FLAGS=0x%x)\n", 
		       (int)unit, (int)irq_start, (int)port_start, (int)(port_start + port_count - 1), (int)flags);
  return 0;
 mysio_attach_failed:
			cycbuf_destoy(&(sc->in_buf));
			cycbuf_destoy(&(sc->out_buf));
  if (sc->dev)		destroy_dev(sc->dev);
  if (sc->cookie)	(void) BUS_TEARDOWN_INTR(device_get_parent(dev), dev, sc->irq, sc->cookie);
  if (sc->irq)		(void) bus_release_resource(dev, SYS_RES_IRQ, sc->h_irq, sc->irq);
  if (sc->port)		(void) bus_release_resource(dev, SYS_RES_IOPORT, sc->h_port, sc->port);
  return ENXIO;
}

/*****************************************************************************/
/* Detach                                                                    */
/*****************************************************************************/
static int	mysio_detach(device_t dev)
{
  /* Cleanup
   */
  struct mysio_softc* sc = (struct mysio_softc*)device_get_softc(dev);
  cycbuf_destoy(&(sc->in_buf));
  cycbuf_destoy(&(sc->out_buf));
  destroy_dev(sc->dev);
  (void) BUS_TEARDOWN_INTR(device_get_parent(dev), dev, sc->irq, sc->cookie);
  (void) bus_release_resource(dev, SYS_RES_IRQ, sc->h_irq, sc->irq);
  (void) bus_release_resource(dev, SYS_RES_IOPORT, sc->h_port, sc->port);  
  return 0;
}

/*****************************************************************************/
/* Intr handler                                                              */
/*****************************************************************************/
static void	mysio_intr(void *arg)
{
  struct mysio_softc* sc = arg;

  /* Check incoming data
   */
  while (bus_space_read_1(sc->bst, sc->bsh, UART_LSR) & UART_DATA_READY)
    INPUT_ADDCHAR(sc, bus_space_read_1(sc->bst, sc->bsh, UART_RX));
  /* Check outcoming data
   */
  while (OUTPUT_COUNT(sc) && bus_space_read_1(sc->bst, sc->bsh, UART_LSR) & UART_EMPTY_TRANSMITTER)
    bus_space_write_1(sc->bst, sc->bsh, UART_TX, OUTPUT_GETNEXTCHAR(sc));
  /* Intr reset
   */
  if (OUTPUT_COUNT(sc))
    bus_space_write_1(sc->bst, sc->bsh, UART_IER, UART_ERXRDY|UART_ETXRDY);
  else
    bus_space_write_1(sc->bst, sc->bsh, UART_IER, UART_ERXRDY);
  /* Wakeup process
   */
  wakeup(sc->waddr);
}

/*****************************************************************************/
/* Open                                                                      */
/*****************************************************************************/
static int	mysio_open(struct cdev* dev, int oflags, int devtype, struct thread *td)
{
  struct mysio_softc* sc = sc = dev->si_drv1;
  if (sc->opened)
    return EBUSY;
  sc->opened = 1;
  return 0;
}
	
/*****************************************************************************/
/* Close                                                                     */
/*****************************************************************************/
static int	mysio_close(struct cdev* dev, int fflag, int devtype, struct thread *td)
{
  struct mysio_softc* sc = sc = dev->si_drv1;
  sc->opened = 0;
  return 0;
}

/*****************************************************************************/

/* Read                                                                      */
/*****************************************************************************/
static int	mysio_read(struct cdev* dev, struct uio *uio, int ioflag)
{
  u_int8_t ch;
  struct mysio_softc* sc = sc = dev->si_drv1;
  /* Wait for incoming data
   */
  while (!INPUT_COUNT(sc))
    MYSIO_SLEEP(0); /* Sleep till intr */
  /* Read it
   */
  while (uio->uio_resid > 0 && INPUT_COUNT(sc))
    {
      ch = INPUT_GETNEXTCHAR(sc);
      int err = uiomove(&ch, 1, uio);
      if (err)
	return err;
      //MYSIO_SLEEP(100);
    }
  return 0;
}

/*****************************************************************************/
/* Write                                                                     */
/*****************************************************************************/
static int	mysio_write(struct cdev* dev, struct uio *uio, int ioflag)
{
  struct mysio_softc* sc = sc = dev->si_drv1;
  /* Write data
   */
  while (uio->uio_resid > 0 && OUTPUT_COUNT(sc) != OUTPUT_MAXSIZE(sc))
    {
      /* Get data
       */
      u_int8_t ch;
      int err = uiomove(&ch, 1, uio);
      if (err)
	return err;
      /* Add to write buffer
       */
      OUTPUT_ADDCHAR(sc, ch);
    }
  /* Start write
   */
  mysio_intr(sc);
  return 0;
}

DRIVER_MODULE(mysio, acpi, mysio_driver, mysio_devclass, 0, 0);
DRIVER_MODULE(mysio, isa, mysio_driver, mysio_devclass, 0, 0);
