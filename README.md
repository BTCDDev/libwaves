# libwaves:

A C library for WAVES, a cryptocurrency

2016 BTCDDev - mvplineup.com

https://wavesplatform.com/

https://bitcointalk.org/index.php?topic=1387944.0

## Features:

- Address Generation

### To Build:

Have Linux (Windows support coming soon) with clang installed.
Run
```sh
./build.sh
```

This will place `libwaves.a` into libwaves/o/


### To Test:

Run `build.sh` in test/address, then run the executable `address_test`

### note:

Right now, only address generation is supported, more functionality will come. Once you create an address from a seed, it will return a public/private keypair.

You can do an http POST request to `https://nodes.wavesnodes.com/waves/address` and include the public key in the request. The response will include your WAVES address.

### Version:
0.0.1


Errors or comments, please email at:

bitcoindarkdev@gmail.com
