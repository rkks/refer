#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ERROR 0
#define MAX_INPUT_LINE 80
#define SUCCESS 1

/******************************************************************************/

int main ( long argc, char **argv ) {

/******************************************************************************/

/*
  Purpose:

    HEXDUMP is a simple hexadecimal dump program.  

  Usage:

    hexdump FILE_NAME > OUTPUT  for command-line use;
    hexdump                     for interactive use.

  Sample output:

    HEXDUMP: Hexadecimal dump of file: box.3ds.

       Address                   Hexadecimal values                  Printable
    --------------  -----------------------------------------------  ----------------

        0 00000000  4d 4d 53 02 00 00 02 00 0a 00 00 00 03 00 00 00  MMS.............
       16 00000010  3d 3d 68 01 00 00 3e 3d 0a 00 00 00 03 00 00 00  ==h...>=........
       32 00000020  00 01 0a 00 00 00 00 00 80 3f 00 40 4e 01 00 00  .........?.@N...
       48 00000030  42 6f 78 30 31 00 00 41 42 01 00 00 10 41 68 00  Box01..AB....Ah.
       64 00000040  00 00 08 00 26 76 0a c2 1f 0c a8 c1 00 00 00 00  ....&v..........
       ...

  Note:

    HEXDUMP is similar to the UNIX octal dump program OD with the "-h" switch,
    but automatically includes a listing of printable ASCII characters.
    
  Reference:

    Howard Burdick,
    Digital Imaging, Theory and Applications,
    McGraw Hill, 1997, page 219.

  Modified:

    09 August 2001
*/
  long int       addr;
  unsigned char *b;
  unsigned char  buf[20];
  long int       cnt;
  long int       cnt2;
  FILE          *filein;
  char           filein_name[MAX_INPUT_LINE];
  FILE          *fileout;
  char           fileout_name[MAX_INPUT_LINE];
  char           input[MAX_INPUT_LINE];
  long           m;
  long           n;
/*
  Check for the right number of arguments.
*/
  if ( argc < 2 ) {

    printf ( "\n" );
    printf ( "HEXDUMP:\n" );
    printf ( "  Please enter the INPUT file name:\n" );

    if ( fgets ( input, MAX_INPUT_LINE, stdin ) != NULL ) {
      sscanf ( input, "%s", filein_name );
    }
    else {
      printf ( "\n" );
      printf ( "HEXDUMP - Fatal error!\n" );
      printf ( "  Input error!\n" );
      return ERROR;
    }

    printf ( "\n" );
    printf ( "HEXDUMP:\n" );
    printf ( "  Please enter the OUTPUT file name:\n" );

    if ( fgets ( input, MAX_INPUT_LINE, stdin ) != NULL ) {
      sscanf ( input, "%s", fileout_name );
    }
    else {
      printf ( "\n" );
      printf ( "HEXDUMP - Fatal error!\n" );
      printf ( "  Input error!\n" );
      return ERROR;
    }

    fileout = fopen ( fileout_name, "w" );

    if ( fileout == NULL ) {
      printf ( "\n" );
      printf ( "HEXDUMP - Fatal error!\n" );
      printf ( "  Cannot open the output file %s.\n", fileout_name );
      return ERROR;
    }

  }
  else {
    strcpy ( filein_name, argv[1] );
    fileout = stdout;
  }
/*
  Open the input file.
*/
  filein = fopen ( filein_name, "rb" );

  if ( filein == NULL ) {
    printf ( "\n" );
    printf ( "HEXDUMP - Fatal error!\n" );
    printf ( "  Cannot open the input file %s.\n", filein_name );
    return ERROR;
  }
/*
  Dump the file contents.
*/
  fprintf ( fileout, "\n" );
  fprintf ( fileout, "HEXDUMP: Hexadecimal dump of file: %s.\n", filein_name );
  fprintf ( fileout, "\n" );
  fprintf ( fileout, "   Address                   Hexadecimal values                  Printable\n" );
  fprintf ( fileout, "--------------  -----------------------------------------------  ----------------\n" );
  fprintf ( fileout, "\n" );

  addr = 0;

  while ( ( cnt = ( long ) 
    fread ( buf, sizeof ( unsigned char ), 16, filein ) ) > 0 ) {

    b = buf;
/*
  Print the address in decimal and hexadecimal.
*/
    fprintf ( fileout, "%5d %08lx  ", (int) addr, addr );
    addr = addr + 16;
/*
  Print 16 data items, in pairs, in hexadecimal.
*/
    cnt2 = 0;
    for ( m = 0; m < 16; m++ ) {   
      cnt2 = cnt2 + 1;
      if ( cnt2 <= cnt ) {
        fprintf ( fileout, "%02x", *b++ );
      }
      else {
        fprintf ( fileout, "  " );
      }
      fprintf ( fileout, " " );
    }
/*
  Print the printable characters, or a period if unprintable.
*/
    fprintf ( fileout, " " );
    cnt2 = 0;
    for ( n = 0; n < 16; n++ ) {
      cnt2 = cnt2 + 1;
      if ( cnt2 <= cnt ) {
        if ( ( buf[n] < 32 ) || ( buf[n] > 126 ) ) {
          fprintf ( fileout, "%c", '.' );
        }
        else {
          fprintf ( fileout, "%c", buf[n] );
        }
      }
    }
    fprintf( fileout, "\n" );
  }

  fclose ( filein );

  fprintf ( fileout, "\n" );
  fprintf ( fileout, "HEXDUMP:\n" );
  fprintf ( fileout, "  End of hexadecimal dump.\n" );

  fclose ( fileout );

  printf ( "\n" );
  printf ( "HEXDUMP:\n" );
  printf ( "  Normal end of execution.\n" );

  return SUCCESS;
}
