var sp = new (require('serialport')).SerialPort('COM5');
var fs = require('fs');
var hex = fs.readFileSync(__dirname + '/blink.hex');
var ih = require('intelhex');
var start = Date.now();
var current = '';
var bytes = [4, 0, 0];
ih(hex.toString()).on('data', function(chunk) {
console.log(chunk);
  bytes = bytes.concat(chunk.bytes);  
}).on('end', function() {

  var length = bytes.length-3;
  bytes[1] = length >> 8;
  bytes[2] = length & 0x00FF;

  var where = 0;
  var collected = 0;
  var write = function(d) {
    if (d) {
      collected++;
    }

    var buf = new Buffer([bytes[where]]);
    process.stdout.write(buf[0] + ' ');
    sp.write(buf);
    where++;
  }

  sp.on('open', write);


  var done = false;
  sp.on('data', write);
  /*function(d) { 

    if (where < current.length) {
      process.stdout.write(d[0] === current[where] ? '.' : 'x');
      if (d[0] !== current[where-1]) {
        console.log(d[0], 'vs', current[where-1]);
        process.exit();

      }
      
      console.log(current[where]);
      sp.write(new Buffer([current[where]]))
      where++;
      return;
    }

    console.log('');
    if (lines.length) {
      current = lines.shift();
      console.log(current);
      where = 0;
      sp.write(new Buffer([current[where]]))
    } else if (d[1] === 255) {
      console.log('done in', (Date.now() - start)/1000, 'secs');
      process.exit();
    }
  });*/

});
