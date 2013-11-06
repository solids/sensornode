var width = 0;
module.exports = function(a, header) {
  if (typeof header !== 'undefined' && header) {
    return false;
  }

  a = Number(a).toString(16).toUpperCase()
  if (a.length < 2) {
    a = '0' + a;
  }

  width+=3;

  if (width>=47) {
    width = 0;
    return a + '\n';
  } else {
    return a  + ' ';
  }
}