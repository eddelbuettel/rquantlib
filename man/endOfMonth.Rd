% $Id: Calendars.Rd 64 2009-04-10 20:24:33Z edd $
\name{endOfMonth}
\alias{endOfMonth}
\title{Calendar functions from QuantLib}
\description{
The \code{endOfMonth} function evaluates the given dates in the context
of the given calendar, and returns a vector that corresponds to the end
of month.}
\usage{
endOfMonth(calendar="TARGET", dates=Sys.Date())
}
\arguments{
  \item{calendar}{A string identifying one of the supported QuantLib
    calendars, see Details for more}
  \item{dates}{A vector (or scalar) of \code{Date} types.}
}
\value{
  An named vector of dates, each of which is the end of month date that
  corresponds to the input dates.  The element names are
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
  Khanh Nguyen \email{nguyen.h.khanh@gmail.com} for the implementation;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
\examples{
  dates <- seq(from=as.Date("2009-04-07"), to=as.Date("2009-04-14"), by=1)
  endOfMonth("UnitedStates", dates)
  endOfMonth("UnitedStates/Settlement", dates)      ## same as previous
  endOfMonth("UnitedStates/NYSE", dates)            ## stocks
  endOfMonth("UnitedStates/GovernmentBond", dates)  ## bonds
  endOfMonth("UnitedStates/NERC", dates)            ## energy
}
\keyword{misc}

