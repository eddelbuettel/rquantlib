% $Id: CallableBond.Rd 87 2009-06-09 16:48:13Z knguyen $
\name{CallableBond}
\alias{CallableBond}
\alias{CallableBond.default}
\title{CallableBond evaluation}
\description{
  The \code{CallableBond} function sets up and evaluates a callable fixed rate bond using Hull-White model 
  and a TreeCallableFixedBondEngine pricing engine. For more detail, see the source codes in quantlib's example folder, Examples/CallableBond/CallableBond.cpp
}
\usage{
\method{CallableBond}{default}(bondparams, hullWhite, coupon, dateparams)
\method{plot}{Bond}
\method{print}{Bond}
\method{summary}{Bond}
}
\arguments{
\item{bondparams}{a named list whose elements are: 
  \tabular{ll}{
    \code{issueDate}  \tab a Date, the bond's issue date \cr
    \code{maturityDate} \tab a Date, the bond's maturity date \cr
    \code{faceAmount} \tab (Optional) a double, face amount of the bond. \cr
    \code{}           \tab Default value is 100.\cr
    \code{redemption} \tab (Optional) a double, percentage of the initial face \cr 
    \code{}           \tab amount that will be returned at maturity date. \cr
    \code{}           \tab Default value is 100. \cr
    \code{callSch} \tab (Optional) a data frame whose columns are "Price",\cr
    \code{}        \tab "Type" and "Date" corresponding to QuantLib's \cr
    \code{}        \tab CallabilitySchedule. Defaule is an empty frame, or no callability.\cr
  } 
}
\item{hullWhite}{a named list whose elements are parameters needed to set up a HullWhite pricing engine in QuantLib: 
  \tabular{ll}{
    \code{term}  \tab a double, to set up a flat rate yield term structure \cr
    \code{alpha} \tab a double, Hull-White model's alpha value \cr
    \code{sigma} \tab a double, Hull-White model's sigma value \cr
    \code{gridIntervals}. \tab a double, time intervals parameter to \cr
    \code{}              \tab set up the TreeCallableFixedBondEngine \cr
  } 
  Currently, the codes only support a flat rate yield term structure. For more detail, see QuantLib's doc on HullWhite 
  and TreeCallableFixedBondEngine.
}
\item{coupon}{a numeric vector of coupon rates}
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
    \code{} \tab See \link{Enum}. Default value is 'Following'. \cr
    \code{terminationDateConvention} \tab (Optional) a number or string \cr 
    \code{}                         \tab termination day convention.\cr
    \code{}                         \tab See \link{Enum}. Default value is'Following'.\cr
  }
  See example below.
}
}
\value{
  The \code{CallableBond} function returns an object of class
  \code{CallableBond} (which inherits from class 
  \code{Bond}). It contains a list with the following
  components:
  \item{NPV}{net present value of the bond}
  \item{cleanPrice}{price price of the bond}
  \item{dirtyPrice}{dirty price of the bond}
  \item{accruedAmount}{accrued amount of the bond}
  \item{yield}{yield of the bond}
  \item{cashFlows}{cash flows of the bond}
}
\details{
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
#set-up a HullWhite according to example from QuantLib
HullWhite <- list(term = 0.055, alpha = 0.03, sigma = 0.01,
                  gridIntervals = 40)

#callability schedule dataframe
Price <- rep(as.double(100),24)
Type <- rep(as.character("C"), 24)
Date <- seq(as.Date("2006-09-15"), by = '3 months', length = 24)
callSch <- data.frame(Price, Type, Date)
callSch$Type <- as.character(callSch$Type)

bondparams <- list(faceAmount=100, issueDate = as.Date("2004-09-16"),
                   maturityDate=as.Date("2012-09-16"), redemption=100,
                   callSch = callSch)
dateparams <- list(settlementDays=3, calendar="us", 
                   dayCounter = "ActualActual", 
                   period="Quarterly", 
                   businessDayConvention = "Unadjusted", 
                   terminationDateConvention= "Unadjusted")
coupon <- c(0.0465)

CallableBond(bondparams, HullWhite, coupon, dateparams)
#examples using default values
CallableBond(bondparams, HullWhite, coupon)
dateparams <- list(                   
                   period="Quarterly", 
                   businessDayConvention = "Unadjusted", 
                   terminationDateConvention= "Unadjusted")
CallableBond(bondparams, HullWhite, coupon, dateparams)


bondparams <- list(issueDate = as.Date("2004-09-16"),
                   maturityDate=as.Date("2012-09-16")
                   )
CallableBond(bondparams, HullWhite, coupon, dateparams)
}
\keyword{misc}

