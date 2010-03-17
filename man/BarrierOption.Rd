% $Id: BarrierOption.Rd,v 1.7 2007/12/31 01:50:55 edd Exp $
\name{BarrierOption}
\alias{BarrierOption}
\alias{BarrierOption.default}
\title{Barrier Option evaluation using Closed-Form solution}
\description{
This function evaluations an Barrier option on a common stock
using a closed-form solution. The option value as well as the common first
derivatives ("Greeks") are returned.}
\usage{
\method{BarrierOption}{default}(barrType, type, underlying, strike, 
                                dividendYield, riskFreeRate, maturity, 
                                volatility, barrier, rebate=0.0)
\method{print}{Option}
\method{summary}{Option}
}
\arguments{
  \item{barrType}{A string with one of the values \code{downin},
    \code{downout}, \code{upin} or \code{upout}}
  \item{type}{A string with one of the values \code{call} or \code{put}}
  \item{underlying}{Current price of the underlying stock}
  \item{strike}{Strike price of the option}
  \item{dividendYield}{Continuous dividend yield (as a fraction) of the stock}
  \item{riskFreeRate}{Risk-free rate}
  \item{maturity}{Time to maturity (in fractional years)}
  \item{volatility}{Volatility of the underlying stock}
  \item{barrier}{Option barrier value}
  \item{rebate}{Optional option rebate, defaults to 0.0}
}
\value{
  An object of class \code{BarrierOption} (which inherits from class
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
  \item{parameters}{List with parameters with which object was created}.

  Note that under the new pricing framework used in QuantLib, binary
  pricers do not provide analytics for 'Greeks'. This is expected to be
  addressed in future releases of QuantLib.
}
\details{
  A closed-form solution is used to value the Barrier Option. In the
  case of Barrier options, the calculations are from Haug's "Option
  pricing formulas" book (McGraw-Hill).
  
  Please see any decent Finance textbook for background reading, and
  the \code{QuantLib} documentation for details on the \code{QuantLib}
  implementation.
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
\seealso{\code{\link{AmericanOption}},\code{\link{EuropeanOption}}}

\examples{
BarrierOption(barrType="downin", type="call", underlying=100,
	strike=100, dividendYield=0.02, riskFreeRate=0.03,
	maturity=0.5, volatility=0.4, barrier=90)
}
\keyword{misc}

