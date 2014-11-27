% $Id$
\name{Schedule}
\alias{Schedule}
\alias{Schedule.default}
\title{Schedule generation}
\description{
  The \code{Schedule} function generates a schedule of dates conformant to
  a given convention in a given calendar.
}
\usage{
\method{Schedule}{default}(params)
}
\arguments{

\item{params}{a named list, QuantLib's parameters of the schedule. 
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

}

\value{
  The \code{Schedule} function returns an object of class
  \code{Schedule}. It contains the list of dates in the schedule.
}
\details{
  Please see any decent Finance textbook for background reading, and the
  \code{QuantLib} documentation for details on the \code{QuantLib}
  implementation.  
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{
  Michele Salvadore \email{michele.salvadore@gmail.com} for the inplementation;
  Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  the QuantLib Group for \code{QuantLib}
}

\seealso{
  \code{\link{FixedRateBond}}
}

\examples{

params <- list(effectiveDate=as.Date("2004-11-30"),
               maturityDate=as.Date("2008-11-30"),
               period='Semiannual',
               calendar='UnitedStates/GovernmentBond',
               businessDayConvention='Unadjusted',
               terminationDateConvention='Unadjusted',
               dateGeneration='Forward',
               endOfMonth=1)
Schedule(params)
}
\keyword{misc}
