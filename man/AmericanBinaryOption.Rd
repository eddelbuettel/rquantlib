\name{AmericanBinaryOption}
\alias{AmericanBinaryOption}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
American Binary Option
}
\description{
This function evaluations an American Binary option (also called One Touch) using a volatility surface.
}
\usage{
AmericanBinaryOption(type, underlying, strike, dividendYield, riskFreeRate, maturity, 
vol, cashPayoff, volStrikes, volMaturities, volMatrix)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{type}{A string with one of the values \code{call} or \code{put}}
  \item{underlying}{Current price of the underlying stock}
  \item{strike}{Strike price of the option}
  \item{dividendYield}{Continuous dividend yield (as a fraction) of the stock. For FX, it is the interest rate of the base currency, eg. for EUR/USD, the EU interest rate.}
  \item{riskFreeRate}{Risk-free rate. For FX, it is the interest rate of the quote currency, eg. for EUR/USD, the US interest rate. }
  \item{maturity}{Time to maturity (in fractional years)}

 \item{vol}{Volatility of the underlying}
  \item{cashPayoff}{Payout amount}
  \item{volStrikes}{A vector containing the volatility strikes
    associated with the column of the volatility matrix.}
  \item{volMaturities}{A vector containing the volatility maturities (in days)
    associated with the rows of the volatility matrix.
}
  \item{volMatrix}{The volatility matrix representing the volatility surface associated with the underlying. Must be a 2D matrix
  stored by rows. See the example below.
}
}

\value{
  \item{value}{Value of option}
  \item{delta}{Sensitivity of the option value for a change in the underlying}
  \item{gamma}{Sensitivity of the option delta for a change in the underlying}
}
\details{
  A closed-form solution is used to value the Binary Option.

  Please see any decent Finance textbook for background reading, and the
  \code{QuantLib} documentation for details on the \code{QuantLib}
  implementation.  
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{
  Marie Redron \email{redronm@gmail.com} for the implementation;
  Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  the QuantLib Group for \code{QuantLib}
}

\examples{

strike=100
spot=95
dividendYield=0.20
riskFreeRate=0.10
t=0.5  
vol=0.2
cashPayoff=15
volStrikes = c(90,95,100)
volMaturities=c(30,90)
volMatrix=matrix(c(0.12,0.1,0.13,
                   0.13,0.1,0.12),
                   3,2)

AmericanBinaryOption(type="call", strike=strike, underlying=spot, dividendYield=dividendYield, 
                    riskFreeRate=riskFreeRate, maturity=t, vol=vol,cashPayoff=15,
                   volStrikes = volStrikes,volMaturities=volMaturities,volMatrix=volMatrix)

}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{misc}
