
## RQuantLib -- R interface to the QuantLib libraries
##
## Copyright (C) 2002 - 2009 Dirk Eddelbuettel <edd@debian.org>
## Copyright (C) 2009        Khanh Nguyen <knguyen@cs.umb.edu>
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
## You should have received a copy of the GNU General Public
## License along with this program; if not, write to the Free
## Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
## MA 02111-1307, USA

AsianOption <- function(averageType, type, underlying, strike, dividendYield,
                        riskFreeRate, maturity, volatility,
                        first, length, fixings,
                        timeSteps=150, gridPoints=151) {
    UseMethod("AsianOption")
}

AsianOption.default <- function(averageType, type, underlying, strike, dividendYield,
                                riskFreeRate, maturity, volatility,
                                first=0, length=0, fixings=0,
                                timeSteps=150, gridPoints=151) {
    averageType <- match.arg(averageType, c("geometric", "arithmetic"))
    type <- match.arg(type, c("call", "put"))
    val <- .Call("QL_AsianOption",
                 list(averageType=as.character(averageType),
                      type=as.character(type),
                      underlying=as.double(underlying),
                      strike=as.double(strike),
                      dividendYield=as.double(dividendYield),
                      riskFreeRate=as.double(riskFreeRate),
                      maturity=as.double(maturity),
                      volatility=as.double(volatility),
                      first=as.double(first),
                      length=as.double(length),
                      fixings=as.double(fixings),
                      timeSteps=as.integer(timeSteps),
                      gridPoints=as.integer(gridPoints)),
                 PACKAGE="RQuantLib")
    class(val) <- c("AsianOption","Option")
    val
}


plot.Option <- function(x, ...) {
    warning("No plotting available for class", class(x)[1],"\n")
    invisible(x)
}

print.Option <- function(x, digits=4, ...) {
    cat("Concise summary of valuation for", class(x)[1], "\n")
    print(round(unlist(x[1:7]), digits))
    invisible(x)
}

summary.Option <- function(object, digits=4, ...) {
    cat("Detailed summary of valuation for", class(object)[1], "\n")
    print(round(unlist(object[1:7]), digits))
    cat("with parameters\n")
    print(unlist(object[["parameters"]]))
    invisible(object)
}

