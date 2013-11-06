var sp = new (require('serialport')).SerialPort('COM5');
var send = new Buffer([2, 0, 2, 254, 255]);


sp.on('data', function(d) {
  for (var i=0; i<d.length-1; i++) {
    if (d[i] !== send[i]) {
      throw new Error('forwarded data does not match recieved')
    }
  }

  console.log(d, d.length);
  console.log('ok!');
  process.exit();
});

sp.on('open', function() {
  console.log('open');
  sp.write(send);
});

