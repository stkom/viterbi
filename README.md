# SystemC Viterbi
This project provides a SystemC implementation of a Viterbi decoder.

## Building the Project
### Under Cygwin
* Download SystemC 2.3.2
* Change to the top level directory (systemc-2.3.2)
* Create a temporary directory and change to it (e.g. `mkdir build` and `cd build`)
* Run cmake `cmake .. -DCMAKE_CXX_STANDARD=11 -DENABLE_PTHREADS=1`
* Build with `cmake --build .` or `cmake --build . -- -j 4` to use 4 threads
* Build this project with cmake

### Under Arch Linux
* Install the `systemc` package from the AUR
* Build this project with cmake

## Documentation
The documentation can be build with Doxygen.
