\name{DoubleNoTouchOption}
\alias{DoubleNoTouchOption}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
Double No-Touch Option
}
\description{
The double No-Touch evaluates a double binary barrier option with two dependant barriers using the Vanna-Volga method for pricing. Payout is the underlying.
}
\usage{
DoubleNoTouchOption(barrierUp, barrierDown, underlying, dividendYield, riskFreeRate, 
maturity, volatility, BF25D, RR25D)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{barrierUp}{
Price of upside barrier
}
  \item{barrierDown}{
Price of downside barrier
}
  \item{underlying}{Current price of the underlying stock}
  \item{dividendYield}{Continuous dividend yield (as a fraction) of the stock. For FX, it is the interest rate of the base currency, eg. for EUR/USD, the EU interest rate.}
  \item{riskFreeRate}{Risk-free rate. For FX, it is the interest rate of the quote currency, eg. for EUR/USD, the US interest rate. }
  \item{maturity}{Time to maturity (in fractional years)}
  \item{volatility}{Volatility of the underlying}
  \item{BF25D}{Current price of the 25 delta Butterfly}
  \item{RR25D}{Current price of the 25 delta Risk Reversal}
}
\value{
 \item{value}{Value of option}
}
\details{
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

spot=1.39078
barrierUp= 1.690215
barrierDown= 1.091345
volatility= 0.10765

DoubleNoTouchOption(barrierUp =barrierUp,  barrierDown =  barrierDown,
                      underlying = spot, dividendYield=6e-04, 
                      riskFreeRate=0.01294,maturity=3/12, volatility=0.10765,
                      BF25D=0.10765, RR25D=0.109225)
 }                     
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{misc}
