var sp = new (require('serialport')).SerialPort('COM5');

sp.on('data', function(d) { 
  console.log(d, d.length)
});

sp.on('open', function() {
  console.log('open');
  sp.write(new Buffer([1, 10]));
})

