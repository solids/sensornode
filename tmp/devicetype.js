var sp = new (require('serialport')).SerialPort('COM5');

sp.on('data', function(d) { 
  console.log(d); 
  process.exit();
});

sp.on('open', function() {
  console.log('open');
  sp.write(new Buffer([9]));
});
