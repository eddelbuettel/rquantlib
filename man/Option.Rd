% $Id: Option.Rd,v 1.1 2002/02/26 03:49:27 edd Exp $
\name{Option}
\alias{Option}
\alias{plot.Option}
\alias{print.Option}
\alias{summary.Option}
\title{Base class for option price evalution}
\description{
  This class forms the basis from which the more specific classes are
  derived. }
\usage{
\method{print}{Option}(x, digits=4, ...)
\method{plot}{Option}(x, ...)
\method{summary}{Option}(object, digits=4, ...)
}
\arguments{
  \item{x}{Any option object derived from this base class} 
  \item{object}{Any option object derived from this base class} 
  \item{digits}{Number of digits of precision shown} 
  \item{...}{Further arguments} 
}
\value{
  None, but side effects of displaying content.
}
\details{
  Please see any decent Finance textbook for background reading, and the
  \code{QuantLib} documentation for details on the \code{QuantLib}
  implementation.  
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
\seealso{\code{\link{AmericanOption}},\code{\link{EuropeanOption}},
\code{\link{BinaryOption}}}

\examples{
EO<-EuropeanOption("call", strike=100, volatility=0.4, 100, 0.01, 0.03, 0.5)
print(EO)
summary(EO)
}
\keyword{misc}

