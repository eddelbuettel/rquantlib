% $Id: EuropeanOptionArrays.Rd,v 1.4 2004/12/28 03:20:07 edd Exp $
\name{EuropeanOptionArrays}
\alias{EuropeanOptionArrays}
\alias{oldEuropeanOptionArrays}
\alias{plotOptionSurface}
\title{European Option evaluation using Closed-Form solution}
\description{
  The \code{EuropeanOptionArrays} function allows any two of the numerical
  input parameters to be a vector, and a list of matrices is
  returned for the option value as well as each of the 'greeks'. For
  each of the returned matrices, each element 
  corresponds to an evaluation under the given set of parameters.
}
\usage{
EuropeanOptionArrays(type, underlying, strike, dividendYield,
                     riskFreeRate, maturity, volatility)
oldEuropeanOptionArrays(type, underlying, strike, dividendYield,
                        riskFreeRate, maturity, volatility)
plotOptionSurface(EOres, ylabel="", xlabel="", zlabel="", fov=60) 
}
\arguments{
  \item{type}{A string with one of the values \code{call} or \code{put}}
  \item{underlying}{(Scalar or list) current price(s) of the underlying stock}
  \item{strike}{(Scalar or list) strike price(s) of the option}
  \item{dividendYield}{(Scalar or list) continuous dividend yield(s) (as a fraction) of the stock}
  \item{riskFreeRate}{(Scalar or list) risk-free rate(s)}
  \item{maturity}{(Scalar or list) time(s) to maturity (in fractional years)}
  \item{volatility}{(Scalar or list) volatilit(y|ies) of the underlying stock}
  \item{EOres}{result matrix produced by \code{EuropeanOptionArrays}}
  \item{ylabel}{label for y-axsis}
  \item{xlabel}{label for x-axsis}
  \item{zlabel}{label for z-axsis}
  \item{fov}{viewpoint for 3d rendering}
}
\value{
  The \code{EuropeanOptionArrays} function allows any two of the numerical
  input parameters to be a vector or sequence. A list of
  two-dimensional matrices is returned. Each cell corresponds to
  an evaluation under the given set of parameters. 

  For these functions, the following components are returned:
  \item{value}{(matrix) value of option}
  \item{delta}{(matrix) change in value for a change in the underlying}
  \item{gamma}{(matrix) change in value for a change in delta}
  \item{vega}{(matrix) change in value for a change in the underlying's volatility}
  \item{theta}{(matrix) change in value for a change in delta}
  \item{rho}{(matrix) change in value for a change in time to maturity}
  \item{dividendRho}{(matrix) change in value for a change in delta}
  \item{parameters}{List with parameters with which object was created}

  The \code{oldEuropeanOptionArrays} function is an older implementation
  which vectorises this at the R level instead but allows more general
  multidimensional arrays.
  
}
\details{
  The well-known closed-form solution derived by Black, Scholes and
  Merton is used for valuation. 

  Please see any decent Finance textbook for background reading, and the
  \code{QuantLib} documentation for details on the \code{QuantLib}
  implementation.  
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
\seealso{\code{\link{AmericanOption}},\code{\link{BinaryOption}}}

\examples{
# define two vectos for the underlying and the volatility
und.seq <- seq(10,180,by=2)
vol.seq <- seq(0.1,0.9,by=0.1)
# evaluate them along with three scalar parameters
EOarr <- EuropeanOptionArrays("call", underlying=und.seq,
                              strike=100, dividendYield=0.01,
                              riskFreeRate=0.03,
                              maturity=1, volatility=vol.seq)
# and look at four of the result arrays: value, delta, gamma, vega
old.par <- par(no.readonly = TRUE)
par(mfrow=c(2,2),oma=c(5,0,0,0),mar=c(2,2,2,1))
plot(EOarr$parameters.underlying, EOarr$value[,1], type='n',
     main="option value", xlab="", ylab="") 
topocol <- topo.colors(length(vol.seq))
for (i in 1:length(vol.seq))
  lines(EOarr$parameters.underlying, EOarr$value[,i], col=topocol[i])
plot(EOarr$parameters.underlying, EOarr$delta[,1],type='n',
     main="option delta", xlab="", ylab="")
for (i in 1:length(vol.seq))
  lines(EOarr$parameters.underlying, EOarr$delta[,i], col=topocol[i])
plot(EOarr$parameters.underlying, EOarr$gamma[,1],type='n',
     main="option gamma", xlab="", ylab="")
for (i in 1:length(vol.seq))
  lines(EOarr$parameters.underlying, EOarr$gamma[,i], col=topocol[i])
plot(EOarr$parameters.underlying, EOarr$vega[,1],type='n',
     main="option vega", xlab="", ylab="")
for (i in 1:length(vol.seq))
  lines(EOarr$parameters.underlying, EOarr$vega[,i], col=topocol[i])
mtext(text=paste("Strike is 100, maturity 1 year, riskless rate 0.03",
        "\nUnderlying price from", und.seq[1],"to", und.seq[length(und.seq)],
        "\nVolatility  from",vol.seq[1], "to",vol.seq[length(vol.seq)]),
      side=1,font=1,outer=TRUE,line=3)
par(old.par)
}
\keyword{misc}

