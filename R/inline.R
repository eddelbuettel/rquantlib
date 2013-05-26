## RQuantLib -- R interface to the QuantLib libraries
##
## Copyright (C) 2002 - 2013 Dirk Eddelbuettel <edd@debian.org>
##
## $Id$
##
## This file is part of the RQuantLib library for GNU R.
## It is made available under the terms of the GNU General Public
## License, version 2, or at your option, any later version,
## incorporated herein by reference.
##
## This program is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
## PURPOSE.  See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public License
## along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

.pkgglobalenv <- new.env(parent=emptyenv())

.onLoad <- function(libname, pkgname) {
    if (.Platform$OS.type=="windows") {
        ## by convention with CRAN, on Windows we expect these two
        ## variables to be set, and to be pointing to the respective
        ## helper installations
        qlroot   <- Sys.getenv("QUANTLIB_ROOT")
        boostlib <- Sys.getenv("BOOSTLIB")
        rarch    <- Sys.getenv("R_ARCH")
        qlcflags <- sprintf("-I%s -I. -I\"%s\"", qlroot, boostlib)
        qllibs   <- sprintf("%s -L%s/lib%s -lQuantLib", Rcpp:::LdFlags(), rarch)
    } else {
        qlcflags <- system( "quantlib-config --cflags", intern = TRUE)
        qllibs   <- system( "quantlib-config --libs",   intern = TRUE)
    }

    assign("ql_cflags", qlcflags, envir=.pkgglobalenv)
    assign("ql_libs", qllibs, envir=.pkgglobalenv)
}

LdFlags <- function(print = TRUE) {
    if (print) cat(.pkgglobalenv$ql_libs) else .pkgglobalenv$ql_libs
}

CFlags <- function(print = TRUE) {
    if (print) cat(.pkgglobalenv$ql_cflags) else .pkgglobalenv$ql_cflags
}

inlineCxxPlugin <- function(...) {
    plugin <-
        Rcpp:::Rcpp.plugin.maker(include.before = "#include <rquantlib.h>",
                                 libs = sprintf("%s $(LAPACK_LIBS) $(BLAS_LIBS) $(FLIBS)", LdFlags(FALSE)),
                                 package = "RQuantLib", Makevars = NULL, Makevars.win = NULL)
    settings <- plugin()
    settings$env$PKG_CPPFLAGS <- CFlags(FALSE)
    settings
}



