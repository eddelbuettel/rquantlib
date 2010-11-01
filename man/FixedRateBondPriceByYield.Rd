% $Id$
\name{FixedRateBondPriceByYield}
\alias{FixedRateBondPriceByYield}
\alias{FixedRateBondPriceByYield.default}
\title{Zero Coupon Bond Yield evaluation}
\description{
  The \code{FixedRateBondPriceByYield} function calculates the theoretical price of a fixed rate bond from its yield
}
\usage{
\method{FixedRateBondPriceByYield}{default}( settlementDays=1, yield, faceAmount, 
                                 effectiveDate, maturityDate,
                                 period, calendar="us", 
                                 rates, dayCounter=2,
                                 businessDayConvention=0, compound = 0, redemption=100, 
                                 issueDate)
}
\arguments{
  \item{settlementDays}{an integer, 1 for T+1, 2 for T+2, etc...}
  \item{yield}{yield of the bond}
  \item{effectiveDate}{bond's effective date}
  \item{maturityDate}{bond's maturity date}
  \item{period}{frequency of events,0=NoFrequency, 1=Once, 2=Annual, 3=Semiannual, 4=EveryFourthMonth, 5=Quarterly, 6=Bimonthly ,7=Monthly ,8=EveryFourthWeely,9=Biweekly, 10=Weekly, 11=Daily. For more information, see QuantLib's Frequency class }
  \item{calendar}{Business Calendar. Either \code{us} or \code{uk}}
  \item{faceAmount}{face amount of the bond}
  \item{rates}{vector of rates}
  \item{businessDayConvention}{convention used to adjust a date in case it is not a valid business day. See quantlib for more detail. 0 = Following, 1 = ModifiedFollowing, 2 = Preceding, 3 = ModifiedPreceding, other = Unadjusted}
  \item{dayCounter}{day count convention. 0 = Actual360(), 1 = Actual365Fixed(), 2 = ActualActual(), 3 = Business252(), 4 = OneDayCounter(), 5 = SimpleDayCounter(), all other = Thirty360(). For more information, see QuantLib's DayCounter class}
  \item{compound}{compounding type. 0=Simple, 1=Compounded, 2=Continuous, all other=SimpleThenCompounded. See QuantLib's Compound class}
  \item{redemption}{redemption when the bond expires}
  \item{issueDate}{date the bond is issued}
}

\value{
  The \code{FixedRateBondPriceByYield} function returns an object of class
  \code{FixedRateBondPriceByYield} (which inherits from class 
  \code{Bond}). It contains a list with the following
  components:
  \item{yield}{yield of the bond}
}
%\details{
%}
\references{\url{http://quantlib.org} for details on \code{QuantLib}. \url{http://www.mathworks.com/access/helpdesk/help/toolbox/finfixed/FixedRateBondPriceByYield.html} for more details about this function}

\author{Khanh Nguyen \email{knguyen@cs.umb.edu}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}


\examples{
FixedRateBondPriceByYield(,0.0307, 100000, as.Date("2004-11-30"), as.Date("2008-11-30"), 3, , c(0.02875), , , , ,as.Date("2004-11-30"))
}
\keyword{misc}

