#!/bin/bash

# this script should be able to generate an arm-none-eabi toolchain on your 
# system if not, try to fix it. :)

# you may need to run this as root, depending on your configuration.

# this script was generated using this guide:
#   http://www.kunen.org/uC/gnu_tool.html

set -e
set -u
set -x

BUILDROOT=${BUILDROOT:-/tmp}
PREFIX=${PREFIX:-/usr/local}

# set make flags in environment, e.g. -j2 for parallel builds
MFLAGS=${MFLAGS:-}

# clean up previous build, if any
rm -rf $BUILDROOT/{src,build}

# create build directories
mkdir -p $BUILDROOT
mkdir -p $BUILDROOT/{orig,src,build}

# create install directory
mkdir -p $PREFIX

# fetch required packages, if necessary
cd $BUILDROOT/orig
if [ ! -f gcc-4.3.3.tar.gz ]; then
  wget ftp://ftp.gnu.org/gnu/gcc/gcc-4.3.3/gcc-4.3.3.tar.gz
fi
if [ ! -f gcc-core-4.3.3.tar.gz ]; then
  wget ftp://ftp.gnu.org/gnu/gcc/gcc-4.3.3/gcc-core-4.3.3.tar.gz
fi
if [ ! -f gmp-4.1.tar.gz ]; then
  wget http://mirror.anl.gov/pub/gnu/gmp/gmp-4.1.tar.gz
fi
if [ ! -f mpfr-2.3.0.tar.gz ]; then
  wget http://www.mpfr.org/mpfr-2.3.0/mpfr-2.3.0.tar.gz
fi
if [ ! -f gdb-6.8.tar.gz ]; then
  wget http://mirrors.usc.edu/pub/gnu/gdb/gdb-6.8.tar.gz
fi
if [ ! -f binutils-2.19.tar.gz ]; then
  wget http://mirrors.usc.edu/pub/gnu/binutils/binutils-2.19.tar.gz
fi
if [ ! -f newlib-1.17.0.tar.gz ]; then
  wget ftp://sources.redhat.com/pub/newlib/newlib-1.17.0.tar.gz
fi

# unpack tarballs
cd $BUILDROOT/src
tar xzf ../orig/gcc-4.3.3.tar.gz
tar xzf ../orig/gcc-core-4.3.3.tar.gz
tar xzf ../orig/gmp-4.1.tar.gz
tar xzf ../orig/mpfr-2.3.0.tar.gz
tar xzf ../orig/gdb-6.8.tar.gz
tar xzf ../orig/binutils-2.19.tar.gz
tar xzf ../orig/newlib-1.17.0.tar.gz

# build binutils
mkdir $BUILDROOT/build/binutils-2.19
cd $BUILDROOT/build/binutils-2.19
# -Wno-unused-but-set-variable us required by morerecent versions of gcc.
../../src/binutils-2.19/configure --target=arm-none-eabi --prefix=$PREFIX --enable-interwork --enable-multilib CFLAGS="-g -O2 -Wno-unused-but-set-variable -Wno-unused-but-set-parameter -Wno-format-security"
make $MFLAGS all 
make install

export PATH="$PATH:$PREFIX/bin"

# build gcc compiler
mkdir $BUILDROOT/build/gcc-4.3.3
cd $BUILDROOT/build/gcc-4.3.3
# only enable C here
../../src/gcc-4.3.3/configure --target=arm-none-eabi --prefix=$PREFIX --enable-interwork --enable-multilib --enable-languages="c" --with-newlib --with-headers=../../src/newlib-1.17.0/newlib/libc/include --with-gmp=../../src/gmp-4.1 --with-mpfr=../../src/mpfr-2.3.0
make $MFLAGS all-gcc 
make install-gcc

# build newlib
mkdir $BUILDROOT/build/newlib-1.17.0
cd $BUILDROOT/build/newlib-1.17.0
../../src/newlib-1.17.0/configure --target=arm-none-eabi --prefix=$PREFIX --enable-interwork --enable-multilib
make $MFLAGS all 
make install

# finish gcc build
cd $BUILDROOT/build/gcc-4.3.3
make $MFLAGS all 
make install

# build gdb
mkdir $BUILDROOT/build/gdb-6.8
cd $BUILDROOT/build/gdb-6.8
../../src/gdb-6.8/configure --target=arm-none-eabi --prefix=$PREFIX --enable-interwork --enable-multilib CFLAGS="-g -O2 -Wno-unused-but-set-variable -Wno-unused-result -Wno-enum-compare"
make $MFLAGS all 
make install

# if you get here, you should have a working toolchain. congratulations!
echo "all done."
