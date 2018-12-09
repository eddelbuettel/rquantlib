\name{SabrSwaption}
\alias{SabrSwaption}
\alias{SabrSwaption.default}
\title{SABR swaption using vol cube data with bermudan alternative using markovfunctional}
\description{
  \code{SabrSwaption} prices a swaption with specified
  expiration or time range if Bermudan, strike, and maturity, using quantlibs SABR model for europeans 
  and quantlib's markovfunctional for Bermudans.  Currently the input is a zero offset log-normal vol surface.
  An example of a dataset can be found in the dataset {rqlib} inlcuded with Rquantlib. It is assumed that the
  swaption is
  exercisable  at the start of a forward start swap if {params$european} flag is set to {TRUE} or starting
  immediately on each reset date (Bermudan) of an existing underlying swap or spot start 
  swap if {params$european} flag is set to {FALSE}.
}
\usage{
SabrSwaption(params, ts, volCubeDF, 
legparams = list(dayCounter = "Thirty360", fixFreq = "Annual", floatFreq = "Semiannual"), 
tsUp01 = NA, tsDn01 = NA, vega = FALSE)}
\arguments{
  \item{params}{A list specifying the \code{tradeDate} (month/day/year),
    \code{settlementDate}, logical flags \code{payFixed} & \code{european}
    (european=FALSE generates Bermudan vlaue), \code{strike}, pricing \code{method}, and curve 
    construction options (see \emph{Examples} section below). Curve construction options are
    \code{interpWhat} (possible values are \code{discount},
    \code{forward}, and \code{zero}) and
    \code{interpHow} (possible values are \code{linear},
    \code{loglinear}
    , and \code{spline}). Both \code{interpWhat} and \code{interpHow}
    are ignored when a flat yield curve is requested, but they must be
    present nevertheless.
    }
  \item{ts}{A term structure built with DiscountCurve is required. See the
    help page for \code{\link{DiscountCurve}} and example below for details.}
  \item{volCubeDF}{The swaption volatility cube in dataframe format with columns Expiry, Tenor, Spread, 
  and LogNormalVol stored by rows. See the example below.}
    \item{legparams}{A list specifying the \code{dayCounter} the day count convention for the 
   fixed leg (default is Thirty360), and  \code{fixFreq}, fixed coupon frequecny (defualt is Annual), 
   \code{floatFreq}, floating leg reset frequency (default is Semiannual).
  }
    \item{tsUp01}{Discount for a user specied up move in rates.}
    \item{tsDn01}{Discount for a user specied down move in rates.}
    \item{vega}{Discount for a user specied up move.}
 }
\value{
  \code{SabrSwaption} returns a list containing the value of the payer and receiver swaptions at the 
  strike specified in \code{params}. 
  \item{NPV}{NPV of swaption in basis points (actual price
  equals \code{price} times notional divided by 10,000)}
  \item{strike}{swaption strike}
  \item{params}{Input parameter list}
  \item{atmRate}{fair rate for swap at swap start date for european or fair swap rate 
  for swap at expiration for bermudan}
  \item{vol}{vol for swaption at swap start date and rate strike for european or vol for swaption 
  for given expiration and strike for bermudan}
  \item{rcvDv01}{reveiver value for a change in rates defined by dv01Up}
  \item{payDv01}{payer value for a change in rates defined by dv01Up}
  \item{rcvCnvx}{reveiver second order value change for a change in rates defined by dv01Up & dv01Dn}
  \item{payCnvx}{payer second order value for a change in rates defined by dv01Up & dv01Dn}  
  \item{strike}{swaption strike}
  
}
\details{
  This function is based on \code{QuantLib} Version 1.64. It
  introduces support for fixed-income instruments in \code{RQuantLib}.
  

}
\references{
  Brigo, D. and Mercurio, F. (2006) \emph{Interest Rate Models: Theory and
    Practice, 2nd Edition}, Springer-Verlag, New York.

  For information about \code{QuantLib} see \url{http://quantlib.org}.

  For information about \code{RQuantLib} see
\url{http://dirk.eddelbuettel.com/code/rquantlib.html}.

}
\author{Terry Leitch}

\seealso{\code{\link{AffineSwaption}}}

\examples{

params <- list(tradeDate=as.Date('2016-2-15'),
               settleDate=as.Date('2016-2-17'),
               startDate=as.Date('2017-2-17'),
               maturity=as.Date('2022-2-17'),
               european=TRUE,
               dt=.25,
               expiryDate=as.Date('2017-2-17'),
               strike=.02,
               interpWhat="discount",
               interpHow="loglinear")

# Set leg paramters for generating discount curve
dclegparams=list(dayCounter="Thirty360",
                 fixFreq="Annual",
                 floatFreq="Semiannual")

setEvaluationDate(as.Date("2016-2-16"))               
times<-times <- seq(0,14.75,.25)

data(tsQuotes)
dcurve <- DiscountCurve(params, tsQuotes, times=times,dclegparams)

# Price the Bermudan swaption
swaplegparams=list(fixFreq="Semiannual",floatFreq="Quarterly")

data(vcube)
pricing <- SabrSwaption(params, dcurve,vcube,swaplegparams)
pricing

}
\keyword{models}
