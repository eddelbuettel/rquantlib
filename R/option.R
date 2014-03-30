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

EuropeanOption <- function(type, underlying, strike, dividendYield,
                           riskFreeRate, maturity, volatility) {
    UseMethod("EuropeanOption")
}

EuropeanOption.default <- function(type, underlying, strike, dividendYield,
                                   riskFreeRate, maturity, volatility) {
    type <- match.arg(type, c("call", "put"))
    val <- europeanOptionEngine(type, underlying, strike, dividendYield,
                                riskFreeRate, maturity, volatility)
    class(val) <- c("EuropeanOption", "Option")
    val
}

AmericanOption <- function(type, underlying, strike, dividendYield,
                           riskFreeRate, maturity, volatility,
                           timeSteps=150, gridPoints=149,
                           engine="BaroneAdesiWhaley") {
    UseMethod("AmericanOption")
}

AmericanOption.default <- function(type, underlying, strike, dividendYield,
                                   riskFreeRate, maturity, volatility,
                                   timeSteps=150, gridPoints=149,
                                   engine="BaroneAdesiWhaley") {
    type <- match.arg(type, c("call", "put"))
    engine <- match.arg(engine, c("BaroneAdesiWhaley", "CrankNicolson"))
    val <- americanOptionEngine(type, underlying, strike, dividendYield,
                                riskFreeRate, maturity, volatility,
                                timeSteps, gridPoints, engine)
    class(val) <- c("AmericanOption","Option")
    val
}

BinaryOption <- function(binType, type, excType, underlying, strike, dividendYield,
                         riskFreeRate, maturity, volatility,
                         cashPayoff) {
    UseMethod("BinaryOption")
}

BinaryOption.default <- function(binType, type, excType, underlying, strike, dividendYield,
                                 riskFreeRate, maturity, volatility,
                                 cashPayoff) {
    type <- match.arg(type, c("call", "put"))
    binType <- match.arg(binType, c("cash", "asset", "gap"))
    excType <- match.arg(excType, c("american", "european"))
    val <- binaryOptionEngine(binType, type, excType, underlying,
                              strike, dividendYield, riskFreeRate,
                              maturity, volatility, cashPayoff) 
    class(val) <- c("BinaryOption", "Option")
    val
}

BarrierOption <- function(barrType, type, underlying, strike,
                          dividendYield, riskFreeRate, maturity,
                          volatility, barrier, rebate=0.0) {
    UseMethod("BarrierOption")
}

BarrierOption.default <- function(barrType, type, underlying, strike,
                                  dividendYield, riskFreeRate, maturity,
                                  volatility, barrier, rebate=0.0) {
    type <- match.arg(type, c("call", "put"))
    barrType <- match.arg(barrType, c("downin", "upin", "downout", "upout"))
    val <- barrierOptionEngine(barrType, type, underlying, strike, dividendYield,
                               riskFreeRate, maturity, volatility, barrier, rebate)
    class(val) <- c("BarrierOption", "Option")
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




