/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2005, 2006, 2007, 2008, 2009 Teunis van Beelen
*
* teuniz@gmail.com
*
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
*
* This version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/



#include "rs232.h"

/*
Possible baudrates on a normal pc:

50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800,
2400, 4800, 9600, 19200, 38400, 57600, 115200
*/
#ifdef __linux__
#define BAUD B115200
#else
#define BAUD "baud=115200 data=8 parity=N stop=1"
#endif


#ifdef __linux__


int Cport, error;

struct termios new_port_settings, old_port_settings;

char comports[22][13]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyS4","/dev/ttyS5",
                       "/dev/ttyS6","/dev/ttyS7","/dev/ttyS8","/dev/ttyS9","/dev/ttyS10","/dev/ttyS11",
                       "/dev/ttyS12","/dev/ttyS13","/dev/ttyS14","/dev/ttyS15","/dev/ttyUSB0",
                       "/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyUSB4","/dev/ttyUSB5"};


int OpenComport(int comport_number)
{
  if(comport_number>21)
  {
    printf("illegal comport number\n");
    return(1);
  }

  Cport = open(comports[comport_number], O_RDWR | O_NOCTTY | O_NDELAY);
  if(Cport==-1)
  {
    perror("unable to open comport ");
    return(1);
  }

  error = tcgetattr(Cport, &old_port_settings);
  if(error==-1)
  {
    close(Cport);
    perror("unable to read portsettings ");
    return(1);
  }
  memset(&new_port_settings, 0, sizeof(new_port_settings));  /* clear the new struct */

  new_port_settings.c_cflag = BAUD | CS8 | CLOCAL | CREAD;
  new_port_settings.c_iflag = IGNPAR;
  new_port_settings.c_oflag = 0;
  new_port_settings.c_lflag = 0;
  new_port_settings.c_cc[VMIN] = 0;      /* block untill n bytes are received */
  new_port_settings.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */
  error = tcsetattr(Cport, TCSANOW, &new_port_settings);
  if(error==-1)
  {
    close(Cport);
    perror("unable to adjust portsettings ");
    return(1);
  }

  return(0);
}


int PollComport(unsigned char *buf, int size)
{
  int n;

#ifndef __STRICT_ANSI__                       /* __STRICT_ANSI__ is defined when the -ansi option is used for gcc */
  if(size>SSIZE_MAX)  size = (int)SSIZE_MAX;  /* SSIZE_MAX is defined in limits.h */
#else
  if(size>4096)  size = 4096;
#endif

  n = read(Cport, buf, size);

  return(n);
}


int SendByte(unsigned char byte)
{
  int n;

  n = write(Cport, &byte, 1);
  if(n<0)  return(1);

  return(0);
}


int SendBuf(unsigned char *buf, int size)
{
  return(write(Cport, buf, size));
}


void CloseComport(void)
{
  close(Cport);
  tcsetattr(Cport, TCSANOW, &old_port_settings);
}



/*
Constant  Description
TIOCM_LE  DSR (data set ready/line enable)
TIOCM_DTR DTR (data terminal ready)
TIOCM_RTS RTS (request to send)
TIOCM_ST  Secondary TXD (transmit)
TIOCM_SR  Secondary RXD (receive)
TIOCM_CTS CTS (clear to send)
TIOCM_CAR DCD (data carrier detect)
TIOCM_CD  Synonym for TIOCM_CAR
TIOCM_RNG RNG (ring)
TIOCM_RI  Synonym for TIOCM_RNG
TIOCM_DSR DSR (data set ready)
*/

int IsCTSEnabled(void)
{
  int status;

  status = ioctl(Cport, TIOCMGET, &status);

  if(status&TIOCM_CTS) return(1);
  else return(0);
}


#else


HANDLE Cport;


char comports[16][10]={"\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",
                       "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
                       "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                       "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16"};


int OpenComport(int comport_number)
{
  if(comport_number>15)
  {
    printf("illegal comport number\n");
    return(1);
  }

  Cport = CreateFileA(comports[comport_number],
                      GENERIC_READ|GENERIC_WRITE,
                      0,                          /* no share  */
                      NULL,                       /* no security */
                      OPEN_EXISTING,
                      0,                          /* no threads */
                      NULL);                      /* no templates */

  if(Cport==INVALID_HANDLE_VALUE)
  {
    printf("unable to open comport\n");
    return(1);
  }

  DCB port_settings;
  memset(&port_settings, 0, sizeof(port_settings));  /* clear the new struct  */
  port_settings.DCBlength = sizeof(port_settings);

  if(!BuildCommDCBA(BAUD, &port_settings))
  {
    printf("unable to set comport dcb settings\n");
    CloseHandle(Cport);
    return(1);
  }

  if(!SetCommState(Cport, &port_settings))
  {
    printf("unable to set comport cfg settings\n");
    CloseHandle(Cport);
    return(1);
  }

  COMMTIMEOUTS Cptimeouts;

  Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
  Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
  Cptimeouts.ReadTotalTimeoutConstant    = 0;
  Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  Cptimeouts.WriteTotalTimeoutConstant   = 0;

  if(!SetCommTimeouts(Cport, &Cptimeouts))
  {
    printf("unable to set comport time-out settings\n");
    CloseHandle(Cport);
    return(1);
  }

  return(0);
}


int PollComport(unsigned char *buf, int size)
{
  int n;

  if(size>4096)  size = 4096;

/* added the void pointer cast, otherwise gcc will complain about */
/* "warning: dereferencing type-punned pointer will break strict aliasing rules" */

  ReadFile(Cport, buf, size, (LPDWORD)((void *)&n), NULL);

  return(n);
}


int SendByte(unsigned char byte)
{
  int n;

  WriteFile(Cport, &byte, 1, (LPDWORD)((void *)&n), NULL);

  if(n<0)  return(1);

  return(0);
}


int SendBuf(unsigned char *buf, int size)
{
  int n;

  if(WriteFile(Cport, buf, size, (LPDWORD)((void *)&n), NULL))
  {
    return(n);
  }

  return(-1);
}


void CloseComport(void)
{
  CloseHandle(Cport);
}


int IsCTSEnabled(void)
{
  int status;

  GetCommModemStatus(Cport, (LPDWORD)((void *)&status));

  if(status&MS_CTS_ON) return(1);
  else return(0);
}


#endif


void cprintf(const char *text)  /* sends a string to serial port */
{
  while(*text != 0)   SendByte(*(text++));
}


