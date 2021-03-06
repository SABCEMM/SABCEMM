# Corresponding Publications
There are two publications using SABCEMM:

* **[SABCEMM-A Simulator for Agent-Based Computational Economic Market Models](https://arxiv.org/abs/1801.01811)**
* **[Simulation of Stylized Facts in Agent-Based Computational Economic Market Models](https://arxiv.org/abs/1812.02726)**

If you would like to have your publication/report listed as well, please drop us an email.

# Simulator for Agent Based Computational Economic Market Models (SABCEMM)
Before getting started, have a look at our full [Doxygen Code Documentation](https://sabcemm.github.io/SABCEMM/) and at our [User Guide](https://github.com/SABCEMM/SABCEMM/wiki/User-Guide). In case of any problems or if you have suggestions/propositions/comments for futur releases we encourage everyone to open an [issue](https://github.com/SABCEMM/SABCEMM/issues).

We are using [Git](https://git-scm.com) ([Tutorial](https://git-scm.com/book/en/v2/Git-Basics-Getting-a-Git-Repository), [Installing](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)) for version control and distribution of the code and [CMake](https://cmake.org) ([Tutorial](https://cmake.org/cmake-tutorial/), [Installing](https://cmake.org/install/)) to manage the compiling and linking of the software. If you have not worked with this software, you can find useful information using the provided links.

We put great effort into making this software accesible to everyone, however all computer/operating system are different. Our tutorials might not cover every step necessary for your machine. We encourage everyone to open an [issue](https://github.com/SABCEMM/SABCEMM/issues) in case of any problems, so that our team may assist you.

# Quickstart with Docker

The easiest way to get use SABCEMM is with our prebuildt [Docker](https://www.docker.com/) image. Our [Docker](https://www.docker.com/) image is a portable Linux machine with a precompiled version of SABCEMM and all necessary dependencies.
On your machine you use a Docker container to run our image.

1. Follow the [Get Started](https://www.docker.com/get-started) guide on docker.com to install Docker on your computer
1. Open a terminal and download our docker image `docker pull sabcemm/sabcemm`. This can take a couple of minutes.
1. Now create a docker container called _sabcemm_container_ by typing `docker run --name sabcemm_container -it sabcemm/sabcemm` in the command prompt. You are now connected (similiar to a SSH) to your container. By prompting `cd /home/SABCEMM/build/src` you navigate to the current SABCEMM build.
1. To execute a simulation call `./financeSimulation ../../input/examples/Cross.xml`. The simulation results can be found in the `/home/SABCEMM/build/src/output` folder.
1. To stop your docker container prompt `exit`. Your container is now stopped, but all your data is still availiable. To run the container again and reconnect prompt `docker start sabcemm_container`. To remove the container call `docker rm sabcemm_container`. Attention: This deletes all the data inside the container!


# Manual Installation: First steps 

This tutorial guides you through the first steps to use the SABCEMM framework on a **Linux** ([Ubuntu](https://www.ubuntu.com),[CentOS](https://www.centos.org), ...) operating system..
If you are trying to use the simulator on a Windows or Mac, please have a look at 
[SABCEMM on Windows or Mac](https://github.com/SABCEMM/SABCEMM/wiki/SABCEMM-on-Windows-or-Mac) for further information.

Open a terminal and enter the following commands.

Get the source code:

`git clone https://github.com/SABCEMM/SABCEMM.git`

`cd SABCEMM`

Update the submodules: 

`git submodule init`

`git submodule update`

Build the program:

`mkdir build`

`cd build`

`cmake ..`

We assume that your default compiler is [GCC](https://gcc.gnu.org)([Tutorial](https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/)) Version 7 or newer. You can check your version with `gcc -v`. In case your version is not 7 or newer, please talk to your system administrator on how to install and configure it for you. It is also possible to change the compiler in the `CMakeCache.txt` file. Point the `CMAKE_C_COMPILER` and `CMAKE_CXX_COMPILER` variables to the correct path and rerun `cmake ..`. In case of CMake failing have a look at the [dependencies section](https://github.com/SABCEMM/SABCEMM#dependencies) below. Now you are all set and can compile the software with:

`make`

This may take a minute while your computer compiles SABCEMM.

SABCEMM and its test will be put in `build/src` and `build/test`, respectively.

Now, create an input file (see [Creating an Input File](https://github.com/SABCEMM/SABCEMM/wiki/Create-an-Input-File)) 
or use one of the existing files in `XXX/SABCEMM/src/input`.
 
To run SABCEMM change into the `build/src` folder:
 
`cd src`

and call

`./financeSimulation PATH/TO/INPUT_FILE.xml `

The simulation results will be placed in the folder `output` (relative to the program's working directory).
 
Some of SABCEMM's features need additional libraries. Those features are turned
 off in CMake by default. See the section dependencies for further guidance.
Per default, SABCEMM does not have any dependencies.

# dependencies

SABCEMM uses CMake as a build-system. If you're not familiar with CMake, have a
 look at the [CMake Tutorial](https://cmake.org/cmake-tutorial/).

Header-only/small dependencies are bundled and possibly encapsuled in submodules (boost, tinyxml2)

However, you must install the following libraries manually:

### mandatory:
* [Boost Libraries](https://www.boost.org/)([Getting Started](https://www.boost.org/doc/libs/1_66_0/more/getting_started/unix-variants.html)) 
* curses (Please ask your system administrator to install this software for you. On ubuntu: apt install libncurses5-dev, on arch: pacman -S curses)

### optional:
* [Intel MKL](https://software.intel.com/en-us/intel-mkl/) ([Download](https://software.intel.com/en-us/mkl/choose-download), [Installing](https://software.intel.com/en-us/articles/intel-math-kernel-library-intel-mkl-2018-install-guide))
* [NAG](http://www.nag.com/) ([Download](https://www.nag.co.uk/content/downloads-nag-c-library-versions), [Installing](https://www.nag.co.uk/content/installing-nag-c-library-mark-261-and-library-documentation?ProdCode=cll6i261dl)) 
* [HDF5](https://support.hdfgroup.org/HDF5/) ([Download](https://www.hdfgroup.org/downloads/), [Installing](https://support.hdfgroup.org/HDF5/release/obtainsrc.html))
* [OpenMP](https://www.openmp.org/)([Setup](https://en.wikibooks.org/wiki/OpenMP/Setup)) Optional feature to run multiple simulations in parallel.
* [lcov](http://ltp.sourceforge.net/coverage/lcov.php) for test coverage generation ([User Guide](http://ltp.sourceforge.net/coverage/lcov/readme.php))


The code is compiled conditionally, depending on the available libraries and user-controlled switches in CMake (`WITH_HDF5`, `WITH_INTEL_MKL`, `WITH_NAG`, `WITH_OPENMP`). Edit the `CMakeCache.txt` and change those variables to ON if you would like to use these libraries. Alternativly use [CCMake](https://cmake.org/cmake/help/v3.0/manual/ccmake.1.html).
 
If dependencies are not met, cmake will fail and not generate Makefiles. Try `ccmake .` and fix the wrong paths.

# Special Notes

* Curses can be causing problems. As a workaround, add `-std=c99` to `CMAKE_C_FLAGS` und `-ltinfo` to `CMAKE_EXE_LINKER_FLAGS`.
* HDF is installed to a standard location and found automatically. Note that HDF5_DIR is set to "HDF5_DIR-NOTFOUND". That's ok!
* When running simulations in parallel we advise to disable the progress bar by disableling `WITH_PROGBAR`!
