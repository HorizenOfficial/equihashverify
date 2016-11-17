# equihashverify
nodejs native binding to check for valid Equihash solutions

##usage:
````javascript
var ev = require('bindings')('equihashverify.node');

var header = new Buffer(..., 'hex');
var solution = new Buffer(..., 'hex'); //do not include byte size preamble "fd4005"

ev.verify(header, solution);
//returns boolean
````

##help
https://zclassic.herokuapp.com
