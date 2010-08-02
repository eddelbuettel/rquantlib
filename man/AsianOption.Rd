% $Id$
\name{AsianOption}
\alias{AsianOption}
\alias{AsianOption.default}
\title{Asian Option evaluation using Closed-Form solution}
\description{
  The \code{AsianOption} function evaluates an Asian-style
  option on a common stock using an analytic solution for continuous
  geometric average price. The option value, the common first
  derivatives ("Greeks") as well as the calling parameters are returned.
}
\usage{
\method{AsianOption}{default}(averageType, type, underlying, strike,
	                  dividendYield, riskFreeRate, maturity, 
	                  volatility, first=0, length=0, fixings=0)
\method{plot}{Option}
\method{print}{Option}
\method{summary}{Option}
}
\arguments{
  \item{averageType}{Specifiy averaging type, either "geometric" or "arithmetic" }
  \item{type}{A string with one of the values \code{call} or \code{put}}
  \item{underlying}{Current price of the underlying stock}
  \item{strike}{Strike price of the option}
  \item{dividendYield}{Continuous dividend yield (as a fraction) of the stock}
  \item{riskFreeRate}{Risk-free rate}
  \item{maturity}{Time to maturity (in fractional years)}
  \item{volatility}{Volatility of the underlying stock}
  \item{first}{to be written}
  \item{length}{to be written}
  \item{fixings}{to be written}
}
\value{
  The \code{AsianOption} function returns an object of class
  \code{AsianOption} (which inherits from class 
  \code{\link{Option}}). It contains a list with the following
  components:
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
}
\details{
  
  When "arithmetic" evaluation is used, only the NPV() is returned.
  
  The well-known closed-form solution derived by Black, Scholes and
  Merton is used for valuation. Implied volatilities are calculated
  numerically.

  Please see any decent Finance textbook for background reading, and the
  \code{QuantLib} documentation for details on the \code{QuantLib}
  implementation.  
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
\examples{
# simple call with some explicit parameters, and slightly increased vol:
AsianOption("geometric", "put", underlying=80, strike=85, div=-0.03, riskFree=0.05, maturity=0.25, vol=0.2)
}
\keyword{misc}

