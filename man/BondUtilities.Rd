% $Id$
\name{BondUtilities}
\alias{matchBDC}
\alias{matchCompounding}
\alias{matchDateGen}
\alias{matchDayCounter}
\alias{matchFrequency}
\alias{matchParams}
\title{Bond parameter conversion utilities}
\description{
These functions are using internally to convert from the characters at
the R level to the \code{enum} types used at the C++ level. They are
documented here mostly to provide a means to look up some of the
possible values---the user is not expected to call these functions directly..}
\usage{
matchBDC(bdc = c("Following", "ModifiedFollowing", "Preceding", "ModifiedPreceding", "Unadjusted")) 
matchCompounding(cp = c("Simple", "Compounded", "Continuous", "SimpleThenCompounded")) 
matchDayCounter(daycounter = c("Actual360", "ActualFixed", "ActualActual", "Business252", "OneDayCounter", "SimpleDayCounter", "Thirty360"))
matchDateGen(dg = c("Backward", "Forward", "Zero", "ThirdWednesday", "Twentieth", "TwentiethIMM"))
matchFrequency(freq = c("NoFrequency","Once", "Annual", "Semiannual", "EveryFourthMonth", "Quarterly", "Bimonthly", "EveryFourthWeek", "Biweekly", "Weekly", "Daily"))
matchParams(params) 
}
\arguments{
  \item{bdc}{A string identifying one of the possible business day convention values.}
  \item{cp}{A string identifying one of the possible compounding frequency values.}
  \item{daycounter}{A string identifying one of the possible day counter scheme values.}
  \item{dg}{A string identifying one of the possible date generation scheme values.}
  \item{freq}{A string identifying one of the possible (dividend) frequency values.}
  \item{params}{A named vector containing the other parameters as components.}
}
\value{
  Each function converts the given character value into a corresponding
  numeric entry. For \code{matchParams}, an named vector of strings is
  converted into a named vector of numerics..
}
\details{
  The QuantLib documentation
  should be consulted for details.
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{Khanh Nguyen \email{knguyen@cs.umb.edu} for the \R interface;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
%\examples{}
\keyword{misc}

