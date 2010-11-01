% $Id: ZeroPriceByYield.Rd,v 1.5 2006/11/11 04:12:45 edd Exp $
\name{ZeroPriceByYield}
\alias{ZeroPriceByYield}
\alias{ZeroPriceByYield.default}
\title{Zero Coupon Bond Theoretical Price evaluation}
\description{
  The \code{ZeroPriceYield} function evaluates a zero-coupon clean price based on its yield. 
}
\usage{
\method{ZeroPriceByYield}{default}(yield, faceAmount, 
                            issueDate, maturityDate, 
                            dayCounter=2, frequency=2, 
                            compound=0, businessDayConvention=4)
}
\arguments{
  \item{yield}{yield of the bond}
  \item{faceAmount}{face amount of the bond}
  \item{issueDate}{date the bond is issued}
  \item{maturityDate}{maturity date, an R's date type}
  \item{dayCounter}{day count convention. 0 = Actual360(), 1 = Actual365Fixed(), 2 = ActualActual(), 3 = Business252(), 4 = OneDayCounter(), 5 = SimpleDayCounter(), all other = Thirty360(). For more information, see QuantLib's DayCounter class}
  \item{frequency}{frequency of events,0=NoFrequency, 1=Once, 2=Annual, 3=Semiannual, 4=EveryFourthMonth, 5=Quarterly, 6=Bimonthly ,7=Monthly ,8=EveryFourthWeely,9=Biweekly, 10=Weekly, 11=Daily. For more information, see QuantLib's Frequency class }
  \item{compound}{compounding type. 0=Simple, 1=Compounded, 2=Continuous, all other=SimpleThenCompounded. See QuantLib's Compound class}
  \item{businessDayConvention}{convention used to adjust a date in case it is not a valid business day. See quantlib for more detail. 0 = Following, 1 = ModifiedFollowing, 2 = Preceding, 3 = ModifiedPreceding, other = Unadjusted}
}
\value{
  The \code{ZeroPriceByYield} function returns an object of class
  \code{ZeroPriceByYield} (which inherits from class 
  \code{Bond}). It contains a list with the following
  components:
  \item{yield}{yield of the bond}
}
%\details{
%}
\references{\url{http://quantlib.org} for details on \code{QuantLib}. \url{http://www.mathworks.com/access/helpdesk/help/toolbox/finfixed/zeroyield.html} for more details about this function}
\author{Khanh Nguyen \email{knguyen@cs.umb.edu}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}


\examples{
ZeroPriceByYield(0.1478, 100, as.Date("1993-6-24"), as.Date("1993-11-1"))
}
\keyword{misc}

