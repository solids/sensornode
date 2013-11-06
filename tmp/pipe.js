var sp = new (require('serialport')).SerialPort('COM5');
var i = 0;
sp.on('data', function(data) {
  console.log(data, i++);
  // for (var i = 0; i<data.length; i++) {

  //   d = Number(data[i]).toString(16).toUpperCase()
  //   if (d.length < 2) {
  //     d = '0' + d;
  //   }

  //   console.log(d);
  // }
});

