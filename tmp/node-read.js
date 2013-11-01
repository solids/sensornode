var sp = new (require('serialport')).SerialPort('COM5');
var fs = require('fs');

var num = function(a) {
  a = Number(a).toString(16).toUpperCase()
  if (a.length < 2) {
    a = '0' + a;
  }
  return a + ' ';
}

sp.on('data', function(d) {
  if (d[0] !== 3) { return; }

  for (var i=0; i<d.length; i+=4) {
    process.stdout.write(
      num(d[i+2]) + num(d[i+3])
    )
  }
  
  if (d.length === 1 && d[0] === 5) {
  	process.exit();
  }
});

sp.on('open', function() {
  sp.write(new Buffer([5]));
});
