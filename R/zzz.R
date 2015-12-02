## RQuantLib -- R interface to the QuantLib libraries
##
## Copyright (C) 2002 - 2015  Dirk Eddelbuettel <edd@debian.org>
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
## You should have received a copy of the GNU General Public
## License along with this program; if not, write to the Free
## Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
## MA 02111-1307, USA

#.First.lib <- function(lib, pkg) {
#  #cat("This is a pre-release. The interface might change...\n")
#  library.dynam("RQuantLib", pkg, lib )
#}

.onAttach <- function(libname, pkgname) {

    ## if it is not interactive (as eg in testing or cronjobs), do nothing
    if (!interactive()) return(invisible(NULL))

    ## else let's test the QuantLib version, and then the intraday capability
    qlver <- getQuantLibVersion()
    if (compareVersion(qlver, "1.7")) {
        packageStartupMessage("QuantLib version ", qlver, " detected which is older than 1.7.")
        packageStartupMessage("Intra-daily options analytics unavailable with that version.")
    } else if (!getQuantLibCapabilities()[["intradayDate"]]) {
        packageStartupMessage("Sufficient QuantLib version with insuffucient configuration.")
        packageStartupMessage("Try configuring your build with the --enable-intraday argument.")
    }
}
        
        
