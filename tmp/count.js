var sp = new (require('serialport')).SerialPort('COM5');

sp.on('data', function(d) { 
  console.log(d);
});

