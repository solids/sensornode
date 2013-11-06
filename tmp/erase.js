var sp = new (require('serialport')).SerialPort('COM5');
var fs = require('fs');

sp.on('data', function(d) {
  if (d[0] !== 5) {
    throw new Error('invalid response: ' + d[0]);
  }
  console.log('done');
  process.exit();
});

sp.on('open', function() {
  sp.write(new Buffer([5]));
});
