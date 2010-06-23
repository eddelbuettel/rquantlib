% $Id$
\name{setCalendarContext}
\alias{setCalendarContext}
\title{Set global calendar constants}
\description{
The \code{setCalendarContext} function sets three values to a singleton
instance at the C++ layer.}
\usage{
setCalendarContext(calendar, fixingDays, settleDate)
}
\arguments{
  \item{calendar}{A string denoting a calendar, default is 'TARGET' or a
    non-country specific calendar.}
  \item{fixingDays}{An integer for the fixing day period, defaults to 2.}
  \item{settleDate}{A date on which trades settles, defaults to two days
    after the current day.}
}
\value{
  A boolean or NULL in case of error.
}
\details{
  These values are assigned to global 'singleton' instance (of which
  only one can ever be instantiated) and permit to store these values
  between calls. 
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  Khanh Nguyen \email{nguyen.h.khanh@gmail.com} for the implementation;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
\keyword{misc}
