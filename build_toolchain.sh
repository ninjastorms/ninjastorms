#!/bin/bash

# this script should be able to generate an arm-none-eabi toolchain on your
# system if not, try to fix it. :)
# tested on Gentoo Linux, and Ubuntu 12.04 LTS (keep it cross-distro!)

# you may need to run this as root, depending on your configuration.
# basically, you just need full write permissions to the path specified in
# PREFIX.

# third party dependencies (probably incomplete):
#   - GNU make, gcc, ... (build-essential)
#   - texinfo

set -e
set -u

# set make flags in environment, e.g. -j2 for parallel builds
MFLAGS=${MFLAGS:-}
PREFIX=${PREFIX:-/usr/local}

BUILDROOT=${BUILDROOT:-/tmp/$0}

TARDIR=$BUILDROOT/tar
SRCDIR=$BUILDROOT/src
BUILDDIR=$BUILDROOT/build

# clean up previous build, if any
rm -rf $SRCDIR $BUILDDIR

# create directories
mkdir -p $TARDIR $SRCDIR $BUILDDIR
mkdir -p $PREFIX

# tweak versions as necessary
GCC_VERSION=4.8.0
GMP_VERSION=5.1.1
MPC_VERSION=1.0.1
MPFR_VERSION=3.1.2
GDB_VERSION=7.6
BINUTILS_VERSION=2.24
NEWLIB_VERSION=1.20.0

# fetch
WGETARGS="-nc -P $TARDIR --progress=dot:giga"
wget $WGETARGS ftp://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.bz2
wget $WGETARGS ftp://ftp.gnu.org/gnu/gmp/gmp-$GMP_VERSION.tar.xz
wget $WGETARGS ftp://ftp.gnu.org/gnu/mpc/mpc-$MPC_VERSION.tar.gz
wget $WGETARGS http://www.mpfr.org/mpfr-$MPFR_VERSION/mpfr-$MPFR_VERSION.tar.gz
wget $WGETARGS ftp://ftp.gnu.org/gnu/gdb/gdb-$GDB_VERSION.tar.bz2
wget $WGETARGS http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.bz2
wget $WGETARGS ftp://sources.redhat.com/pub/newlib/newlib-$NEWLIB_VERSION.tar.gz

# unpack
cd $SRCDIR
TARARGS="--checkpoint=.4096 --total"
tar $TARARGS -xf $TARDIR/gcc-$GCC_VERSION.tar.bz2
tar $TARARGS -xf $TARDIR/gmp-$GMP_VERSION.tar.xz
tar $TARARGS -xf $TARDIR/mpc-$MPC_VERSION.tar.gz
tar $TARARGS -xf $TARDIR/mpfr-$MPFR_VERSION.tar.gz
tar $TARARGS -xf $TARDIR/gdb-$GDB_VERSION.tar.bz2
tar $TARARGS -xf $TARDIR/binutils-$BINUTILS_VERSION.tar.bz2
tar $TARARGS -xf $TARDIR/newlib-$NEWLIB_VERSION.tar.gz

mv gmp-$GMP_VERSION gcc-$GCC_VERSION/gmp
mv mpc-$MPC_VERSION gcc-$GCC_VERSION/mpc
mv mpfr-$MPFR_VERSION gcc-$GCC_VERSION/mpfr

# build binutils
mkdir $BUILDDIR/binutils-$BINUTILS_VERSION
cd $BUILDDIR/binutils-$BINUTILS_VERSION
../../src/binutils-$BINUTILS_VERSION/configure \
    --target=arm-none-eabi --prefix=$PREFIX --enable-interwork --enable-multilib \
    CFLAGS="-g -O2 -Wno-error=unused-value"
make $MFLAGS all
make install

export PATH="$PATH:$PREFIX/bin"

# configure gcc
mkdir $BUILDDIR/gcc-$GCC_VERSION
cd $BUILDDIR/gcc-$GCC_VERSION
../../src/gcc-$GCC_VERSION/configure \
    --target=arm-none-eabi --prefix=$PREFIX --enable-interwork --enable-multilib \
    --enable-languages="c" \
    --with-newlib --with-headers=$SRCDIR/newlib-$NEWLIB_VERSION/newlib/libc/include

# build newlib
mkdir $BUILDDIR/newlib-$NEWLIB_VERSION
cd $BUILDDIR/newlib-$NEWLIB_VERSION
../../src/newlib-$NEWLIB_VERSION/configure \
    --target=arm-none-eabi --prefix=$PREFIX --enable-interwork --enable-multilib
make $MFLAGS all
make install

# build gcc
cd $BUILDDIR/gcc-$GCC_VERSION
make $MFLAGS all
make install

# build gdb
mkdir $BUILDDIR/gdb-$GDB_VERSION
cd $BUILDDIR/gdb-$GDB_VERSION
../../src/gdb-$GDB_VERSION/configure \
    --target=arm-none-eabi --prefix=$PREFIX --enable-interwork --enable-multilib \
    CFLAGS="-g -O2 -Wno-error=unused-value"
make $MFLAGS all
make install

echo "all done. :)"
