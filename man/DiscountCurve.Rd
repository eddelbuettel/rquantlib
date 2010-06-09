% $Id: DiscountCurve.Rd,v 1.5 2007/12/31 02:12:27 edd Exp $
\name{DiscountCurve}
\alias{DiscountCurve}
\alias{DiscountCurve.default}
\alias{plot.DiscountCurve}
\title{Returns the discount curve (with zero rates and forwards) given times}
\description{
  \code{DiscountCurve} constructs the spot term structure of interest
  rates based on input market data including the settlement date,
  deposit rates, futures prices, FRA rates, or swap rates, in various
  combinations. It returns the corresponding discount factors, zero
  rates, and forward rates for a vector of times that is specified
  as input.
}
\usage{
DiscountCurve(params, tsQuotes, times)
}
\arguments{
  \item{params}{A list specifying the \code{tradeDate} (month/day/year),
    \code{settleDate}, forward rate
    time span \code{dt}, and two curve
    construction options: \code{interpWhat} (with possible values
    \code{discount}, \code{forward}, and \code{zero}) and
    \code{interpHow} (with possible values \code{linear},
    \code{loglinear}, and \code{spline}). \code{spline} here means cubic spline
    interpolation of the \code{interpWhat} value.
  }
  \item{tsQuotes}{Market quotes used to construct the spot term
    structure of interest rates. Must be a list of name/value pairs,
    where the currently recognized names are:
    \tabular{ll}{
      \code{flat} \tab rate for a flat yield curve\cr
      \code{d1w}  \tab 1-week deposit rate\cr
      \code{d1m}  \tab 1-month deposit rate\cr
      \code{d3m}  \tab 3-month deposit rate\cr
      \code{d6m}  \tab 6-month deposit rate\cr
      \code{d9m}  \tab 9-month deposit rate\cr
      \code{d1y}  \tab 1-year deposit rate\cr
      \code{s2y}  \tab 2-year swap rate\cr
      \code{s3y}  \tab 3-year swap rate\cr
      \code{s5y}  \tab 5-year swap rate\cr
      \code{s10y} \tab 10-year swap rate\cr
      \code{s15y} \tab 15-year swap rate\cr
      \code{s20y} \tab 20-year swap rate\cr
      \code{s30y} \tab 30-year swap rate\cr
      \code{fut1}--\code{fut8} \tab 3-month futures contracts\cr
      \code{fra3x6} \tab 3x6 FRA\cr
      \code{fra6x9} \tab 6x9 FRA\cr
      \code{fra6x12}\tab 6x12 FRA
    }
    Here rates are expected as fractions (so 5\% means .05).
    If \code{flat} is specified it must be the first and only item in
    the list. The eight futures correspond to the first eight IMM
    dates. The maturity dates of the instruments specified need not be
    ordered, but they must be distinct.
    }
  \item{times}{A vector of times at which to return the discount
    factors, forward rates, and zero rates. Times must be specified such
    that the largest time plus \code{dt} does not exceed the longest
    maturity of the instruments used for calibration (no extrapolation).
    }
}
\value{
  \code{DiscountCurve} returns a list containing:
  \item{times}{Vector of input times}
  \item{discounts}{Corresponding discount factors}
  \item{forwards}{Corresponding forward rates with time span \code{dt}}
  \item{zerorates}{Corresponding zero coupon rates}
  \item{flatQuotes}{True if a flat quote was used, False otherwise}
  \item{params}{The input parameter list}
}
\details{
  This function is based on \code{QuantLib} Version 0.3.10. It
  introduces support for fixed-income instruments in \code{RQuantLib}.
  
  Forward rates and zero rates are computed assuming continuous
  compounding, so the forward rate \eqn{f} over the
  period from \eqn{t_1}{t1} to \eqn{t_2}{t2} is determined by the
  relation
  \deqn{d_1/d_2 = e^{f (t_2 - t_1)},}{d1/d2 = exp(f(t2 - t1)),}
  where \eqn{d_1}{d1} and \eqn{d_2}{d2} are discount factors
  corresponding to the two times. In the case of the zero rate
  \eqn{t_1}{t1} is the current time (the spot date).

  Curve construction can be a delicate problem and the algorithms may
  fail for some input data sets and/or some combinations of the
  values for \code{interpWhat} and \code{interpHow}.
  Fortunately, the C++ exception mechanism seems to work well with the R
  interface, and \code{QuantLib} exceptions are propagated back to the
  R user, usually with a message that indicates what went wrong. (The
  first part of the message contains technical information about the
  precise location of the problem in the \code{QuantLib} code. Scroll to
  the end to find information that is meaningful to the R user.)
  
}
\references{
  Brigo, D. and Mercurio, F. (2001) \emph{Interest Rate Models: Theory and
    Practice}, Springer-Verlag, New York.

  For information about \code{QuantLib} see \url{http://quantlib.org}.

  For information about \code{RQuantLib} see
\url{http://dirk.eddelbuettel.com/code/rquantlib.html}.

}
\author{Dominick Samperi}

\seealso{\code{\link{BermudanSwaption}}}

\examples{

savepar <- par(mfrow=c(3,3), mar=c(4,4,2,0.5))

## This data is taken from sample code shipped with QuantLib 0.9.7
## from the file Examples/Swap/swapvaluation
params <- list(tradeDate=as.Date('2004-09-20'),
               settleDate=as.Date('2004-09-22'),
               dt=.25,
               interpWhat="discount",
               interpHow="loglinear")

## We get numerical issue for the spline interpolation if we add
## any on of these three extra futures -- the original example
## creates different curves based on different deposit, fra, futures
## and swap data
tsQuotes <- list(d1w = 0.0382,
                 d1m = 0.0372,
                 d3m = 0.0363,
                 d6m = 0.0353,
                 d9m = 0.0348,
                 d1y = 0.0345,
#                 fut1=96.2875,
#                 fut2=96.7875,
#                 fut3=96.9875,
#                 fut4=96.6875,
#                 fut5=96.4875,
#                 fut6=96.3875,
#                 fut7=96.2875,
#                 fut8=96.0875,
                 s2y = 0.037125,
                 s3y = 0.0398,
                 s5y = 0.0443,
                 s10y = 0.05165,
                 s15y = 0.055175)

times <- seq(0,10,.1)

# Loglinear interpolation of discount factors
curves <- DiscountCurve(params, tsQuotes, times)
plot(curves,setpar=FALSE)

# Linear interpolation of discount factors
params$interpHow="linear"
curves <- DiscountCurve(params, tsQuotes, times)
plot(curves,setpar=FALSE)

# Spline interpolation of discount factors
params$interpHow="spline"
curves <- DiscountCurve(params, tsQuotes, times)
plot(curves,setpar=FALSE)

par(savepar)

}
\keyword{models}
