# Flexible Equihash Solution Verification
nodejs native binding to check for valid Equihash solutions

# Dependencies
````
sudo apt-get install build-essential libsodium-dev libboost-system-dev
````

# Usage
````javascript
var eq = require('equihashverify');

var header = new Buffer(..., 'hex');
var solution = new Buffer(..., 'hex'); //do not include byte size preamble, "fd4005" for 200,9

eq.verify(header, solution, n, k, personalization);
//returns boolean
````

n, k, and personalization are parameters to support validating solutions for many Equihash variants.

The most common Equihash parameters are:
* 200, 9, "ZcashPoW" which is what is commonly referred to as just "Equihash". Zencash uses this.
* 144, 5, "`personalization`" is commonly referred to as "Equihash 144,5".
  * An example personalization is "BgoldPoW" for Bitcoin Gold.

# Backward compatibility
````javascript
eq.verify(header, solution); // default is 200, 9, "ZcashPoW" (works for Zencash and others)
````

# Test Suite:
````
sudo npm install -g mocha
npm install
mocha
````

