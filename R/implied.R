##  RQuantLib -- R interface to the QuantLib libraries
##
##  Copyright (C) 2002 - 2014  Dirk Eddelbuettel <edd@debian.org>
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

## also dumps core (0.3.7)
## no longer under 0.3.9 and 0.3.10 with g++ 3.4/4.0
EuropeanOptionImpliedVolatility <- function(type, value, underlying,
                                            strike, dividendYield,
                                            riskFreeRate, maturity,
                                            volatility) {
        UseMethod("EuropeanOptionImpliedVolatility")
}

EuropeanOptionImpliedVolatility.default <- function(type, value, underlying,
                                                    strike, dividendYield,
                                                    riskFreeRate, maturity,
                                                    volatility) {
    
    val <- europeanOptionImpliedVolatilityEngine(type, value, underlying, strike,
                                                 dividendYield, riskFreeRate,
                                                 maturity, volatility)
    class(val) <- c("EuropeanOptionImpliedVolatility","ImpliedVolatility")
    val
}

# also dumps core (0.3.7)
## no longer under 0.3.9 and 0.3.10 with g++ 3.4/4.0
AmericanOptionImpliedVolatility <- function(type, value, underlying, strike,
                                            dividendYield, riskFreeRate,
                                            maturity, volatility,
                                            timeSteps=150, gridPoints=151) {
    UseMethod("AmericanOptionImpliedVolatility")
}

AmericanOptionImpliedVolatility.default <- function(type, value, underlying, strike,
                                                    dividendYield, riskFreeRate, maturity,
                                                    volatility, timeSteps=150, gridPoints=151) {
    val <- americanOptionImpliedVolatilityEngine(type, value, underlying, strike, dividendYield,
                                                 riskFreeRate, maturity, volatility, timeSteps, gridPoints)
    class(val) <- c("AmericanOptionImpliedVolatility","ImpliedVolatility")
    val
}

BinaryOptionImpliedVolatility <- function(type, value, underlying, strike, dividendYield, riskFreeRate,
                                          maturity, volatility, cashPayoff=1) {
    UseMethod("BinaryOptionImpliedVolatility")
}

BinaryOptionImpliedVolatility.default <- function(type, value, underlying, strike, dividendYield, riskFreeRate,
                                                  maturity, volatility, cashPayoff=1) {
    val <- binaryOptionImpliedVolatilityEngine(type, value, underlying, strike, dividendYield,
                                               riskFreeRate, maturity, volatility, cashPayoff)
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
