var sp = new (require('serialport')).SerialPort('COM5');
var fs = require('fs');

sp.on('data', function(d) {
  console.log(d);
  console.log('done');
  process.exit();
});

sp.on('open', function() {
  sp.write(new Buffer([7]));
});
