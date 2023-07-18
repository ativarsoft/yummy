#!/bin/bash -e

libtoolize
aclocal
autoreconf -vfi
./configure
make

