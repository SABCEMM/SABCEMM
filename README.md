# Simulator for Agent Based Computational Economic Market Models (SABCEMM)
Before getting started, have a look at our full [Doxygen Code Documentation](https://sabcemm.github.io/SABCEMM/).


# First steps 

This tutorial guides you through the first steps to use the SABCEMM framework on a linux.
If you are trying to use the framework on Windows or Mac, please have a look at 
[SABCEMM on Windows or Mac](https://github.com/SABCEMM/SABCEMM/wiki/SABCEMM-on-Windows-or-Mac) for further information.


Open a terminal and enter the following commands.

Get the source code:

`git clone https://github.com/SABCEMM/SABCEMM.git`

Update the submodules: 

`git submodule init
git submodule update`

Build the program:

`mkdir build
cd build
cmake ..
make`


SABCEMM and its test will be put in `build/src` and `build/test`, respectively.

Now, create an input file (see [Creating an Input File](https://github.com/SABCEMM/SABCEMM/wiki/Create-an-Input-File)) 
or use one of the existing files in src/input.
 
Then, SABCEMM is run like:
 
`cd src 
./financeSimulation PATH/TO/INPUT_FILE.xml `

The simulation results will be placed in a folder `output` (relative to the program's working directory).
 
Some of SABCEMM's features need additional libraries. Those features are turned
 off in CMake by default. See the section dependencies for further guidance.
Per default, SABCEMM does not have any dependencies.

# dependencies

SABCEMM uses CMake as a build-system. If you're not familiar with CMake, have a
 look at the [CMake Tutorial](https://cmake.org/cmake-tutorial/).

Header-only/small dependencies are bundled and possibly encapsuled in submodules (boost, tinyxml2)

However, you must install the following libraries manually:

### mandatory:
--none--

### optional:
* [Intel MKL](https://software.intel.com/en-us/intel-mkl/) (install manually, on arch you may try: pacaur -S intel-mkl)
* [NAG](http://www.nag.com/) (install manually) 
* HDF5 (on ubuntu: apt install libhdf5-dev, on arch: pacman -S hdf5-cpp-fortran)
* curses (on ubuntu: apt install libncurses5-dev, on arch: pacman -S curses)
* lcov for test coverage generation (on ubuntu: apt install lcov, on arch: pacaur -S lcov)

The code is compiled conditionally, depending on the available libraries and 
 user-controlled switches in CMake (`WITH_HDF5`, `WITH_INTEL_MKL`, `WITH_NAG`).
 
If dependencies are not met, cmake will fail and not generate Makefiles. Try "ccmake ." and fix the wrong paths.

# Special Notes

* Curses can be causing problems. As a workaround, add `-std=c99` to `CMAKE_C_FLAGS` und `-ltinfo` to `CMAKE_EXE_LINKER_FLAGS`.
* HDF is installed to a standard location and found automatically. Note that HDF5_DIR is set to "HDF5_DIR-NOTFOUND". That's ok!

