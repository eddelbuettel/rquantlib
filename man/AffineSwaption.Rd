\name{AffineSwaption}
\alias{AffineSwaption}
\alias{AffineSwaption.default}
\alias{summary.G2AnalyticAffineSwaption}
\alias{summary.HWAnalyticAffineSwaption}
\alias{summary.HWTreeAffineSwaption}
\alias{summary.BKTreeAffineSwaption}
\title{Affine swaption valuation using several short-rate models}
\description{
  \code{AffineSwaption} prices a swaption with specified
  strike and maturity (in years), after calibrating the selected
  affine short-rate model to an input swaption volatility matrix. Swaption
  maturities are in years down the rows, and swap tenors are in years
  along the columns, in the usual fashion. It is assumed that the
  swaption is
  exercisable  at the start of the swap if {params$european} flag is set to {TRUE} or  
  on each reset date (Bermudan) of the underlying swap if {params$european} flag is 
  set to {FALSE}.
}
\usage{
AffineSwaption(params, ts, swaptionMaturities, swapTenors,
volMatrix,legparams)
}
\arguments{
  \item{params}{A list specifying the \code{tradeDate} (month/day/year),
    \code{settlementDate}, logical flags \code{payFixed} & \code{european}
    (european=FALSE generates Bermudan vlaue), \code{strike}, pricing \code{method}, and curve construction options
    (see \emph{Examples} section below). Curve construction options are
    \code{interpWhat} (possible values are \code{discount},
    \code{forward}, and \code{zero}) and
    \code{interpHow} (possible values are \code{linear},
    \code{loglinear}
    , and \code{spline}). Both \code{interpWhat} and \code{interpHow}
    are ignored when a flat yield curve is requested, but they must be
    present nevertheless.
    The pricing method can be one of the following (all short-rate models):
    \tabular{ll}{
      \code{G2Analytic} \tab G2 2-factor Gaussian model using analytic formulas.\cr
      \code{HWAnalytic} \tab Hull-White model using analytic formulas.\cr
      \code{HWTree} \tab Hull-White model using a tree.\cr
      \code{BKTree} \tab Black-Karasinski model using a tree.
      }
    }
  \item{ts}{A term structure built with DiscountCurve is required. See the
    help page for \code{\link{DiscountCurve}} and example below for details.}
  \item{swaptionMaturities}{A vector containing the swaption maturities
    associated with the rows of the swaption volatility matrix.}
  \item{swapTenors}{A vector containing the underlying swap tenors
    associated with the columns of the swaption volatility matrix.}
  \item{volMatrix}{The swaption volatility matrix. Must be a 2D matrix
  stored by rows. See the example below.}
    \item{legparams}{A list specifying the \code{dayCounter} the day count convention for the 
   fixed leg (default is Thirty360), and  \code{fixFreq}, fixed coupon frequecny (defualt is Annual), \code{floatFreq}, 
   floating leg reset frequency (default is Semiannual).
  }
 }
\value{
  \code{AffineSwaption} returns a list containing calibrated model
  paramters (what parameters are returned depends on the model
  selected) along with:
  \item{NPV}{NPV of swaption in basis points (actual price
  equals \code{price} times notional divided by 10,000)}
  \item{ATMStrike}{At-the-money strike}
  \item{params}{Input parameter list}
}
\details{
  This function is based on \code{QuantLib} Version 0.3.10. It
  introduces support for fixed-income instruments in \code{RQuantLib}.
  
  At present only a small number of the many parameters that can be set
  in \code{QuantLib} are exposed by this function. Some of the
  hard-coded parameters that apply to the current version include:
  day-count conventions, fixing days (2), index (Euribor),
  fixed leg frequency (annual), and floating leg frequency
  (semi-annual). Also, it is assumed that the swaption
  volatility matrix corresponds to expiration dates and tenors that are
  measured in years (a 6-month expiration date is not currently
  supported, for example).

  Given the number of parameters that must be
  specified and the care with which they must be specified (with no
  defaults), it is not practical to use this function in the usual
  interactive fashion.

  The simplest approach is simply to save the
  example below to a file, edit as desired, and \code{source} the result.
  Alternatively, the input commands can be kept in a script file
  (under Windows) or an Emacs/ESS session (under Linux), and selected
  parts of the script can be executed in the usual way.

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
\author{Terry Leitch}

\seealso{\code{\link{DiscountCurve}}}

\examples{
if (.Platform$OS.type != "windows" && .Platform$r_arch != "i386") {
    
# This data was generated to match the original quantlib example for Bermudan Swaption
params <- list(tradeDate=as.Date('2016-2-15'),
               settleDate=as.Date('2016-2-17'),
               startDate=as.Date('2017-2-17'),
               maturity=as.Date('2022-2-17'),
               payFixed=TRUE,
               european=FALSE,
               dt=.25,
               strike=.06,
               method="G2Analytic",
               interpWhat="discount",
               interpHow="loglinear")

# Market data used to construct the term structure of interest rates
tsQuotes <- list(d1w  =0.0382,
                 d1m  =0.0372,
                 fut1=96.2875,
                 fut2=96.7875,
                 fut3=96.9875,
                 fut4=96.6875,
                 fut5=96.4875,
                 fut6=96.3875,
                 fut7=96.2875,
                 fut8=96.0875,
                 s3y  =0.0398,
                 s5y  =0.0443,
                 s10y =0.05165,
                 s15y =0.055175)


# Swaption volatility matrix with corresponding maturities and tenors
swaptionMaturities <- c(1,2,3,4,5)

swapTenors <- c(1,2,3,4,5)

volMatrix <- matrix(
    c(0.1490, 0.1340, 0.1228, 0.1189, 0.1148,
      0.1290, 0.1201, 0.1146, 0.1108, 0.1040,
      0.1149, 0.1112, 0.1070, 0.1010, 0.0957,
      0.1047, 0.1021, 0.0980, 0.0951, 0.1270,
      0.1000, 0.0950, 0.0900, 0.1230, 0.1160),
    ncol=5, byrow=TRUE)

legparams=list(dayCounter="Thirty360",
               fixFreq="Annual",
               floatFreq="Semiannual")

setEvaluationDate(as.Date("2016-2-16"))               
times<-times <- seq(0,14.75,.25)
dcurve <- DiscountCurve(params, tsQuotes, times=times,legparams)

# Price the Bermudan swaption
pricing <- AffineSwaption(params, dcurve,swaptionMaturities, swapTenors, volMatrix,legparams)
summary(pricing)

}    
}
\keyword{models}
