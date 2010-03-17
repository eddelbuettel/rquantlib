## RQuantLib -- R interface to the QuantLib libraries
##
## Copyright (C) 2002 - 2009 Dirk Eddelbuettel <edd@debian.org>
##
## $Id: implied.R,v 1.8 2005/10/12 03:20:39 edd Exp $
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

## also dumps core (0.3.7)
## no longer under 0.3.9 and 0.3.10 with g++ 3.4/4.0
EuropeanOptionImpliedVolatility <-
  function(type, value, underlying, strike, dividendYield,
           riskFreeRate, maturity, volatility) {
  UseMethod("EuropeanOptionImpliedVolatility")
}

EuropeanOptionImpliedVolatility.default <-
  function(type, value, underlying, strike, dividendYield,
            riskFreeRate, maturity, volatility) {
  val <- .Call("QL_EuropeanOptionImpliedVolatility",
               list(type=as.character(type),
		    value=as.double(value),
                    underlying=as.double(underlying),
                    strike=as.double(strike),
                    dividendYield=as.double(dividendYield),
                    riskFreeRate=as.double(riskFreeRate),
                    maturity=as.double(maturity),
                    volatility=as.double(volatility)),
               PACKAGE="RQuantLib")
  class(val) <- c("EuropeanOptionImpliedVolatility","ImpliedVolatility")
  val
}

# also dumps core (0.3.7)
## no longer under 0.3.9 and 0.3.10 with g++ 3.4/4.0
AmericanOptionImpliedVolatility <-
  function(type, value, underlying, strike, dividendYield, riskFreeRate,
           maturity, volatility, timeSteps=150, gridPoints=151) {
  UseMethod("AmericanOptionImpliedVolatility")
}

AmericanOptionImpliedVolatility.default <-
  function(type, value, underlying, strike, dividendYield, riskFreeRate,
            maturity, volatility, timeSteps=150, gridPoints=151) {
  val <- .Call("QL_AmericanOptionImpliedVolatility",
               list(type=as.character(type),
		    value=as.double(value),
                    underlying=as.double(underlying),
                    strike=as.double(strike),
                    dividendYield=as.double(dividendYield),
                    riskFreeRate=as.double(riskFreeRate),
                    maturity=as.double(maturity),


                    volatility=as.double(volatility),
                    timeSteps=as.integer(timeSteps),
                    gridPoints=as.integer(gridPoints)),
               PACKAGE="RQuantLib")
  class(val) <- c("AmericanOptionImpliedVolatility","ImpliedVolatility")
  val
}

# dumps core :-/
BinaryOptionImpliedVolatility <-
  function(type, value, underlying, strike, dividendYield, riskFreeRate,
           maturity, volatility, cashPayoff=1) {
  UseMethod("BinaryOptionImpliedVolatility")
}

BinaryOptionImpliedVolatility.default <-
  function(type, value, underlying, strike, dividendYield, riskFreeRate,
            maturity, volatility, cashPayoff=1) {
  val <- .Call("QL_BinaryOptionImpliedVolatility",
               list(type=as.character(type),
		    value=as.double(value),
                    underlying=as.double(underlying),
                    strike=as.double(strike),
                    dividendYield=as.double(dividendYield),
                    riskFreeRate=as.double(riskFreeRate),
                    maturity=as.double(maturity),
                    volatility=as.double(volatility),
                    cashPayoff=as.double(cashPayoff)),
               PACKAGE="RQuantLib")
  class(val) <- c("BinaryOptionImpliedVolatility","ImpliedVolatility")
  val
}

print.ImpliedVolatility <- function(x, digits=3, ...) {
  impvol <- x[[1]]
  cat("Implied Volatility for", class(x)[1], "is", round(impvol, digits), "\n")
  invisible(x)
}

summary.ImpliedVolatility <- function(object, digits=3, ...) {
  impvol <- object[[1]]
  cat("Implied Volatility for", class(object)[1], "is", round(impvol, digits), "\n")
  cat("with parameters\n")
  print(unlist(object[[2]]))
  invisible(object)
}
