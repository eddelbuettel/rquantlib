% $Id$
\name{ZeroCouponBond}
\alias{ZeroCouponBond}
\alias{ZeroCouponBond.default}
\title{Zerocoupon bond pricing}
\description{
  The \code{ZeroCouponBond} function evaluates a zero-coupon plainly using discount curve. 
  More specificly, the calculation is done by DiscountingBondEngine from QuantLib.
  The NPV, clean price, dirty price, accrued interest, yield and cash flows of the bond is returned. 
  For more detail, see the source codes in quantlib's test-suite. test-suite/bond.cpp

}
\usage{
\method{ZeroCouponBond}{default}(bond, discountCurve, dateparams)
}
\arguments{

\item{bond}{bond parameters, a named list whose elements are: 
  \tabular{ll}{
    \code{issueDate}     \tab a Date, the bond's issue date\cr
    \code{maturityDate}  \tab a Date, the bond's maturity date\cr
    \code{faceAmount}    \tab (Optional) a double, face amount of the bond.\cr
    \code{}        \tab  Default value is 100. \cr
    \code{redemption}    \tab (Optional) a double, percentage of the initial \cr
    \code{}        \tab 	face amount that will be returned at maturity \cr
    \code{}        \tab    date. Default value is 100.\cr
  }
}
\item{discountCurve}{Can be one of the following:
  \tabular{ll}{
    \code{a DiscountCurve} \tab  a object of DiscountCurve class \cr
    \code{}                \tab For more detail, see example or \cr 
    \code{}                \tab the discountCurve function \cr
    \code{A 2 items list} \tab specifies a flat curve in two \cr 
    \code{}               \tab values "todayDate" and "rate"   \cr
    \code{A 3 items list} \tab specifies three values to construct a \cr
    \code{}                \tab DiscountCurve object, "params" , \cr 
    \code{} \tab "tsQuotes", "times". \cr
    \code{}                \tab For more detail, see example or \cr 
    \code{}                \tab the discountCurve function \cr
  }
}



\item{dateparams}{(Optional) a named list, QuantLib's date parameters of the bond. 
  \tabular{ll}{
    \code{settlementDays} \tab (Optional) a double, settlement days. \cr 
    \code{}        \tab Default value is 1.\cr
    \code{calendar} \tab (Optional) a string, either 'us' or 'uk' \cr 
    \code{}        \tab corresponding to US Goverment Bond \cr
    \code{}		   \tab calendar and UK Exchange calendar.\cr
    \code{}        \tab  Default value is 'us'.\cr
    \code{businessDayConvention} \tab (Optional) a number or string, \cr 
    \code{}                 \tab business day convention. \cr 
    \tab See \link{Enum}. Default value is 'Following'. \cr
  }
  See example below.
}
}
\value{
  The \code{ZeroCouponBond} function returns an object of class
  \code{ZeroCouponBond} (which inherits from class 
  \code{Bond}). It contains a list with the following
  components:
  \item{NPV}{net present value of the bond}
  \item{cleanPrice}{clean price of the bond}
  \item{dirtyPrice}{dirty price of the bond}
  \item{accruedAmount}{accrued amount of the bond}
  \item{yield}{yield of the bond}
  \item{cashFlows}{cash flows of the bond}
}
\details{
  A discount curve is built to calculate the bond value. 

  Please see any decent Finance textbook for background reading, and the
  \code{QuantLib} documentation for details on the \code{QuantLib}
  implementation.  
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}

\author{Khanh Nguyen \email{knguyen@cs.umb.edu} for the inplementation; Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}


\examples{
# Simple call with all parameter and a flat curve
bond <- list(faceAmount=100,issueDate=as.Date("2004-11-30"),
             maturityDate=as.Date("2008-11-30"), redemption=100 )

dateparams <-list(settlementDays=1, calendar="us", businessDayConvention='Unadjusted')


discountCurve.param <- list(tradeDate=as.Date('2002-2-15'),
                           settleDate=as.Date('2002-2-15'),
                           dt=0.25,
                           interpWhat='discount', interpHow='loglinear')
discountCurve.flat <- DiscountCurve(discountCurve.param, list(flat=0.05))

ZeroCouponBond(bond, discountCurve.flat, dateparams)


# The same bond with a discount curve constructed from market quotes
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
discountCurve <- DiscountCurve(discountCurve.param, tsQuotes)
ZeroCouponBond(bond, discountCurve, dateparams)


#examples with default arguments
ZeroCouponBond(bond, discountCurve)

bond <- list(issueDate=as.Date("2004-11-30"),
             maturityDate=as.Date("2008-11-30"))
dateparams <-list(settlementDays=1)
ZeroCouponBond(bond, discountCurve, dateparams)

}
\keyword{misc}

