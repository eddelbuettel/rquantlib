% $Id: Calendars.Rd 64 2009-04-10 20:24:33Z edd $
\name{dayCount}
\alias{dayCount}
\title{DayCounter functions from QuantLib}
\description{
The \code{dayCount} function returns the number of day between two dates
given a day counter \link{Enum}}
\usage{
dayCount(startDates, endDates, dayCounters)
}
\arguments{
  \item{startDates}{A vector of \code{Date} type.}
  \item{endDates}{A vector of \code{Date} type.}
  \item{dayCounters}{A vector of numeric type. See \link{Enum}}
}
\value{
A numeric vector contains the number of day between two dates from the
input.
}
\details{
  The day counters are coming from QuantLib, and the QuantLib documentation
  should be consulted for details. See \link{Enum} and \url{http://quantlib.org/reference/class_quant_lib_1_1_day_counter.html}
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  Khanh Nguyen \email{nguyen.h.khanh@gmail.com} for the implementation;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
\examples{
startDates <- seq(from=as.Date("2009-04-07"), to=as.Date("2009-04-14"),by=1)
endDates <- seq(from=as.Date("2009-11-07"), to=as.Date("2009-11-14"), by=1)
dayCounters <- c(0,1,2,3,4,5,6,1)
dayCount(startDates, endDates, dayCounters)
}
\keyword{misc}

