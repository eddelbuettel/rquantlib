% $Id: Calendars.Rd 64 2009-04-10 20:24:33Z edd $
\name{adjust}
\alias{adjust}
\title{Calendar functions from QuantLib}
\description{
The \code{adjust} function evaluates the given dates in the context
of the given calendar, and returns a vector that adjusts each input
dates to the appropriate near business day with respect to the given convention. 
.}
\usage{
adjust(calendar="TARGET", dates=Sys.Date(), bdc = 0)
}
\arguments{
  \item{calendar}{A string identifying one of the supported QuantLib
    calendars, see Details for more}
  \item{dates}{A vector (or scalar) of \code{Date} types.}
  \item{bdc}{business day convention. By default, this value is 0 and
    correspond to Following convention}
}
\value{
  An named vector of dates. The element names are
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
  adjust("UnitedStates", dates)
  adjust("UnitedStates/Settlement", dates)      ## same as previous
  adjust("UnitedStates/NYSE", dates)            ## stocks
  adjust("UnitedStates/GovernmentBond", dates)  ## bonds
  adjust("UnitedStates/NERC", dates)            ## energy
}
\keyword{misc}

