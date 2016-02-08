# gridlab-d-federates
This repository contains the model and business logic code for market and demand controllers which interact with a gridlab D simulation and its associated federate.

## Installing Gridlab-D
-----------------------

You'll need to install the prerequisites for building gridlab-d:

```bash
sudo apt-get install automake autoconf libtool curl
```

THen you can checkout the source code for gridlab-d from the SVN repository:

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

## Setting up the GLD Federates example
---------------------------------------