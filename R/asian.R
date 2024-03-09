
##  RQuantLib -- R interface to the QuantLib libraries
##
##  Copyright (C) 2002 - 2014  Dirk Eddelbuettel <edd@debian.org>
##  Copyright (C) 2009         Khanh Nguyen <knguyen@cs.umb.edu>
##
##  This file is part of RQuantLib.
##
##  RQuantLib is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 2 of the License, or
##  (at your option) any later version.
##
##  RQuantLib is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with RQuantLib.  If not, see <http://www.gnu.org/licenses/>.

AsianOption <- function(averageType, type, underlying, strike, dividendYield,
                        riskFreeRate, maturity, volatility,
                        first, length, fixings) {
    UseMethod("AsianOption")
}

AsianOption.default <- function(averageType, type, underlying, strike, dividendYield,
                                riskFreeRate, maturity, volatility,
                                first=0, length=11.0/12.0, fixings=26) {
    averageType <- match.arg(averageType, c("geometric", "arithmetic"))
    type <- match.arg(type, c("call", "put"))
    if (missing(maturity)) {
        if (averageType=="geometric") {
            warning("Geometric Asian Option requires maturity argument")
            return(NULL)
        } else {
            maturity <- 1.0             # actually unused for arithmetic option case
        }
    }
    val <- asianOptionEngine(averageType, type, underlying,
                             strike, dividendYield, riskFreeRate,
                             maturity, volatility,
                             first, length, fixings)
    class(val) <- c("AsianOption","Option")
    val
}
