\name{DoubleKnockOutOption}
\alias{DoubleKnockOutOption}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{Double Knock-Out Option
%%  ~~function to do ... ~~
}
\description{
The double Knock-out function evaluates a double barrier option with two dependant knock-out barriers using the Vanna-Volga method for pricing. If one of the barriers are reached, the option is killed.
}
\usage{
DoubleKnockOutOption(type, barrierUp, barrierDown, underlying, strike, dividendYield, 
riskFreeRate, maturity, vol25Put, volATM, vol25Call, volatility)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{type}{A string with one of the values \code{call} or \code{put}}
  \item{barrierUp}{
Price of the upside barrier
}
  \item{barrierDown}{
Price of the downside barrier
}
  \item{underlying}{Current price of the underlying stock}
  \item{strike}{Strike price of the option}
  \item{dividendYield}{Continuous dividend yield (as a fraction) of the stock. For FX, it is the interest rate of the base currency, eg. for EUR/USD, the EU interest rate.}
  \item{riskFreeRate}{Risk-free rate. For FX, it is the interest rate of the quote currency, eg. for EUR/USD, the US interest rate. }
  \item{maturity}{Time to maturity (in fractional years)}
  \item{vol25Put}{Volatility of the 25 delta put}
  \item{volATM}{Volatility at-the-money of the underlying}
  \item{vol25Call}{Volatility of the 25 delta call}
  \item{volatility}{Volatility of the underlying}
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

%% ~Make other sections like Warning with \section{Warning }{....} ~

\seealso{
%% ~~objects to See Also as \code{\link{help}}, ~~~
}
\examples{

spot=1.39078
barrierUp= 1.690215
barrierDown= 1.091345
volatility= 0.10765

DoubleKnockOutOption("call",barrierUp =barrierUp,  barrierDown =   barrierDown,
                      underlying = spot, strike = 1.091345, dividendYield=6e-04, 
                      riskFreeRate=0.01294,maturity=3/12,   vol25Put=0.111075,   
                      volATM=0.10765, vol25Call=0.109225, volatility=0.10765)
DoubleKnockOutOption("put",barrierUp = 1.5,  barrierDown =   1.1,underlying =   1.13321,
                      strike = 1.5, dividendYield=0.0003541, riskFreeRate=0.0033871,
                      maturity=3/12, vol25Put=0.10087, volATM=0.08925, vol25Call=0.08463, 
                      volatility=0.08925)
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{misc}
