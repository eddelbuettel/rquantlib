#!/bin/sh

set -e

#####################################################################
# build environment
BOOST_VERSION=1_61_0
QL_VERSION=1.8.1
RQL_VERSION=0.4.3
BUILD_ROOT="c:/opt/build"

QUANTLIB_SRC="$BUILD_ROOT/src/QuantLib-$QL_VERSION"
BOOSTLIB="$BUILD_ROOT/src/boost_$BOOST_VERSION"
QUANTLIB_ROOT="$BUILD_ROOT/QuantLib"

#####################################################################
# Check Rtools locations
if [ ! -d "c:/opt/Rtools/mingw_32" ]; then
    echo "c:/opt/Rtools/mingw_32 not found"
    exit 1
fi
if [ ! -d "c:/opt/Rtools/mingw_64" ]; then
    echo "c:/opt/Rtools/mingw_64 not found"
    exit 1
fi
# Check for boost headers
if [ ! -d "$BOOSTLIB/boost" ]; then
    echo "Did not find boost headers in $BOOSTLIB/boost"
    exit 1
fi
# Check for quantlib source
if [ ! -d "$QUANTLIB_SRC" ]; then
    echo "Did not find QuantLib source in $QUANTLIB_SRC"
    exit 1
fi
# Ensure there is an /etc/fstab
if [ -n "/etc/fstab" ]; then
    touch /etc/fstab
fi

mkdir -p $QUANTLIB_ROOT/lib
# RQuantLib expects 32/64-bit libraries in these respective directories
mkdir $QUANTLIB_ROOT/lib/i386
mkdir $QUANTLIB_ROOT/lib/x64
cd $QUANTLIB_SRC

#####################################################################
# 32-bit QuantLib
umount /mingw
mount c:/opt/Rtools/mingw_32 /mingw

make clean
configure --with-boost-include=$BOOSTLIB --prefix=$QUANTLIB_ROOT --enable-intraday
make #-j
make install

# Move 32-bit build into lib/i386
mv $QUANTLIB_ROOT/lib/pkg* $QUANTLIB_ROOT/lib/i386
mv $QUANTLIB_ROOT/lib/lib* $QUANTLIB_ROOT/lib/i386

# Shrink quantlib library
cp $QUANTLIB_ROOT/lib/i386/libQuantLib.a $QUANTLIB_ROOT/lib/i386/libQuantLib.a.bak
strip --strip-unneeded $QUANTLIB_ROOT/lib/i386/libQuantLib.a

#####################################################################
# 64-bit QuantLib
umount /mingw
mount c:/opt/Rtools/mingw_64 /mingw

make clean
CXXFLAGS="-m64" configure --with-boost-include=$BOOSTLIB --prefix=$QUANTLIB_ROOT --enable-intraday
make -j
make install

# Move 64-bit build into lib/x64
mv $QUANTLIB_ROOT/lib/pkg* $QUANTLIB_ROOT/lib/x64
mv $QUANTLIB_ROOT/lib/lib* $QUANTLIB_ROOT/lib/x64

# Shrink quantlib library
cp $QUANTLIB_ROOT/lib/x64/libQuantLib.a $QUANTLIB_ROOT/lib/x64/libQuantLib.a.bak
strip --strip-unneeded $QUANTLIB_ROOT/lib/x64/libQuantLib.a

#####################################################################
# Install RQuantLib
export QUANTLIB_ROOT
export BOOSTLIB
# RQuantLib expects headers in $QUANTLIB_ROOT/ql, not $QUANTLIB_ROOT/include/ql
ln -s $QUANTLIB_ROOT/include/ql $QUANTLIB_ROOT/.

cd $BUILD_ROOT
R CMD INSTALL RQuantLib_0.4.3.tar.gz
#R CMD INSTALL RQuantLib_${RQL_VERSION}.tar.gz

# run tests on each system
Rscript --arch x64 -e 'source(system.file(package=\"RQuantLib\", \"unitTests/runTests.R\"))'
Rscript --arch i386 -e 'source(system.file(package=\"RQuantLib\", \"unitTests/runTests.R\"))'

