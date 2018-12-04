\name{FixedRateBond}
\alias{FixedRateBond}
\alias{FixedRateBond.default}
\alias{FixedRateBondPriceByYield}
\alias{FixedRateBondPriceByYield.default}
\alias{FixedRateBondYield}
\alias{FixedRateBondYield.default}
\title{Fixed-Rate bond pricing}
\description{
  The \code{FixedRateBond} function evaluates a fixed rate bond using discount curve, the yield or the clean price. 
  More specificly, when a discount curve is provided the calculation is done by DiscountingBondEngine from QuantLib.
  The NPV, clean price, dirty price, accrued interest, yield, duration, actual settlement date and cash flows of the bond is returned.
  When a yield is provided instead, no engine is provided to the bond class and prices are computed from yield. In the latter case, NPV is set to NA. Same situation when the clean price is given instead of discount curve or yield.
  For more detail, see the source codes in QuantLib's file \code{test-suite/bond.cpp}.

  The \code{FixedRateBondPriceByYield} function calculates the theoretical price of a fixed rate bond from its yield.

  The \code{FixedRateBondYield} function calculates the theoretical yield of a fixed rate bond from its price.
}
\usage{

\method{FixedRateBond}{default}(bond, rates, schedule,
                                calc=list(dayCounter='ActualActual.ISMA',
                                          compounding='Compounded',
                                          freq='Annual',
                                          durationType='Modified'),
                                discountCurve = NULL, yield = NA, price = NA)

\method{FixedRateBondPriceByYield}{default}( settlementDays=1, yield, faceAmount=100, 
                                 effectiveDate, maturityDate,
                                 period, calendar="UnitedStates/GovernmentBond", 
                                 rates, dayCounter=2,
                                 businessDayConvention=0, compound = 0, redemption=100, 
                                 issueDate)

\method{FixedRateBondYield}{default}( settlementDays=1, price, faceAmount=100, 
                                 effectiveDate, maturityDate,
                                 period, calendar="UnitedStates/GovernmentBond", 
                                 rates, dayCounter=2,
                                 businessDayConvention=0, 
                                 compound = 0, redemption=100, 
                                 issueDate)
}
\arguments{
\item{bond}{(Optional) bond parameters, a named list whose elements are: 
  \tabular{ll}{
    \code{settlementDays}      \tab (Optional) a double, settlement days. \cr 
    \code{}                    \tab Default value is 1.\cr
    \code{faceAmount}          \tab (Optional) a double, face amount of the bond.\cr
    \code{}                    \tab  Default value is 100. \cr
    \code{dayCounter}          \tab (Optional) a number or string, \cr 
    \code{}                    \tab day counter convention. Defaults to 'Thirty360' \cr
    \code{issueDate}           \tab (Optional) a Date, the bond's issue date\cr
    \code{}                    \tab Defaults to QuantLib default. \cr
    \code{paymentConvention}   \tab (Optional) a number or string, the bond \cr
    \code{}                    \tab payment convention. \cr
    \code{}                    \tab Defaults to QuantLib default. \cr
    \code{redemption}          \tab (Optional) a double, the redemption amount. \cr
    \code{}                    \tab Defaults to QuantLib default (100). \cr
    \code{paymentCalendar}     \tab (Optional) a string, the name of the calendar. \cr
    \code{}                    \tab Defaults to QuantLib default. \cr
    \code{exCouponPeriod}      \tab (Optional) a number, the number of days when \cr
    \code{}                    \tab the coupon goes ex relative to the coupon date. \cr
    \code{}                    \tab Defaults to QuantLib default. \cr
    \code{exCouponCalendar}    \tab (Optional) a string, the name of the \cr
    \code{}                    \tab ex-coupon calendar. \cr
    \code{}                    \tab Defaults to QuantLib default. \cr
    \code{exCouponConvention}  \tab (Optional) a number or string, the coupon \cr
    \code{}                    \tab payment convention. \cr
    \code{}                    \tab Defaults to QuantLib default. \cr
    \code{exCouponEndOfMonth}  \tab (Optional) 1 or 0, use End of Month rule for \cr
    \code{}                    \tab ex-coupon dates. Defaults to 0 (false).
  }
}

\item{rates}{a numeric vector, bond's coupon rates}

\item{schedule}{(Optional) a named list, QuantLib's parameters of the bond's schedule. 
  \tabular{ll}{
    \code{effectiveDate}       \tab a Date, when the schedule becomes effective. \cr 
    \code{maturityDate}        \tab a Date, when the schedule matures. \cr 
    \code{period}              \tab (Optional) a number or string, the frequency of \cr
    \code{}                    \tab the schedule. Default value is 'Semiannual'. \cr
    \code{calendar}            \tab (Optional) a string, the calendar name. \cr 
    \code{}                    \tab Defaults to 'TARGET' \cr
    \code{businessDayConvention} \tab (Optional) a number or string, the \cr 
    \code{}                    \tab day convention to use. \cr
    \code{}                    \tab Defaults to 'Following'. \cr
    \code{terminationDateConvention} \tab (Optional) a number or string, the \cr 
    \code{}                    \tab day convention to use for the terminal date. \cr
    \code{}                    \tab Defaults to 'Following'. \cr
    \code{dateGeneration}      \tab (Optional) a number or string, the \cr 
    \code{}                    \tab date generation rule. \cr
    \code{}                    \tab Defaults to 'Backward'. \cr
    \code{endOfMonth}          \tab (Optional) 1 or 0, use End of Month rule for \cr
    \code{}                    \tab schedule dates. Defaults to 0 (false).
  }
  See example below.
}

\item{calc}{(Optional) a named list, QuantLib's parameters for calculations. 
  \tabular{ll}{
    \code{dayCounter}          \tab (Optional) a number or string, day counter \cr 
    \code{}                    \tab convention. Defaults to 'ActualActual.ISMA' \cr
    \code{compounding}         \tab a string, what kind of compounding to use. \cr 
    \code{}                    \tab Defaults to 'Compounded' \cr
    \code{freq}                \tab (Optional) a number or string, the frequency \cr
    \code{}                    \tab to use. Default value is 'Annual'. \cr
    \code{durationType}        \tab (Optional) a number or string, the type of \cr 
    \code{}                    \tab duration to calculate. Defaults to 'Simple' \cr
    \code{accuracy}            \tab (Optional) a number, the accuracy required. \cr 
    \code{}                    \tab Defaults to 1.0e-8. \cr
    \code{maxEvaluations}      \tab (Optional) a number, max number of iterations. \cr 
    \code{}                    \tab Defaults to 100.
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
    \code{}                \tab the discountCurve function
  }
}

\item{yield}{yield of the bond}
\item{price}{clean price of the bond}
\item{settlementDays}{an integer, 1 for T+1, 2 for T+2, etc...}
\item{effectiveDate}{bond's effective date}
\item{maturityDate}{bond's maturity date}
\item{period}{frequency of events,0=NoFrequency, 1=Once, 2=Annual, 3=Semiannual, 4=EveryFourthMonth, 5=Quarterly, 6=Bimonthly ,7=Monthly ,8=EveryFourthWeek,9=Biweekly, 10=Weekly, 11=Daily. For more information, see QuantLib's Frequency class }
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
  \item{duration}{the duration of the bond}
  \item{settlementDate}{the actual settlement date used for the bond}
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
bond <- list(settlementDays=1,
             issueDate=as.Date("2004-11-30"),
             faceAmount=100,
             dayCounter='Thirty360',
             paymentConvention='Unadjusted')
schedule <- list(effectiveDate=as.Date("2004-11-30"),
                 maturityDate=as.Date("2008-11-30"),
                 period='Semiannual',
                 calendar='UnitedStates/GovernmentBond',
                 businessDayConvention='Unadjusted',
                 terminationDateConvention='Unadjusted',
                 dateGeneration='Forward',
                 endOfMonth=1)
calc=list(dayCounter='Actual360',
          compounding='Compounded',
          freq='Annual',
          durationType='Modified')
coupon.rate <- c(0.02875)
                       
params <- list(tradeDate=as.Date('2002-2-15'),
               settleDate=as.Date('2002-2-19'),
               dt=.25,
               interpWhat="discount",
               interpHow="loglinear")
setEvaluationDate(as.Date("2004-11-22"))

discountCurve.flat <- DiscountCurve(params, list(flat=0.05))
FixedRateBond(bond,
              coupon.rate,
              schedule,
              calc,
              discountCurve=discountCurve.flat)


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
tsQuotes <- list("flat" = 0.02)		## While discount curve code is buggy

discountCurve <- DiscountCurve(params, tsQuotes)
FixedRateBond(bond,
              coupon.rate,
              schedule,
              calc,
              discountCurve=discountCurve)

#Same bond calculated from yield rather than from the discount curve
yield <- 0.02
FixedRateBond(bond,
              coupon.rate,
              schedule,
              calc,
              yield=yield)


#same example with clean price
price <- 103.31
FixedRateBond(bond,
              coupon.rate,
              schedule,
              calc,
              price = price)

#example with default calc parameter
FixedRateBond(bond,
              coupon.rate,
              schedule,
              discountCurve=discountCurve)
              
#example with default calc and schedule parameters
schedule <- list(effectiveDate=as.Date("2004-11-30"),
                 maturityDate=as.Date("2008-11-30"))
FixedRateBond(bond,
              coupon.rate,
              schedule,
              discountCurve=discountCurve)

#example with default calc, schedule and bond parameters
FixedRateBond(,
              coupon.rate,
              schedule,
              discountCurve=discountCurve)

FixedRateBondPriceByYield(,0.0307, 100000, as.Date("2004-11-30"),
                          as.Date("2008-11-30"), 3, , c(0.02875),
                          , , , ,as.Date("2004-11-30"))

FixedRateBondYield(,90, 100000, as.Date("2004-11-30"), as.Date("2008-11-30"),
                   3, , c(0.02875), , , , ,as.Date("2004-11-30"))

}
\keyword{misc}

