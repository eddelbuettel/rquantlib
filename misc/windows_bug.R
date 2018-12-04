#!/usr/bin/r
library('RQuantLib')
params <- list(tradeDate=as.Date('2004-09-20'),
               settleDate=as.Date('2004-09-22'),
               dt=.25,
               interpWhat="discount",
               interpHow="loglinear")
setEvaluationDate(as.Date("2004-09-20"))

tsQuotes <- list(d1w = 0.0382,
                 d1m = 0.0372,
                 fut1=96.2875,
                 fut2=96.7875,
                 fut3=96.9875,
                 fut4=96.6875,
                 fut5=96.4875,
                 fut6=96.3875,
                 fut7=96.2875,
                 fut8=96.0875,
                 # s2y = 0.037125,  ## s2y perturbs
                 s3y = 0.0398,
                 s5y = 0.0443,
                 s10y = 0.05165,
                 s15y = 0.055175)
times <- seq(0,10,.1)

setEvaluationDate(params$tradeDate)
discountCurve <- DiscountCurve(params, tsQuotes, times)

# price a zero coupon bond
bondparams <- list(faceAmount=100, issueDate=as.Date("2004-11-30"),
                   maturityDate=as.Date("2008-11-30"), redemption=100 )
dateparams <-list(settlementDays=1,
                  calendar="UnitedStates/GovernmentBond",
                  businessDayConvention=4)
#debugonce(RQuantLib:::ZeroBondWithRebuiltCurve)
res <- ZeroCouponBond(bondparams, discountCurve, dateparams)
print(str(res))
