var sp = new (require('serialport')).SerialPort('COM5');
var fs = require('fs');
var width = 0, header = 4, where = 0;
var argv = require('optimist').argv;
var num = require('./format')

var i = 0;
sp.on('data', function(d) {
  
  for (var i=0; i<d.length; i++) {
    if (!argv.v && !d.toString('hex', i).replace(/FF|04/ig, '').length) {
      break;
    }

    var out = num(d[i], header);

    if (out === false) {
      header--;
      continue;
    } else {
      where++;
    }
    
    process.stdout.write(out)
  }

  if (d[d.length-1]===4) {
    console.log('\n\nread', where, 'bytes befor nops');

    process.exit();
  }
  
});

sp.on('open', function() {
  sp.write(new Buffer([4]));
});
