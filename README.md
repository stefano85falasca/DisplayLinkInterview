Platform
======
The code has been tried and tested on various versions of ubuntu.

I don't have a Windows machine available, so I have not tried it on Windows.

Usage
======
* clone
* update submodules with `git submodule update --init --recursive`
* have cmake generate the build system, e.g. `cmake /path/to/src`
  * initial caches can be found, if desired, in src/cmake, e.g. `cmake -C /path/to/src/cmake/release.txt /path/to/src`
* compile

Testing
======
Several test programs are generated during build
* testcommonlib
* testtslib
* testcommon

Normal defensive programming techniques have been used throughout.

Assertions are used to perform sanity checking of the computations at various stages.

Modules
======
This is a contrived example that maybe didn't need dividing in modules, but I did.

* display - library providing the Display class
* common - library providing assertions
* testscreens - library providing the ramp generation facilities
* ramp - executable
