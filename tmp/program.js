var sp = new (require('serialport')).SerialPort('COM5');
var fs = require('fs');
var argv = require('optimist').argv;
var hex = fs.readFileSync(argv.f || __dirname + '/blink.hex');
var ih = require('intelhex');
var start = Date.now();
var current = '';
var num = require('./format');

var bytes = [3, 0, 0, 1, 0, 0, 0, 0, 0];
var initialLength = bytes.length;
ih(hex.toString()).on('data', function(chunk) {
console.log(chunk);
  bytes = bytes.concat(chunk.bytes);  
}).on('end', function() {

  var length = bytes.length-initialLength;
  bytes = new Buffer(bytes);
  bytes.writeUInt32BE(length, initialLength-4)

  var where = 0;
  var collected = 0;
  var write = function(d) {
    if (d) {
      process.stdout.write(num(d[0]));

      collected+=d.length;
    }

    if (where >= bytes.length) {
      console.log('\n\nok', collected, bytes.length, length);
      process.exit();
    }

    var buf = new Buffer([bytes[where]]);
    
    
    sp.write(buf);
    where++;
  }

  sp.on('open', write);

  var done = false;
  sp.on('data', write);
});
