% $Id: AmericanOption.Rd,v 1.6 2006/11/11 04:08:06 edd Exp $
\name{AmericanOption}
\alias{AmericanOption}
\alias{AmericanOption.default}
\title{American Option evaluation using Finite Differences}
\description{
This function evaluations an American-style option on a common stock
using finite differences. The option value as well as the common first
derivatives ("Greeks") are returned.}
\usage{
\method{AmericanOption}{default}(type, underlying, strike,
		dividendYield, riskFreeRate, maturity, volatility, 
		timeSteps=150, gridPoints=149, engine="BaroneAdesiWhaley")
}
\arguments{
  \item{type}{A string with one of the values \code{call} or \code{put}}
  \item{underlying}{Current price of the underlying stock}
  \item{strike}{Strike price of the option}
  \item{dividendYield}{Continuous dividend yield (as a fraction) of the stock}
  \item{riskFreeRate}{Risk-free rate}
  \item{maturity}{Time to maturity (in fractional years)}
  \item{volatility}{Volatility of the underlying stock}
  \item{timeSteps}{Time steps for the \dQuote{CrankNicolson} finite
    differences method engine, default value is 150}
  \item{gridPoints}{Grid points for the \dQuote{CrankNicolson} finite differences method,
    default value is 149}
  \item{engine}{String selecting pricing engine, currently supported are
    \dQuote{BaroneAdesiWhaley} and \dQuote{CrankNicolson}}
}
\value{
  An object of class \code{AmericanOption} (which inherits from class
  \code{\link{Option}}) is returned. It contains a list with the
  following components: 
  \item{value}{Value of option}
  \item{delta}{Sensitivity of the option value for a change in the underlying}
  \item{gamma}{Sensitivity of the option delta for a change in the underlying}
  \item{vega}{Sensitivity of the option value for a change in the
    underlying's volatility} 
  \item{theta}{Sensitivity of the option value for a change in t, the
    remaining time to maturity}
  \item{rho}{Sensitivity of the option value for a change in the
    risk-free interest rate}
  \item{dividendRho}{Sensitivity of the option value for a change in the
    dividend yield}
  \item{parameters}{List with parameters with which object was created}

  Note that under the new pricing framework used in QuantLib,
  pricers do not provide analytics for all 'Greeks'. When
  \dQuote{CrankNicolson} is selected, then at least delta, gamma and
  vega are available. With the default pricing engine of
  \dQuote{BaroneAdesiWhaley}, no greeks are returned.
}
\details{
  The Finite Differences method is used to value the American Option.
  
  Please see any decent Finance textbook for background reading, and
  the \code{QuantLib} documentation for details on the \code{QuantLib}
  implementation. 
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
\seealso{\code{\link{EuropeanOption}}}

\examples{
# simple call with unnamed parameters
AmericanOption("call", 100, 100, 0.02, 0.03, 0.5, 0.4)
# simple call with some explicit parameters
AmericanOption("put", strike=100, volatility=0.4, 100, 0.02, 0.03, 0.5)
# simple call with unnamed parameters, using Crank-Nicolons
AmericanOption("put", strike=100, volatility=0.4, 100, 0.02, 0.03, 0.5, engine="CrankNicolson")
}
\keyword{misc}

