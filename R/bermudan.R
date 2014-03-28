##  RQuantLib function BermudanSwaption
##
##  Copyright (C) 2005         Dominick Samperi
##  Copyright (C) 2007 - 2014  Dirk Eddelbuettel
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

BermudanSwaption <- function(params, tsQuotes, swaptionMaturities,
                             swapTenors, volMatrix) {
    UseMethod("BermudanSwaption")
}

BermudanSwaption.default <- function(params, tsQuotes, swaptionMaturities,
                                     swapTenors, volMatrix) {

  # Check that params list names
  if (!is.list(params) || length(params) == 0) {
      stop("The params parameter must be a non-empty list", call.=FALSE)
  }

  # Check that the term structure quotes are properly formatted.
  if (!is.list(tsQuotes) || length(tsQuotes) == 0) {
    stop("Term structure quotes must be a non-empty list", call.=FALSE)
  }
  if (length(tsQuotes) != length(names(tsQuotes))) {
    stop("Term structure quotes must include labels", call.=FALSE)
  }
  if (!is.numeric(unlist(tsQuotes))) {
    stop("Term structure quotes must have numeric values", call.=FALSE)
  }

  # Check for correct matrix/vector types
  if (!is.matrix(volMatrix)
      || !is.vector(swaptionMaturities)
      || !is.vector(swapTenors)) {
    stop("Swaption vol must be a matrix, maturities/tenors must be vectors",
         call.=FALSE)
  }

  # Check that matrix/vectors have compatible dimensions
  if (prod(dim(volMatrix)) != length(swaptionMaturities)*length(swapTenors)) {
    stop("Dimensions of swaption vol matrix not compatible with maturity/tenor vectors",
         call.=FALSE)
  }

  # Finally ready to make the call...
  # We could coerce types here and pass as.integer(round(swapTenors)),
  # temp <- as.double(volMatrix), dim(temp) < dim(a) [and pass temp instead
  # of volMatrix]. But this is taken care of in the C/C++ code.
  val <- bermudanSwaptionEngine(params, tsQuotes,
                                swaptionMaturities,
                                swapTenors, volMatrix)
  class(val) <- c(params$method, "BermudanSwaption")
  val
}

summary.G2Analytic <- function(object,...) {
  cat('\n\tSummary of pricing results for Bermudan Swaption\n')
  cat('\nPrice (in bp) of Bermudan swaption is ', object$price)
  cat('\nStike is ', format(object$params$strike,digits=6))
  cat(' (ATM strike is ', format(object$ATMStrike,digits=6), ')')
  cat('\nModel used is: G2/Jamshidian using analytic formulas')
  cat('\nCalibrated model parameters are:')
  cat('\na = ', format(object$a,digits=4))
  cat('\nb = ', format(object$b,digits=4))
  cat('\nsigma = ', format(object$sigma,digits=4))
  cat('\neta = ', format(object$eta,digits=4))
  cat('\nrho = ', format(object$rho,digits=4))
  cat('\n\n')
}

summary.HWAnalytic <- function(object,...) {
  cat('\n\tSummary of pricing results for Bermudan Swaption\n')
  cat('\nPrice (in bp) of Bermudan swaption is ', object$price)
  cat('\nStike is ', format(object$params$strike,digits=6))
  cat(' (ATM strike is ', format(object$ATMStrike,digits=6), ')')
  cat('\nModel used is: Hull-White using analytic formulas')
  cat('\nCalibrated model parameters are:')
  cat('\na = ', format(object$a,digits=4))
  cat('\nsigma = ', format(object$sigma,digits=4))
  cat('\n\n')
}

summary.HWTree <- function(object,...) {
  cat('\n\tSummary of pricing results for Bermudan Swaption\n')
  cat('\nPrice (in bp) of Bermudan swaption is ', object$price)
  cat('\nStike is ', format(object$params$strike,digits=6))
  cat(' (ATM strike is ', format(object$ATMStrike,digits=6), ')')
  cat('\nModel used is: Hull-White using a tree')
  cat('\nCalibrated model parameters are:')
  cat('\na = ', format(object$a,digits=4))
  cat('\nsigma = ', format(object$sigma,digits=4))
  cat('\n\n')
}

summary.BKTree <- function(object,...) {
  cat('\n\tSummary of pricing results for Bermudan Swaption\n')
  cat('\nPrice (in bp) of Bermudan swaption is ', object$price)
  cat('\nStike is ', format(object$params$strike,digits=6))
  cat(' (ATM strike is ', format(object$ATMStrike,digits=6), ')')
  cat('\nModel used is: Black-Karasinski using a tree')
  cat('\nCalibrated model parameters are:')
  cat('\na = ', format(object$a,digits=4))
  cat('\nsigma = ', format(object$sigma,digits=4))
  cat('\n\n')
}
