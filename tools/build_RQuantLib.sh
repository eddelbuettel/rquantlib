#!/bin/sh
#set -x
set -e

###############################################################################
# NOTES:
#
# All directories must be specified as Windows paths (e.g. C:/Windows)
#
# This file has only been tested using MinGW's msys_1.0. Attempts to use msys2
# were not successful. Use msys.bat to call build_RQuantLib.sh
#
# Download QuantLib and boost sources and unzip them into $BUILD_ROOT/src.
# Update the $BOOST_VERSION and $QL_VERSION environment variables to reflect
# the versions used.
#
# The current RQuantLib Makefile deletes all object files and downloads the
# pre-built QuantLib library from https://github.com/rwinlib/quantlib. Update
# the Makefile to point to your local libQuantLib.a and remove/comment the
# 'make all' commands to prevent the deletion and download.

###############################################################################
# build environment
BOOST_VERSION=1_68_0
QL_VERSION=1.14
RQL_VERSION=0.4.5.2
BUILD_ROOT="E:/build"
R_TOOLS_DIR="E:/R/Rtools35"

BINPREF=$R_TOOLS_DIR/mingw_32/bin/
BINPREF64=$R_TOOLS_DIR/mingw_64/bin/

QUANTLIB_SRC="$BUILD_ROOT/src/QuantLib-$QL_VERSION"
BOOSTLIB="$BUILD_ROOT/src/boost_$BOOST_VERSION"
QUANTLIB_ROOT="$BUILD_ROOT/QuantLib"

###############################################################################
# Check Rtools locations
if [ ! -d "$R_TOOLS_DIR/mingw_32" ]; then
    echo "$R_TOOLS_DIR/mingw_32 not found"
    exit 1
fi
if [ ! -d "$R_TOOLS_DIR/mingw_64" ]; then
    echo "$R_TOOLS_DIR/mingw_64 not found"
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

# Install architecture-specific files into these directories via the
# --exec-prefix argument to QuantLib's configure script
mkdir -p $QUANTLIB_ROOT/lib/i386
mkdir -p $QUANTLIB_ROOT/lib/x64

# Ensure a mount point exists and add bin/ to PATH
mkdir -p /mingw
export PATH="$(cygpath -u $R_TOOLS_DIR)/bin":/mingw/bin:$PATH

# Use ccache if available
if [ -x "$(command -v ccache)" ]; then
	export PATH=/usr/lib/ccache/bin:$PATH
fi

################################################################################
## 32-bit QuantLib
mount --replace $R_TOOLS_DIR/mingw_32 /mingw

    ## Create and change to build directory
    #mkdir -p $BUILD_ROOT/ql32
    #cd $BUILD_ROOT/ql32

# QuantLib 'buglet' prevents building outside source directory
cd $QUANTLIB_SRC

# Clean prior build before (re-)running configure
if [ -f "Makefile" ]; then
  make -j6 distclean
fi

CXXFLAGS="-g0 -O2" $QUANTLIB_SRC/configure --prefix=$QUANTLIB_ROOT \
    --exec-prefix=$QUANTLIB_ROOT/lib/i386 --with-boost-include=$BOOSTLIB \
    --enable-intraday --enable-shared --enable-static --with-gnu-ld
make -j6
make install

# Shrink quantlib library
cp $QUANTLIB_ROOT/lib/i386/libQuantLib.a \
   $QUANTLIB_ROOT/lib/i386/libQuantLib.a.bak
strip --strip-unneeded $QUANTLIB_ROOT/lib/i386/libQuantLib.a

################################################################################
## 64-bit QuantLib
mount --replace $R_TOOLS_DIR/mingw_64 /mingw

  ## Create and change to build directory
  #mkdir -p $BUILD_ROOT/ql64
  #cd $BUILD_ROOT/ql64

# QuantLib 'bug' prevents building outside source directory
cd $QUANTLIB_SRC

# Clean prior build before (re-)running configure
if [ -f "Makefile" ]; then
  make -j6 distclean
fi

CXXFLAGS="-m64 -g0 -O2" $QUANTLIB_SRC/configure --prefix=$QUANTLIB_ROOT \
    --exec-prefix=$QUANTLIB_ROOT/lib/x64 --with-boost-include=$BOOSTLIB \
    --enable-intraday --enable-shared --enable-static --with-gnu-ld
make -j6
make install

## Shrink quantlib library
cp $QUANTLIB_ROOT/lib/x64/libQuantLib.a \
   $QUANTLIB_ROOT/lib/x64/libQuantLib.a.bak
strip --strip-unneeded $QUANTLIB_ROOT/lib/x64/libQuantLib.a

###############################################################################
# Install RQuantLib
#export QUANTLIB_ROOT
#export BOOSTLIB
# RQuantLib expects headers in $QUANTLIB_ROOT/ql, not $QUANTLIB_ROOT/include/ql
#ln -s $QUANTLIB_ROOT/include/ql $QUANTLIB_ROOT/.

#mkdir -p $BUILD_ROOT/library
#R_CMD="Rscript -e 'install.packages(c(\"Rcpp\",\"zoo\"), lib = \"$BUILD_ROOT/library\")'"
#Rscript -e $R_CMD
#
## Need to remove the OS_type: unix from DESCRIPTION
#cd $BUILD_ROOT
#R CMD INSTALL RQuantLib_${RQL_VERSION}.tar.gz --library=$BUILD_ROOT/library --no-clean-on-error --preclean --build
#
## run tests on each system
#Rscript --arch x64 -e 'source(system.file(package=\"RQuantLib\", \"unitTests/runTests.R\"))'
#Rscript --arch i386 -e 'source(system.file(package=\"RQuantLib\", \"unitTests/runTests.R\"))'
