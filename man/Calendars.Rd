% $Id$
\name{Calendars}
\alias{isBusinessDay}
\alias{businessDay}
\alias{isHoliday}
\alias{isWeekend}
\alias{isEndOfMonth}
\alias{getEndOfMonth}
\alias{endOfMonth}
\alias{getHolidayList}
\alias{holidayList}
\title{Calendar functions from QuantLib}
\description{
The \code{isBusinessDay} function evaluates the given dates in the context
of the given calendar, and returns a vector of booleans indicating
business day status. \code{BusinessDay} is also
recognised (but may be deprecated one day).

The \code{isHoliday} function evaluates the given dates in the context
of the given calendar, and returns a vector of booleans indicating
holiday day status.

The \code{isWeekend} function evaluates the given dates in the context
of the given calendar, and returns a vector of booleans indicating
weekend status.

The \code{isEndOfMonth} function evaluates the given dates in the context
of the given calendar, and returns a vector of booleans indicating
end of month status.

The \code{getEndOfMonth} function evaluates the given dates in the context
of the given calendar, and returns a vector that corresponds to the end
of month. \code{endOfMonth} is a deprecated form for this function.

The \code{getHolidayList} function returns the holidays between the
given dates, with an option to exclude weekends.  \code{holidayList} is
a deprecated form for this function.

}
\usage{
isBusinessDay(calendar="TARGET", dates=Sys.Date())
businessDay(calendar="TARGET", dates=Sys.Date())  # deprecated form
isHoliday(calendar="TARGET", dates=Sys.Date())
isWeekend(calendar="TARGET", dates=Sys.Date())
isEndOfMonth(calendar="TARGET", dates=Sys.Date())
getEndOfMonth(calendar="TARGET", dates=Sys.Date())
endOfMonth(calendar="TARGET", dates=Sys.Date())
getHolidayList(calendar="TARGET", from=Sys.Date(), to = Sys.Date() + 5, includeWeekends = 0) 
holidayList(calendar="TARGET", from=Sys.Date(), to = Sys.Date() + 5, includeWeekends = 0) 
}
\arguments{
  \item{calendar}{A string identifying one of the supported QuantLib
    calendars, see Details for more}
  \item{dates}{A vector (or scalar) of \code{Date} types.}
  \item{from}{A vector (or scalar) of \code{Date} types.}
  \item{to}{A vector (or scalar) of \code{Date} types.}
  \item{includeWeekends}{boolean that indicates whether the calculation
    should include the weekends. Default = false}
}
\value{
  A named vector of booleans each of which is true if the corresponding
  date is a business day (or holiday or weekend) in the given calendar.
  The element names are
  the dates (formatted as text in yyyy-mm-dd format).
}
\details{
  The calendars are coming from QuantLib, and the QuantLib documentation
  should be consulted for details.

  Currently, the following strings are recognised: TARGET (a default
  calendar), Canada and Canada/Settlement, Canada/TSX, Germany and
  Germany/FrankfurtStockExchange, Germany/Settlement, Germany/Xetra,
  Germany/Eurex, Italy and Italy/Settlement, Italy/Exchange, Japan,
  UnitedKingdom and UnitedKingdom/Settlement, UnitedKingdom/Exchange,
  UnitedKingdom/Metals, UnitedStates and UnitedStates/Settlement,
  UnitedStates/NYSE, UnitedStates/GovernmentBond, UnitedStates/NERC.

  (In case of multiples entries per country, the country default is listed
  right after the country itself. Using the shorter form is equivalent.)
  
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
\examples{
  dates <- seq(from=as.Date("2009-04-07"), to=as.Date("2009-04-14"), by=1)
  isBusinessDay("UnitedStates", dates)
  isBusinessDay("UnitedStates/Settlement", dates)      ## same as previous
  isBusinessDay("UnitedStates/NYSE", dates)            ## stocks
  isBusinessDay("UnitedStates/GovernmentBond", dates)  ## bonds
  isBusinessDay("UnitedStates/NERC", dates)            ## energy

  isHoliday("UnitedStates", dates)
  isHoliday("UnitedStates/Settlement", dates)      ## same as previous
  isHoliday("UnitedStates/NYSE", dates)            ## stocks
  isHoliday("UnitedStates/GovernmentBond", dates)  ## bonds
  isHoliday("UnitedStates/NERC", dates)            ## energy

  isWeekend("UnitedStates", dates)
  isWeekend("UnitedStates/Settlement", dates)      ## same as previous
  isWeekend("UnitedStates/NYSE", dates)            ## stocks
  isWeekend("UnitedStates/GovernmentBond", dates)  ## bonds
  isWeekend("UnitedStates/NERC", dates)            ## energy

  isEndOfMonth("UnitedStates", dates)
  isEndOfMonth("UnitedStates/Settlement", dates)      ## same as previous
  isEndOfMonth("UnitedStates/NYSE", dates)            ## stocks
  isEndOfMonth("UnitedStates/GovernmentBond", dates)  ## bonds
  isEndOfMonth("UnitedStates/NERC", dates)            ## energy

  getEndMonth("UnitedStates", dates)
  getEndMonth("UnitedStates/Settlement", dates)      ## same as previous
  getEndMonth("UnitedStates/NYSE", dates)            ## stocks
  getEndMonth("UnitedStates/GovernmentBond", dates)  ## bonds
  getEndMonth("UnitedStates/NERC", dates)            ## energy

  from <- as.Date("2009-04-07")
  to<-as.Date("2009-04-14")
  getHolidayList("UnitedStates", from, to)
  to <- as.Date("2009-10-7")
  getHolidayList("UnitedStates", from, to)
}
\keyword{misc}

