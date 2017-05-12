# Overview
This addon provides a wrapper of the C++ implementation of libsmrf for Node.js.

# Building
In order to build this addon, the C++ implementation of libsmrf must to be built and installed in advance.
By default, `/usr/local` as well as the system defaults are searched.
A custom install location of libsmrf can be supplied like this:

    npm install --SMRF_INCLUDE_DIR=/path/to/smrf-include-dir --SMRF_LIB_DIR=/path/to/smrf-lib-dir
