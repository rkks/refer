/* It is often useful to control the memory layout of fields in a data structure to match an interface control definition, or to interface with hardware. Define a data structure matching the RS-232 Plug Definition. Use the 9-pin definition for brevity.

Pin Settings for Plug
(Reverse order for socket.)
__________________________________________
1  2  3  4  5  6  7  8  9  10 11 12 13
14 15 16 17 18 19 20 21 22 23 24 25
_________________
1  2  3  4  5
6  7  8  9

25 pin                        9 pin
1 - PG   Protective ground
2 - TD   Transmitted data     3
3 - RD   Received data        2
4 - RTS  Request to send      7
5 - CTS  Clear to send        8
6 - DSR  Data set ready       6
7 - SG   Signal ground        5
8 - CD   Carrier detect       1
9 - + voltage (testing)
10 - - voltage (testing)
11 -
12 - SCD  Secondary CD
13 - SCS  Secondary CTS
14 - STD  Secondary TD
15 - TC   Transmit clock
16 - SRD  Secondary RD
17 - RC   Receiver clock
18 -
19 - SRS  Secondary RTS            
20 - DTR  Data terminal ready      4
21 - SQD  Signal quality detector
22 - RI   Ring indicator           9
23 - DRS  Data rate select
24 - XTC  External clock
25 -
*/

struct RS232_data
{
  unsigned carrier_detect        : 1;
  unsigned received_data         : 1;
  unsigned transmitted_data      : 1;
  unsigned data_terminal_ready   : 1;
  unsigned signal_ground         : 1;
  unsigned data_set_ready        : 1;
  unsigned request_to_send       : 1;
  unsigned clear_to_send         : 1;
  unsigned ring_indicator        : 1;
};

/* Note: The order of the fields is implementation-defined (i.e. the first bit might be the least-significant one or the most-significant one). On GCC and MSVC++, the first bit is the least-significant one. Since the order of bits can't be assured but it could be important if we need to interact with hardware, the best way is to define bit masks; of course actual writing/reading to/from an hardware "register" greater than a single byte must be done taking care of endianness. */