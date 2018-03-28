/* based on sample code from node-serialport on Github */

/* install nodejs 
   npm install serialport
   node read_serial.js 
*/

var serialport = require("serialport");
var SerialPort = serialport.SerialPort; 

var sp = new SerialPort("/dev/ttyACM0", {
   parser: serialport.parsers.readline('\n'),
   baudrate: 9600
 });

sp.on("open", function() {
  console.log('open');
  sp.on('data', function(data) {
    console.log('data received: ' + data);
  });
});
