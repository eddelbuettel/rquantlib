% $Id$
\name{ZeroYield}
\alias{ZeroYield}
\alias{ZeroYield.default}
\title{Zero Coupon Bond Yield evaluation}
\description{
  The \code{ZeroYield} function evaluations a zero-coupon yield based. See also http://www.mathworks.com/access/helpdesk/help/toolbox/finfixed/zeroyield.html
}
\usage{
\method{ZeroYield}{default}(price, faceAmount, 
                            issueDate, maturityDate, 
                            dayCounter=2, frequency=2, 
                            compound=0, businessDayConvention=4)
\method{plot}{Bond}
\method{print}{Bond}
\method{summary}{Bond}
}
\arguments{
  \item{price}{price of the bond}
  \item{faceAmount}{face amount of the bond}
  \item{issueDate}{date the bond is issued}
  \item{maturityDate}{maturity date, an R's date type}
  \item{dayCounter}{day count convention. 0 = Actual360(), 1 = Actual365Fixed(), 2 = ActualActual(), 3 = Business252(), 4 = OneDayCounter(), 5 = SimpleDayCounter(), all other = Thirty360(). For more information, see QuantLib's DayCounter class}
  \item{frequency}{frequency of events,0=NoFrequency, 1=Once, 2=Annual, 3=Semiannual, 4=EveryFourthMonth, 5=Quarterly, 6=Bimonthly ,7=Monthly ,8=EveryFourthWeely,9=Biweekly, 10=Weekly, 11=Daily. For more information, see QuantLib's Frequency class }
  \item{compound}{compounding type. 0=Simple, 1=Compounded, 2=Continuous, all other=SimpleThenCompounded. See QuantLib's Compound class}
  \item{businessDayConvention}{convention used to adjust a date in case it is not a valid business day. See quantlib for more detail. 0 = Following, 1 = ModifiedFollowing, 2 = Preceding, 3 = ModifiedPreceding, other = Unadjusted}
}
\value{
  The \code{ZeroYield} function returns an object of class
  \code{ZeroYield} (which inherits from class 
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
ZeroYield(90, 100, as.Date("1993-6-24"), as.Date("1993-11-1"))
}
\keyword{misc}

