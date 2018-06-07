# Equihash - Zcash Implementation
nodejs native binding to check for valid Equihash solutions

# Dependencies
````
sudo apt-get install build-essential libsodium-dev libboost-system-dev
````

# Usage:
````javascript
var ev = require('bindings')('equihashverify.node');

var header = new Buffer(..., 'hex');
var solution = new Buffer(..., 'hex'); //do not include byte size preamble "fd4005"

ev.verify(header, solution, n, k);
//returns boolean
````
# Test Suite:
````
sudo npm install -g mocha
npm install
mocha
````

