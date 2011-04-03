% $Id: ConvertibleFloatingCouponBond 84 2009-06-19 knguyen $
\name{ConvertibleBond}
\alias{ConvertibleFixedCouponBond}
\alias{ConvertibleFixedCouponBond.default}
\alias{ConvertibleFloatingCouponBond}
\alias{ConvertibleFloatingCouponBond.default}
\alias{ConvertibleZeroCouponBond}
\alias{ConvertibleZeroCouponBond.default}
\title{Convertible Bond evaluation for Fixed, Floating and Zero Coupon}
\description{
The \code{ConvertibleFixedCouponBond} function setups and evaluates a
ConvertibleFixedCouponBond using QuantLib's BinomialConvertibleEngine
%\url{http://quantlib.org/reference/class_quant_lib_1_1_binomial_convertible_engine.html}
and BlackScholesMertonProcess
%\url{http://quantlib.org/reference/class_quant_lib_1_1_black_scholes_merton_process.html}.
The NPV, clean price, dirty price, accrued interest, yield and cash flows of
the bond is returned. For detail, see test-suite/convertiblebond.cpp 

The \code{ConvertibleFloatingCouponBond} function setups and evaluates a
ConvertibleFixedCouponBond using QuantLib's BinomialConvertibleEngine
% \url{http://quantlib.org/reference/class_quant_lib_1_1_binomial_convertible_engine.html}
and BlackScholesMertonProcess
% \url{http://quantlib.org/reference/class_quant_lib_1_1_black_scholes_merton_process.html}.
The NPV, clean price, dirty price, accrued interest, yield and cash flows of
the bond is returned. For detail, see test-suite/convertiblebond.cpp 

The \code{ConvertibleZeroCouponBond} function setups and evaluates a
ConvertibleFixedCouponBond using QuantLib's BinomialConvertibleEngine
% \url{http://quantlib.org/reference/class_quant_lib_1_1_binomial_convertible_engine.html}
and BlackScholesMertonProcess
% \url{http://quantlib.org/reference/class_quant_lib_1_1_black_scholes_merton_process.html}.
The NPV, clean price, dirty price, accrued interest, yield and cash flows of
the bond is returned. For detail, see \code{test-suite/convertiblebond.cpp}. 
}
\usage{
\method{ConvertibleFloatingCouponBond}{default}(bondparams, iborindex, spread, process, dateparams)
\method{ConvertibleFixedCouponBond}{default}(bondparams, coupon, process, dateparams)
\method{ConvertibleZeroCouponBond}{default}(bondparams, process, dateparams)
}
\arguments{

\item{bondparams}{bond parameters, a named list whose elements are: 
	\tabular{ll}{
       \code{issueDate}     \tab a Date, the bond's issue date\cr
       \code{maturityDate}  \tab a Date, the bond's maturity date\cr
       \code{creditSpread}  \tab a double, credit spread parameter \cr 
       \code{}        \tab  in the constructor of the bond. \cr
       \code{conversitionRatio}  \tab a double, conversition ratio \cr 
       \code{}        \tab parameter in the constructor of the bond. \cr
       \code{exercise} 	\tab (Optional) a string, either "eu" for European \cr
       \code{}        \tab 	option, or "am" for American option. \cr
       \code{}        \tab  Default value is 'am'.\cr
       \code{faceAmount}    \tab (Optional) a double, face amount of the bond.\cr
       \code{}        \tab  Default value is 100. \cr
       \code{redemption}    \tab (Optional) a double, percentage of the initial \cr
       \code{}        \tab 	face amount that will be returned at maturity \cr
       \code{}        \tab    date. Default value is 100.\cr
       \code{divSch} \tab (Optional) a data frame whose columns are \cr
       \code{}        \tab "Type", "Amount", "Rate", and "Date" \cr 
       \code{}        \tab corresponding to QuantLib's DividendSchedule. \cr
       \code{}        \tab  Default value is an empty frame, or no dividend. \cr
       \code{callSch} \tab (Optional) a data frame whose columns are "Price",\cr
       \code{}        \tab "Type" and "Date" corresponding to QuantLib's \cr
       \code{}        \tab CallabilitySchedule. Defaule is an empty frame, \cr 
       \code{}         \tab or no callability.\cr
     }
   }
   
\item{iborindex}{a DiscountCurve object, represents an IborIndex}
\item{spread}{ a double vector, represents paramter 'spreads' in ConvertibleFloatingBond's constructor. }
\item{coupon}{a double vector of coupon rate}
\item{process}{arguments to construct a BlackScholes process and set up the binomial pricing engine for this bond. 
  \tabular{ll}{
    \code{underlying} \tab a double, flat underlying term structure \cr
    \code{volatility} \tab a double, flat volatility term structure \cr
    \code{dividendYield} \tab a DiscountCurve object \cr
    \code{riskFreeRate} \tab a DiscountCurve object \cr
  }
}

\item{dateparams}{(Optional) a named list, QuantLib's date parameters of the bond. 
  \tabular{ll}{
    \code{settlementDays} \tab (Optional) a double, settlement days. \cr 
    \code{}        \tab Default value is 1.\cr
    \code{calendar} \tab (Optional) a string, either 'us' or 'uk' \cr 
    \code{}        \tab corresponding to US Goverment Bond \cr
    \code{}		   \tab calendar and UK Exchange calendar.\cr
    \code{}        \tab  Default value is 'us'.\cr
    \code{dayCounter} \tab (Optional) a number or string, \cr 
    \code{}        \tab day counter convention.\cr
    \code{}        \tab  See \link{Enum}. Default value is 'Thirty360' \cr 
    \code{period}  \tab (Optional) a number or string, \cr
    \code{}       \tab  interest compounding interval. See \link{Enum}. \cr
    \code{}       \tab Default value is 'Semiannual'.\cr
    \code{businessDayConvention} \tab (Optional) a number or string, \cr 
    \code{}                 \tab business day convention. \cr 
    \tab See \link{Enum}. Default value is 'Following'. \cr
  }
  See the examples below.
}
}
\value{
  The \code{ConvertibleFloatingCouponBond} function returns an object of class
  \code{ConvertibleFloatingCouponBond} (which inherits from class 
  \code{Bond}). It contains a list with the following
  components:
  \item{NPV}{net present value of the bond}
  \item{cleanPrice}{price price of the bond}
  \item{dirtyPrice}{dirty price of the bond}
  \item{accruedAmount}{accrued amount of the bond}
  \item{yield}{yield of the bond}
  \item{cashFlows}{cash flows of the bond}

  The \code{ConvertibleFixedCouponBond} function returns an object of class
  \code{ConvertibleFixedCouponBond} (which inherits from class 
  \code{Bond}). It contains a list with the following
  components:
  \item{NPV}{net present value of the bond}
  \item{cleanPrice}{price price of the bond}
  \item{dirtyPrice}{dirty price of the bond}
  \item{accruedAmount}{accrued amount of the bond}
  \item{yield}{yield of the bond}
  \item{cashFlows}{cash flows of the bond}

  The \code{ConvertibleZeroCouponBond} function returns an object of class
  \code{ConvertibleZeroCouponBond} (which inherits from class 
  \code{Bond}). It contains a list with the following
  components:
  \item{NPV}{net present value of the bond}
  \item{cleanPrice}{price price of the bond}
  \item{dirtyPrice}{dirty price of the bond}
  \item{accruedAmount}{accrued amount of the bond}
  \item{yield}{yield of the bond}
  \item{cashFlows}{cash flows of the bond}
 
}
\details{
 Please see any decent Finance textbook for background reading, and the
  \code{QuantLib} documentation for details on the \code{QuantLib}
  implementation.
}
\references{ 
  \url{http://quantlib.org/} for details on \code{QuantLib}.}
}
\author{Khanh Nguyen \email{knguyen@cs.umb.edu} for the inplementation; Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  the QuantLib Group for \code{QuantLib}
}
\examples{
#this follow an example in test-suite/convertiblebond.cpp 
params <- list(tradeDate=Sys.Date()-2,
               settleDate=Sys.Date(),
               dt=.25,
               interpWhat="discount",
               interpHow="loglinear")


dividendYield <- DiscountCurve(params, list(flat=0.02))
riskFreeRate <- DiscountCurve(params, list(flat=0.05))

dividendSchedule <- data.frame(Type=character(0), Amount=numeric(0),
                            Rate = numeric(0), Date = as.Date(character(0)))
callabilitySchedule <- data.frame(Price = numeric(0), Type=character(0),
                          Date = as.Date(character(0)))

process <- list(underlying=50, divYield = dividendYield,
                rff = riskFreeRate, volatility=0.15)

today <- Sys.Date()
bondparams <- list(exercise="am", faceAmount=100,
                   divSch = dividendSchedule, 
                   callSch = callabilitySchedule,
                   redemption=100,
                   creditSpread=0.005,
                   conversionRatio = 0.0000000001, 
                   issueDate=as.Date(today+2), 
                   maturityDate=as.Date(today+3650))
dateparams <- list(settlementDays=3, 
                   dayCounter="ActualActual", 
                   period = "Semiannual", calendar = "us", 
                   businessDayConvention="Following")

lengths <- c(2,4,6,8,10,12,14,16,18,20,22,24,26,28,30)
coupons <- c( 0.0200, 0.0225, 0.0250, 0.0275, 0.0300,
              0.0325, 0.0350, 0.0375, 0.0400, 0.0425,
              0.0450, 0.0475, 0.0500, 0.0525, 0.0550 )
marketQuotes <- rep(100, length(lengths))
curvedateparams <- list(settlementDays=0, period="Annual", 
                   dayCounter="ActualActual", 
                  businessDayConvention ="Unadjusted")
curveparams <- list(method="ExponentialSplinesFitting", 
                    origDate = Sys.Date())
curve <- FittedBondCurve(curveparams, lengths, coupons, marketQuotes, curvedateparams)
iborindex <- list(type="USDLibor", length=6, 
                  inTermOf="Month", term=curve)   
spreads <- c()
#ConvertibleFloatingCouponBond(bondparams, iborindex, spreads, process, dateparams)


#example using default values
#ConvertibleFloatingCouponBond(bondparams, iborindex,spreads, process)

dateparams <- list(settlementDays=3,                    
                   period = "Semiannual", 
                   businessDayConvention="Unadjusted")

bondparams <- list(                  
                   creditSpread=0.005, conversionRatio = 0.0000000001, 
                   issueDate=as.Date(today+2), 
                   maturityDate=as.Date(today+3650))
#ConvertibleFloatingCouponBond(bondparams, iborindex,
#spreads, process, dateparams)



#this follow an example in test-suite/convertiblebond.cpp 
#for ConvertibleFixedCouponBond

#set up arguments to build a pricing engine.
params <- list(tradeDate=Sys.Date()-2,
               settleDate=Sys.Date(),
               dt=.25,
               interpWhat="discount",
               interpHow="loglinear")
times <- seq(0,10,.1)

dividendYield <- DiscountCurve(params, list(flat=0.02), times)
riskFreeRate <- DiscountCurve(params, list(flat=0.05), times)

dividendSchedule <- data.frame(Type=character(0), Amount=numeric(0),
                            Rate = numeric(0), Date = as.Date(character(0)))
callabilitySchedule <- data.frame(Price = numeric(0), Type=character(0),
                          Date = as.Date(character(0)))

process <- list(underlying=50, divYield = dividendYield,
                rff = riskFreeRate, volatility=0.15)

today <- Sys.Date()
bondparams <- list(exercise="am", faceAmount=100, divSch = dividendSchedule, 
                   callSch = callabilitySchedule, redemption=100, 
                   creditSpread=0.005, conversionRatio = 0.0000000001, 
                   issueDate=as.Date(today+2), 
                   maturityDate=as.Date(today+3650))
dateparams <- list(settlementDays=3, 
                   dayCounter="Actual360", 
                   period = "Once", calendar = "us", 
                   businessDayConvention="Following"
                   )
coupon <- c(0.05)
ConvertibleFixedCouponBond(bondparams, coupon, process, dateparams)

#example with default value
ConvertibleFixedCouponBond(bondparams, coupon, process)

dateparams <- list(settlementDays=3, 
                   dayCounter="Actual360")
ConvertibleFixedCouponBond(bondparams, coupon, process, dateparams)

bondparams <- list(creditSpread=0.005, conversionRatio = 0.0000000001, 
                   issueDate=as.Date(today+2), 
                   maturityDate=as.Date(today+3650))
ConvertibleFixedCouponBond(bondparams, coupon, process, dateparams)



#this follow an example in test-suite/convertiblebond.cpp 
params <- list(tradeDate=Sys.Date()-2,
               settleDate=Sys.Date(),
               dt=.25,
               interpWhat="discount",
               interpHow="loglinear")
times <- seq(0,10,.1)


dividendYield <- DiscountCurve(params, list(flat=0.02), times)
riskFreeRate <- DiscountCurve(params, list(flat=0.05), times)

dividendSchedule <- data.frame(Type=character(0), Amount=numeric(0),
                            Rate = numeric(0), Date = as.Date(character(0)))
callabilitySchedule <- data.frame(Price = numeric(0), Type=character(0),
                          Date = as.Date(character(0)))

process <- list(underlying=50, divYield = dividendYield,
                rff = riskFreeRate, volatility=0.15)

today <- Sys.Date()
bondparams <- list(exercise="am", faceAmount=100, divSch = dividendSchedule, 
                   callSch = callabilitySchedule, redemption=100, 
                   creditSpread=0.005, conversionRatio = 0.0000000001, 
                   issueDate=as.Date(today+2), 
                   maturityDate=as.Date(today+3650))
dateparams <- list(settlementDays=3, 
                   dayCounter="Actual360", 
                   period = "Once", calendar = "us", 
                   businessDayConvention="Following"
                   )

ConvertibleZeroCouponBond(bondparams, process, dateparams)

#example with default values
ConvertibleZeroCouponBond(bondparams, process)


bondparams <- list(creditSpread=0.005,
                   conversionRatio=0.0000000001, 
                   issueDate=as.Date(today+2),
                   maturityDate=as.Date(today+3650))

dateparams <- list(settlementDays=3, dayCounter='Actual360')                   
ConvertibleZeroCouponBond(bondparams, process, dateparams)
ConvertibleZeroCouponBond(bondparams, process)

}
