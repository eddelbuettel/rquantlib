% $Id: BinaryOptionImpliedVolatility.Rd,v 1.2 2006/11/11 04:12:02 edd Exp $
\name{BinaryOptionImpliedVolatility}
\alias{BinaryOptionImpliedVolatility}
\alias{BinaryOptionImpliedVolatility.default}
\title{Implied Volatility calculation for Binary Option}
\description{
  The \code{BinaryOptionImpliedVolatility} function solves for the
  (unobservable) implied volatility, given an option price as well as
  the other required parameters to value an option.}
\usage{
\method{BinaryOptionImpliedVolatility}{default}(type, value, underlying,
		strike, dividendYield, riskFreeRate, maturity, volatility,
		cashPayoff=1)
}
\arguments{
  \item{type}{A string with one of the values \code{call}, \code{put} or
    \code{straddle}}
  \item{value}{Value of the option (used only for ImpliedVolatility calculation)}
  \item{underlying}{Current price of the underlying stock}
  \item{strike}{Strike price of the option}
  \item{dividendYield}{Continuous dividend yield (as a fraction) of the stock}
  \item{riskFreeRate}{Risk-free rate}
  \item{maturity}{Time to maturity (in fractional years)}
  \item{volatility}{Initial guess for the volatility of the underlying
    stock}
  \item{cashPayoff}{Binary payout if options is exercised, default is 1}
}
\value{

  The \code{BinaryOptionImpliedVolatility} function returns an object
  of class \code{\link{ImpliedVolatility}}. It contains a list with the
  following elements:
  \item{impliedVol}{The volatility implied by the given market prices}
  \item{parameters}{List with the option parameters used}

}
\details{
  The Finite Differences method is used to value the Binary Option.
  Implied volatilities are then calculated numerically.

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
BinaryOptionImpliedVolatility("call", value=4.50, strike=100, 100, 0.02, 0.03, 0.5, 0.4, 10)
}
\keyword{misc}

