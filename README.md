# gridlab-d-federates
This repository contains the model and business logic code for market and demand controllers which interact with a gridlab D simulation and its associated federate.

## Installing Gridlab-D

You'll need to install the prerequisites for building gridlab-d:

```bash
sudo apt-get install automake autoconf libtool curl
```

Then you can checkout the source code for gridlab-d from the SVN repository:

```bash
svn co svn://svn.code.sf.net/p/gridlab-d/code/trunk gridlab-d-code
```

Then build and install gridlab-d on your system:

```baash
cd gridlab-d-code
autoreconf -isf
./configure
make
sudo make install
```

## Installing CPP-NETLIB

Since the gridlab-d federate communicates with GLD through the use of HTTP requests, the c++ code for the federate relies on [cpp-netlib 0.11.2](http://cpp-netlib.org).  To download and install cpp-netlib on your machine you must perform the following steps:

```bash
sudo apt-get install p7zip-full cmake
cd ~
wget http://downloads.cpp-netlib.org/0.11.2/cpp-netlib-0.11.2-final.zip
7z x cpp-netlib-0.11.2-final.zip
mv cpp-netlib-0.11.2-final cpp-netlib
mkdir cpp-netlib-build
cd cpp-netlib-build
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_C_COMPILER=gcc   \
      -DCMAKE_CXX_COMPILER=g++ \
      ../cpp-netlib
make
sudo make install
```

## Setting up the GLD Federates example

Make the `GridlabDHelloWorld` example folders in `$C2WTROOT`:

```bash
mkdir -p $C2WTROOT/examples/GridlabDHelloWorld/models/gme
mkdir -p $C2WTROOT/examples/GridlabDHelloWorld/models/gridlab-d
mkdir -p $C2WTROOT/examples/GridlabDHelloWorld/cpp/
```

Copy the model files to their respective locations:

```bash
cp src models/*.xme $C2WTROOT/examples/GridlabDHelloWorld/models/gme/.
cp src models/*.glm $C2WTROOT/examples/GridlabDHelloWorld/models/gridlab-d/.
```

Open the copied XME with GME and run the `C2W Main Interpreter` and the `C2W Deployment Interpreter`.

Copy everything in `src` to `$C2WTROOT/examples/GridlabDHelloWorld/cpp/.`

```bash
cp -r src/* $C2WTROOT/examples/GridlabDHelloWorld/cpp/.
```

Build the copied code:

```bash
cd $C2WTROOT/examples/GridlabDHelloWorld/cpp
cmake .
make
```
