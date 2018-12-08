\name{Enum}
\alias{Enum}
\title{Documentation for parameters}
\description{
Reference for parameters when constructing a bond
}
%\usage{}
\arguments{
\item{DayCounter}{an int value
       \tabular{ll}{
         \code{0} \tab Actual360 \cr
         \code{1} \tab Actual360FixEd \cr
         \code{2} \tab ActualActual \cr
         \code{3} \tab ActualBusiness252 \cr
         \code{4} \tab OneDayCounter \cr
         \code{5} \tab SimpleDayCounter \cr
         \code{6} \tab Thirty360 \cr
         \code{7} \tab Actual365NoLeap (NB: deprecated) \cr
         \code{8} \tab ActualActual.ISMA \cr
         \code{9} \tab ActualActual.Bond \cr
         \code{10} \tab ActualActual.ISDA \cr
         \code{11} \tab ActualActual.Historical \cr
         \code{12} \tab ActualActual.AFB \cr
         \code{anything else} \tab ActualActual.Euro
       }
}

\item{businessDayConvention}{an int value
      \tabular{ll}{
        \code{0} \tab Following \cr
        \code{1} \tab ModifiedFollowing \cr
        \code{2} \tab Preceding \cr
        \code{3} \tab ModifiedPreceding \cr
        \code{4} \tab Unadjusted \cr
        \code{5} \tab HalfMonthModifiedFollowing \cr
        \code{6} \tab Nearest \cr
        \code{anything else} \tab Unadjusted
      }
}

\item{compounding}{an int value
     \tabular{ll}{
       \code{0} \tab Simple \cr
       \code{1} \tab Compounded \cr
       \code{2} \tab Continuous \cr
       \code{3} \tab SimpleThenCompounded 
     }
}

\item{period or frequency}{an int value
     \tabular{ll}{
       \code{-1} \tab NoFrequency \cr
       \code{0} \tab Once \cr
       \code{1} \tab Annual \cr
       \code{2} \tab Semiannual \cr
       \code{3} \tab EveryFourthMonth \cr
       \code{4} \tab Quarterly \cr
       \code{6} \tab BiMonthtly \cr
       \code{12} \tab Monthly \cr
       \code{13} \tab EveryFourthWeek \cr
       \code{26} \tab BiWeekly \cr
       \code{52} \tab Weekly \cr
       \code{365} \tab Daily \cr       
       \code{anything else} \tab OtherFrequency
     }
}

\item{date generation}{an int value to specify date generation rule
     \tabular{ll}{
        \code{0} \tab Backward \cr
        \code{1} \tab Forward \cr
        \code{2} \tab Zero \cr
        \code{3} \tab ThirdWednesday \cr
        \code{4} \tab Twentieth \cr
        \code{5} \tab TwentiethIMM \cr
        \code{6} \tab OldCDS \cr
        \code{7} \tab CDS \cr
        \code{anything else} \tab TwentiethIMM
     }
}

\item{durationType}{an int value to specify duration type
     \tabular{ll}{
        \code{0} \tab Simple \cr
        \code{1} \tab Macaulay \cr
        \code{2} \tab Modified
     }
}

}
\value{
None
}
\details{
 Please see any decent Finance textbook for background reading, and the
  \code{QuantLib} documentation for details on the \code{QuantLib}
  implementation, particularly the datetime classes.
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{Khanh Nguyen \email{knguyen@cs.umb.edu}}

\keyword{misc}

