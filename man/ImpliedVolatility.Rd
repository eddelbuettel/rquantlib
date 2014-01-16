% $Id: ImpliedVolatility.Rd,v 1.2 2003/08/01 01:23:48 edd Exp $
\name{ImpliedVolatility}
\alias{ImpliedVolatility}
\alias{print.ImpliedVolatility}
\alias{summary.ImpliedVolatility}
\title{Base class for option-price implied volatility evalution}
\description{
  This class forms the basis from which the more specific classes are
  derived. }
\usage{
\method{print}{ImpliedVolatility}(x, digits=3, ...)
\method{summary}{ImpliedVolatility}(object, digits=3, ...)
}
\arguments{
  \item{x}{Any option-price implied volatility object derived
    from this base class} 
  \item{object}{Any option-price implied volatility object derived
    from this base class} 
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

\seealso{\code{\link{AmericanOptionImpliedVolatility}},
  \code{\link{EuropeanOptionImpliedVolatility}},
  \code{\link{AmericanOption}},\code{\link{EuropeanOption}},
  \code{\link{BinaryOption}}}

\examples{
impVol<-EuropeanOptionImpliedVolatility("call", value=11.10, strike=100,
                                        volatility=0.4, 100, 0.01, 0.03, 0.5)
print(impVol)
summary(impVol)
}
\keyword{misc}

