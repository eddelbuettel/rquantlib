% $Id: EuropeanOptionImpliedVolatility.Rd,v 1.6 2006/11/11 04:13:25 edd Exp $
\name{EuropeanOptionImpliedVolatility}
\alias{EuropeanOptionImpliedVolatility}
\alias{EuropeanOptionImpliedVolatility.default}
\title{Implied Volatility calculation for European Option}
\description{
  The \code{EuropeanOptionImpliedVolatility} function solves for the
  (unobservable) implied volatility, given an option price as well as
  the other required parameters to value an option.}
\usage{
\method{EuropeanOptionImpliedVolatility}{default}(type, value,
	underlying, strike, dividendYield, riskFreeRate, maturity, volatility)

\method{print}{ImpliedVolatility}
\method{summary}{ImpliedVolatility}
}
\arguments{
  \item{type}{A string with one of the values \code{call} or \code{put}}
  \item{value}{Value of the option (used only for ImpliedVolatility calculation)}
  \item{underlying}{Current price of the underlying stock}
  \item{strike}{Strike price of the option}
  \item{dividendYield}{Continuous dividend yield (as a fraction) of the stock}
  \item{riskFreeRate}{Risk-free rate}
  \item{maturity}{Time to maturity (in fractional years)}
  \item{volatility}{Initial guess for the volatility of the underlying stock}
}
\value{

  The \code{EuropeanOptionImpliedVolatility} function returns an object
  of class \code{\link{ImpliedVolatility}}. It contains a list with the
  following elements:
  \item{impliedVol}{The volatility implied by the given market prices}
  \item{parameters}{List with the option parameters used}

}
\details{
  The well-known closed-form solution derived by Black, Scholes and
  Merton is used for valuation. Implied volatilities are then calculated
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
\seealso{\code{\link{EuropeanOption}},\code{\link{AmericanOption}},\code{\link{BinaryOption}}}
\examples{
EuropeanOptionImpliedVolatility(type="call", value=11.10, underlying=100,
	strike=100, dividendYield=0.01, riskFreeRate=0.03,
	maturity=0.5, volatility=0.4)
}
\keyword{misc}


