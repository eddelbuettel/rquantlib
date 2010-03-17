% $Id: Calendars.Rd 64 2009-04-10 20:24:33Z edd $
\name{holidayList}
\alias{holidayList}
\title{Calendar functions from QuantLib}
\description{
The \code{holidayList} function evaluates two given dates in the context
of the given calendar, and returns a vector that gives the list of holiday
between.  
}
\usage{
holidayList(calendar="TARGET", from=Sys.Date(),
to = Sys.Date() + 5, includeWeekends = 0) 
}
\arguments{
  \item{calendar}{A string identifying one of the supported QuantLib
    calendars, see Details for more}
  \item{from}{A vector (or scalar) of \code{Date} types.}
  \item{to}{A vector (or scalar) of \code{Date} types.}
  \item{includeWeekends}{boolean that indicates whether the calculation
    should include the weekends. Default = false}
}
\value{
  An vector of dates. 
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
  Khanh Nguyen \email{nguyen.h.khanh@gmail.com} for the implementation;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
\examples{
  from <- as.Date("2009-04-07")
to<-as.Date("2009-04-14")
holidayList("UnitedStates", from, to)
to <- as.Date("2009-10-7")
holidayList("UnitedStates", from, to)
}
\keyword{misc}

