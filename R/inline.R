## RQuantLib -- R interface to the QuantLib libraries
##
## Copyright (C) 2002 - 2014  Dirk Eddelbuettel <edd@debian.org>
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

    ## default to NULL
    qlcflags <- qllibs <- NULL
    
    if (.Platform$OS.type=="windows") {
        if (Sys.getenv("QUANTLIB_ROOT") != "") {
            ## by convention with CRAN, on Windows we expect these two
            ## variables to be set, and to be pointing to the respective
            ## helper installations
            qlroot   <- Sys.getenv("QUANTLIB_ROOT")
            boostlib <- Sys.getenv("BOOSTLIB")
            rarch    <- Sys.getenv("R_ARCH")
            qlcflags <- sprintf("-I%s -I. -I\"%s\"", qlroot, boostlib)
            qllibs   <- sprintf("-L%s/lib%s -lQuantLib", qlroot, rarch)
        }
    } else {
        ## on Linux and OS X, see if we have quantlib-config which may well be
        ## false in the case of prebuild binary packages as eg r-cran-rquantlib
        ## on Debian / Ubuntu as well as the OS X package from CRAN
        qc <- system("bash -c 'type -p quantlib-config'", ignore.stderr=TRUE, intern=TRUE)
        if (is.character(qc) && nchar(qc) > 1) {
            qlcflags <- system(paste(qc, "--cflags"), intern = TRUE)
            qllibs   <- system(paste(qc, "--libs"),   intern = TRUE)
        }
    }

    assign("ql_cflags", qlcflags, envir=.pkgglobalenv)
    assign("ql_libs", qllibs, envir=.pkgglobalenv)
}

LdFlags <- function(print = TRUE) {
    if (is.null(.pkgglobalenv$ql_libs)) stop("Cannot supply LdFlags as none set.")
    if (print) cat(.pkgglobalenv$ql_libs) else .pkgglobalenv$ql_libs
}

CFlags <- function(print = TRUE) {
    if (is.null(.pkgglobalenv$ql_cflags)) stop("Cannot supply CFlags as none set.")
    if (print) cat(.pkgglobalenv$ql_cflags) else .pkgglobalenv$ql_cflags
}

inlineCxxPlugin <- function(...) {
    plugin <- Rcpp.plugin.maker(include.before = "#include <RQuantLib.h>",
                                libs = sprintf("%s $(LAPACK_LIBS) $(BLAS_LIBS) $(FLIBS)", LdFlags(FALSE)),
                                package = "RQuantLib",
                                Makevars = NULL,
                                Makevars.win = NULL)
    settings <- plugin()
    settings$env$PKG_CPPFLAGS <- paste("-DRQuantLib_Plugin", CFlags(FALSE))
    settings
}
