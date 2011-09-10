% $Id$
\name{FixedRateBond}
\alias{FixedRateBond}
\alias{FixedRateBond.default}
\alias{FixedRateBondPriceByYield}
\alias{FixedRateBondPriceByYield.default}
\alias{FixedRateBondYield}
\alias{FixedRateBondYield.default}
\title{Fixed-Rate bond pricing}
\description{
  The \code{FixedRateBond} function evaluates a fixed rate bond using discount curve. 
  More specificly, the calculation is done by DiscountingBondEngine from QuantLib.
  The NPV, clean price, dirty price, accrued interest, yield and cash flows of the bond is returned. 
  For more detail, see the source codes in QuantLib's file \code{test-suite/bond.cpp}.

  The \code{FixedRateBondPriceByYield} function calculates the theoretical price of a fixed rate bond from its yield.

  The \code{FixedRateBondYield} function calculates the theoretical yield of a fixed rate bond from its price.
}
\usage{
\method{FixedRateBond}{default}(bond, rates, discountCurve, dateparams )

\method{FixedRateBondPriceByYield}{default}( settlementDays=1, yield, faceAmount, 
                                 effectiveDate, maturityDate,
                                 period, calendar="us", 
                                 rates, dayCounter=2,
                                 businessDayConvention=0, compound = 0, redemption=100, 
                                 issueDate)

\method{FixedRateBondYield}{default}( settlementDays=1, price, faceAmount, 
                                 effectiveDate, maturityDate,
                                 period, calendar="us", 
                                 rates, dayCounter=2,
                                 businessDayConvention=0, 
                                 compound = 0, redemption=100, 
                                 issueDate)
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
    \code{effectiveDate}  \tab (Optinal) a Date, the bond's effective date. Default value is issueDate\cr
  }
}

\item{rates}{a numeric vector, bond's coupon rates}

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
    \code{dayCounter} \tab (Optional) a number or string, \cr 
    \code{}        \tab day counter convention.\cr
    \code{}        \tab  See \link{Enum}. Default value is 'Thirty360' \cr 
    \code{period}  \tab (Optional) a number or string, \cr
    \code{}       \tab  interest compounding interval. See \link{Enum}. \cr
    \code{}       \tab Default value is 'Semiannual'.\cr
    \code{businessDayConvention} \tab (Optional) a number or string, \cr 
    \code{}                 \tab business day convention. \cr 
    \tab See \link{Enum}. Default value is 'Following'. \cr
    \code{terminationDateConvention} \tab (Optional) a number or string, \cr 
    \code{}                 \tab termination day convention. \cr 
    \tab See \link{Enum}. Default value is 'Following'. \cr
    \code{endOfMonth} \tab (Optional) a numeric with value 1 or 0. \cr 
    \code{}           \tab End of Month rule. Default value is 0.\cr
    \code{dateGeneration} \tab (Optional) a numeric, date generation method. \cr 
    \code{} \tab See \link{Enum}. Default value is 'Backward' \cr
  }
  See example below.
}

\item{settlementDays}{an integer, 1 for T+1, 2 for T+2, etc...}
\item{yield}{yield of the bond}
\item{price}{price of the bond}
\item{effectiveDate}{bond's effective date}
\item{maturityDate}{bond's maturity date}
\item{period}{frequency of events,0=NoFrequency, 1=Once, 2=Annual, 3=Semiannual, 4=EveryFourthMonth, 5=Quarterly, 6=Bimonthly ,7=Monthly ,8=EveryFourthWeely,9=Biweekly, 10=Weekly, 11=Daily. For more information, see QuantLib's Frequency class }
\item{calendar}{Business Calendar. Either \code{us} or \code{uk}}
\item{faceAmount}{face amount of the bond}
\item{businessDayConvention}{convention used to adjust a date in case it is not a valid business day. See quantlib for more detail. 0 = Following, 1 = ModifiedFollowing, 2 = Preceding, 3 = ModifiedPreceding, other = Unadjusted}
\item{dayCounter}{day count convention. 0 = Actual360(), 1 = Actual365Fixed(), 2 = ActualActual(), 3 = Business252(), 4 = OneDayCounter(), 5 = SimpleDayCounter(), all other = Thirty360(). For more information, see QuantLib's DayCounter class}
\item{compound}{compounding type. 0=Simple, 1=Compounded, 2=Continuous, all other=SimpleThenCompounded. See QuantLib's Compound class}
\item{redemption}{redemption when the bond expires}
\item{issueDate}{date the bond is issued}

}
\value{
  The \code{FixedRateBond} function returns an object of class
  \code{FixedRateBond} (which inherits from class 
  \code{Bond}). It contains a list with the following
  components:
  \item{NPV}{net present value of the bond}
  \item{cleanPrice}{clean price of the bond}
  \item{dirtyPrice}{dirty price of the bond}
  \item{accruedAmount}{accrued amount of the bond}
  \item{yield}{yield of the bond}
  \item{cashFlows}{cash flows of the bond}

  The \code{FixedRateBondPriceByYield} function returns an object of class
  \code{FixedRateBondPriceByYield} (which inherits from class 
  \code{Bond}). It contains a list with the following
  components:
  \item{price}{price of the bond}

  The \code{FixedRateBondYield} function returns an object of class
  \code{FixedRateBondYield} (which inherits from class 
  \code{Bond}). It contains a list with the following
  components:
  \item{yield}{yield of the bond}
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
#Simple call with a flat curve
bond <- list(faceAmount=100,
             issueDate=as.Date("2004-11-30"),
             maturityDate=as.Date("2008-11-30"),
             redemption=100, 
             effectiveDate=as.Date("2004-11-30"))
dateparams <- list(settlementDays=1,
                   calendar="us", dayCounter = 'Thirty360', period=2, 
                   businessDayConvention = 4, terminationDateConvention=4,
                   dateGeneration=1, endOfMonth=1)
coupon.rate <- c(0.02875)
                       
params <- list(tradeDate=as.Date('2002-2-15'),
               settleDate=as.Date('2002-2-19'),
               dt=.25,
               interpWhat="discount",
               interpHow="loglinear")
setEvaluationDate(as.Date("2004-11-22"))

discountCurve.flat <- DiscountCurve(params, list(flat=0.05))
FixedRateBond(bond, coupon.rate, discountCurve.flat, dateparams)


#Same bond with a discount curve constructed from market quotes
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

discountCurve <- DiscountCurve(params, tsQuotes)
FixedRateBond(bond, coupon.rate, discountCurve, dateparams)


#example with default dateparams
FixedRateBond(bond, coupon.rate, discountCurve)

##exampe with defaul bond parameter and dateparams
bond <- list(issueDate=as.Date("2004-11-30"),
             maturityDate=as.Date("2008-11-30"))
dateparams <- list(calendar="us",
                   dayCounter = "ActualActual", 
                   period="Annual")
FixedRateBond(bond, coupon.rate, discountCurve, dateparams)


FixedRateBondPriceByYield(,0.0307, 100000, as.Date("2004-11-30"), as.Date("2008-11-30"), 3, , c(0.02875), , , , ,as.Date("2004-11-30"))

FixedRateBondYield(,90, 100000, as.Date("2004-11-30"), as.Date("2008-11-30"), 3, , c(0.02875), , , , ,as.Date("2004-11-30"))

}
\keyword{misc}

