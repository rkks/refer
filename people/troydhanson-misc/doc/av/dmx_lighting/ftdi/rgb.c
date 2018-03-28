/* 
 * This program controls a DMX light fixture. 
 *
 * The light fixture is a Chauvet LED Techno Strobe RGB 
 * (http://www.chauvetlighting.com/led-techno-strobe-rgb.html). The DMX
 * channel codes it responds to (equating to byte sequence, here) is documented 
 * http://www.chauvetlighting.com/products/manuals/ST-4000_LED_Techno_Strobe_RGB_UM_Rev3_WO.pdf
 *
 * The DMX communication utilizes a USB-to-DMX adapter, 
 * an ENTTEC DMX USB Pro (http://www.enttec.com/dmxusb.php). It's byte
 * API is documented in http://www.enttec.com/docs/dmx_usb_pro_api_spec.pdf
 *
 * The ENTTEC adapter is a USB to serial (DMX) adapter, utilizing the
 * the FTDI chip. There is a FTDI library that uses libusb underneath
 * (http://www.intra2net.com/en/developer/libftdi). This program is
 * coded to that libftdi API.
 *
 * So, the FTDI library lets us talk to the ENTTEC, which has its own byte
 * protocol, of which we're only using one capability (send DMX message).  
 * The DMX message we send, speaks the Chauvet's DMX dialect to set its RGB,
 * strobe and dim settings.
 * 
 * The Chauvet light must have its DIP switches all set to "off".
 * If they are in another configuration it changes the DMX start address
 * that the fixture responds to, which would requiring adjusting the 
 * DMX message (essentially adding bytes because the DMX channels are
 * position-based, in the message buffer).
 *
 * Troy D. Hanson, April 14 2013
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ftdi.h>

char enttec_msg[] = {
  0x7e,  // start of message delimiter (Enttec USB Pro API)
  0x06,  // label to identify type of message (06==output only send DMX packet request)
  0x07,  // data length LSB, (how many bytes in Chauvet message below)
  0x00,  // data length MSB,
  // Chauvet DMX message starts here
    0x00,  // standard DMX Start code
    0x00,  // Chauvet LED Techno Strobe: select RGB mode
    0x8f,  // Chauvet LED Techno Strobe: R
    0x8f,  // Chauvet LED Techno Strobe:  G
    0x8f,  // Chauvet LED Techno Strobe:   B 
    0x00,  // Chauvet LED Techno Strobe: Strobe rate (lower=slower)
    0x80,  // Chauvet LED Techno Strobe: Dimming  (higher=dimmer)
  // Chauvet DMX message ends here
  0xe7,  // end of message delimiter (Enttec USB Pro API)
};

int main(int argc, char **argv)
{
    struct ftdi_context *ftdi;
    int f = 0;
    int vid = 0x403;  /* USB vendor ID  -- use 'lsusb' to see */
    int pid = 0x6001; /* USB product ID */

    if ((ftdi = ftdi_new()) == 0) {
        fprintf(stderr, "ftdi_new failed\n");
        return -1;
    }

    ftdi_set_interface(ftdi, INTERFACE_ANY);
    f = ftdi_usb_open(ftdi, vid, pid);
    if (f < 0)
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(ftdi));
        fprintf(stderr, "you may need to run as root\n");
        return -1;
    }

    f = ftdi_write_data(ftdi, enttec_msg, sizeof(enttec_msg));
    ftdi_usb_close(ftdi);
    ftdi_free(ftdi);
    return 0;
}
