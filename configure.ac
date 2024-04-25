# RQuantLib configure.in by Dirk Eddelbuettel <edd@debian.org>
#
# Using pieces borrowed from RPgSQL, GNU Gretl, GNU R and QuantLib
# Greatly simplified thanks to quantlib-config
# Another contribution by Kurt Hornik gratefully acknowledged

# require at least autoconf 2.69
AC_PREREQ([2.69])

# Process this file with autoconf to produce a configure script.

# Would like to extract R package version from R DESCRIPTION file
AC_INIT([RQuantlib],[0.4.22],[edd@debian.org])

# Set CXX and CXXFLAGS based on the R configuration (with thanks to KH)
: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
    AC_MSG_ERROR([Could not determine R_HOME.])
fi
CXX=`${R_HOME}/bin/R CMD config CXX`
CXXFLAGS=`"${R_HOME}/bin/R" CMD config CXXFLAGS`

# We are using C++
AC_LANG(C++)
AC_REQUIRE_CPP

AC_PROG_CXX
if test "${GXX}" = yes; then
    gxx_version=`${CXX} -v 2>&1 | grep "^.*g.. version" | \\
		       sed -e 's/^.*g.. version *//'`
    case ${gxx_version} in
	4.6.*|4.7.*|4.8.*|4.9.*|5.*|6.*|7.*|8.*|9.*|10.*|11.*|12.*|13.*)
	     gxx_newer_than_45="-fpermissive"
	;;
    esac
fi

AC_DEFUN([AC_PROG_R], [AC_CHECK_PROG(R,R,yes)])
AC_PROG_R

## We no longer need to set LDFLAGS for Rcpp 0.11.0 or later,
## and hence no longer need to check for Rscipt
##
## ## look for Rscript, but use the one found via R_HOME to allow for multiple installations
## AC_DEFUN(AC_PROG_RSCRIPT, [AC_CHECK_PROG(RSCRIPT,Rscript,yes)])
## AC_PROG_RSCRIPT
## #AC_MSG_CHECKING([for Rscript])
## if test x"${RSCRIPT}" == x"yes" ; then
## #     AC_MSG_RESULT([yes])
##      ## Rcpp compile flag providing header directory containing Rcpp.h
##      #rcpp_cxxflags=`${R_HOME}/bin/Rscript -e 'Rcpp:::CxxFlags()'`
##      ## link flag providing libary as well as path to library, and optionally rpath
##      rcpp_ldflags=`${R_HOME}/bin/Rscript -e 'Rcpp:::LdFlags()'`
##      ## now use all these
##      #AC_SUBST([CXXFLAGS],["${CXXFLAGS} $rcpp_cxxflags"])
##      AC_SUBST([LDFLAGS],["${LDFLAGS} $rcpp_ldflags"])
## else
## #    AC_MSG_RESULT([no])
##     echo "
##   Your installation does not appear to have Rscript installed.
##
##   Please make sure that you have a working and complete R installation.
## "
##     exit 1
## fi

# borrowed from a check for gnome in GNU gretl: define a check for quantlib-config
AC_DEFUN([AC_PROG_QUANTLIB], [AC_CHECK_PROG(QUANTLIB,quantlib-config,yes)])
AC_PROG_QUANTLIB
if test x"${QUANTLIB}" == x"yes" ; then
    # use quantlib-config for QL settings
    pkg_cxxflags=`quantlib-config --cflags`
    pkg_libs=`quantlib-config --libs`
    pkg_version=`quantlib-config --version`
    ##echo "Seeing ${pkg_cxxflags} ${pkg_libs} ${pkg_version}"

    # also test for quantlib version using result from quantlib-config --version
    case ${pkg_version} in
    0.1.*|0.2.*|0.3.*|0.4.*|0.8.*|0.9.*|1.0.*|1.1.*|1.2.*|1.3.*|1.4.*|1.5.*|1.6.*|1.7.*|1.8*|1.9.*|1.10.*|1.11.*|1.12.*|1.13.*|1.14.*|1.15.*|1.16.*|1.17.*)
	AC_MSG_WARN([RQuantLib requires QuantLib (>= 1.18).])
	AC_MSG_ERROR([Please upgrade to a current version.])
	;;
    1.18.*|1.19.*|1.20.*|1.21.*|1.22.*|1.23.*|1.24.*)
	echo "checking for suitable QuantLib version... sort-of (${pkg_version}). Consider upgrading."
	;;
    1.25.*|1.26*|1.27*|1.28*|1.29*|1.30*|1.31*)
	echo "checking for suitable QuantLib version... yes (${pkg_version})"
	;;
    esac

    # now use all these
    AC_SUBST([CXXFLAGS],["${CXXFLAGS} "])
    AC_SUBST([CXXFLAGS],["${CXXFLAGS} ${pkg_cxxflags} ${gxx_newer_than_45}"])
    AC_SUBST([LDFLAGS],["${LDFLAGS} $pkg_libs"])
elif test "`uname`" == "Darwin" ; then
  quantlib="https://github.com/autobrew/bundler/releases/download/quantlib-1.31.1/quantlib-1.31.1-universal.tar.xz"
  quantlibdir="$PWD/.deps"
  mkdir -p $quantlibdir
  echo "downloading $quantlib"
  curl -sSL "$quantlib" -o quantlib.tar.xz
  tar xf quantlib.tar.xz --strip 1 -C "$quantlibdir"
  rm quantlib.tar.xz
  pkg_cxxflags="-I${quantlibdir}/include"
  pkg_libs="-L${quantlibdir}/lib -lQuantLib"

  # Same as above
  AC_SUBST([CXXFLAGS],["${CXXFLAGS} $pkg_cxxflags"])
  AC_SUBST([LDFLAGS],["${LDFLAGS} $pkg_libs"])
else
    AC_MSG_ERROR([Please install QuantLib before trying to build RQuantLib.])
fi


## check for boost, this segment as well as segment below are
## from QuantLib's configure.ac
##
##
## Help the compiler find external software
AC_ARG_WITH([boost-include],
            AS_HELP_STRING([--with-boost-include=INCLUDE_PATH],[Supply the location of Boost header files]),
            [ql_boost_include_path=$withval],
            [ql_boost_include_path=""])
if test [ -n "$ql_boost_include_path" ] ; then
   AC_SUBST([CXXFLAGS],["${CXXFLAGS} -I${ql_boost_include_path}"])
fi
AC_ARG_WITH([boost-lib],
            AS_HELP_STRING([--with-boost-lib=LIB_PATH],[Supply the location of Boost libraries]),
            [ql_boost_lib_path=$withval],
            [ql_boost_lib_path=""])
if test [ -n "$ql_boost_lib_path" ] ; then
   AC_SUBST([LDFLAGS],["${LDFLAGS} -L${ql_boost_lib_path}"])
fi
##
## Check for Boost components
# QL_CHECK_BOOST_DEVEL
# --------------------
# Check whether the Boost headers are available
AC_DEFUN([QL_CHECK_BOOST_DEVEL],
    [AC_MSG_CHECKING([for Boost development files])
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[@%:@include <boost/version.hpp>
     @%:@include <boost/shared_ptr.hpp>
     @%:@include <boost/assert.hpp>
     @%:@include <boost/current_function.hpp>]], [[]])],[AC_MSG_RESULT([yes])],[AC_MSG_RESULT([no])
     AC_MSG_ERROR([Boost development files not found])
    ])
])

# QL_CHECK_BOOST_VERSION
# ----------------------
# Check whether the Boost installation is up to date
AC_DEFUN([QL_CHECK_BOOST_VERSION],
   [AC_MSG_CHECKING([for minimal Boost version])
    AC_REQUIRE([QL_CHECK_BOOST_DEVEL])
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[@%:@include <boost/version.hpp>]], [[@%:@if BOOST_VERSION < 106500
     @%:@error too old
     @%:@endif]])],[AC_MSG_RESULT([yes])],[AC_MSG_RESULT([no])
     AC_MSG_ERROR([outdated Boost installation])
    ])
])

# QL_CHECK_BOOST
# ------------------------
# Boost-related tests
AC_DEFUN([QL_CHECK_BOOST],
    [AC_REQUIRE([QL_CHECK_BOOST_DEVEL])
     AC_REQUIRE([QL_CHECK_BOOST_VERSION])
])
QL_CHECK_BOOST


AC_SUBST(CXXFLAGS)
AC_SUBST(LDFLAGS)
AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT

echo "Configured ${PACKAGE_NAME} ${PACKAGE_VERSION}"

### Local Variables: ***
### mode: autoconf ***
### autoconf-indentation: 4 ***
### End: ***
