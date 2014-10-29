
stopifnot(require(RQuantLib))

## values from Quantlib's test-suite
## Reference: Haug, Option Pricing Formulas, McGraw-Hill, 1998
##
## and generally sourced from the code in the test-suite/
## directory of the QuantLib distribution

## europeanoption.cpp:  call value == 2.1334
print(EuropeanOption("call", underlying=60, strike=65, div=0, riskFree=0.08,
                     maturity=0.25, vol=0.3), digits=5)
## europeanoption.cpp:  put value == 2.4648
print(EuropeanOption("put", underlying=100, strike=95, div=0.05, riskFree=0.1,
                     maturity=0.5, vol=0.2), digits=5)

## europeanoption.cpp:  call delta == 0.5946
print(EuropeanOption("call", underlying=105, strike=100,div=0.1,riskFree=0.1,
                     maturity=0.5, vol=0.36), digits=4)
## europeanoption.cpp:  put delta == -0.3566
print(EuropeanOption("put", underlying=105, strike=100,div=0.1,riskFree=0.1,
                     maturity=0.5, vol=0.36), digits=4)

## europeanoption.cpp:  call gamma == 0.0278
print(EuropeanOption("call", underlying=55, strike=60,div=0.0,riskFree=0.1,
                     maturity=0.75, vol=0.30), digits=4)
## europeanoption.cpp:  put gamma == 0.0278
print(EuropeanOption("put", underlying=55, strike=60,div=0.0,riskFree=0.1,
                     maturity=0.75, vol=0.30), digits=4)

## europeanoption.cpp:  call vega == 18.9358
print(EuropeanOption("call", underlying=55, strike=60,div=0.0,riskFree=0.1,
                     maturity=0.75, vol=0.30), digits=4)
## europeanoption.cpp:  put vega == 18.9358
print(EuropeanOption("put", underlying=55, strike=60,div=0.0,riskFree=0.1,
                     maturity=0.75, vol=0.30), digits=4)


## americanoption.cpp:  call value == 10.0089 -- we show 10.00606
print(AmericanOption("call", underlying=110, strike=100, div=0.1, riskFree=0.1,
                     maturity=0.1, vol=0.15), digits=5)
## americanoption.cpp:  put value == 0.3159
print(AmericanOption("call", underlying=90, strike=100, div=0.1, riskFree=0.1,
                     maturity=0.1, vol=0.25), digits=5)


## barrier: down and out call == 9.0246
print(BarrierOption("downout", barrier=95, rebate=3, type="call",
                    strike=90, underlying=100, div=0.04, riskF=0.08,
                    mat=0.5, vol=0.25), digits=4)
## barrier: down and in call == 7.7627
print(BarrierOption("downin", barrier=95, rebate=3, type="call",
                    strike=90, underlying=100, div=0.04, riskF=0.08,
                    mat=0.5, vol=0.25), digits=4)


## binary aka digital: put == 2.6710
print(BinaryOption(binType="cash", type="put", excType="european",
                   strike=80, underl=100, div=0.06, r=0.06,
                   mat=0.75, vol=0.35, cash=10), digits=4)

## asianoption.cpp:  put == 4.6922 (from testAnalyticContinuousGeometricAveragePrice())
print( AsianOption("geometric", "put", underlying=80, strike=85, div=-0.03, riskFree=0.05, maturity=0.25, vol=0.2))

# simple call with unnamed parameters

bond <- list(faceAmount=100,issueDate=as.Date("2004-11-30"),
             maturityDate=as.Date("2008-11-30"), redemption=100 )

dateparams <-list(settlementDays=1, calendar="UnitedStates/GovernmentBond", businessDayConvention='Unadjusted')


discountCurve.param <- list(tradeDate=as.Date('2002-2-15'),
                           settleDate=as.Date('2002-2-15'),
                           dt=0.25,
                           interpWhat='discount', interpHow='loglinear')
discountCurve <- DiscountCurve(discountCurve.param, list(flat=0.05))

ZeroCouponBond(bond, discountCurve, dateparams)


## bond.cpp: examples from Fixed Income page of Matlab
ZeroYield(95, 100, as.Date("1993-6-24"), as.Date("1993-11-1"))

## bond.cpp: test theoretical price of bond by its yield
ZeroPriceByYield(0.1478, 100, as.Date("1993-6-24"), as.Date("1993-11-1"))

## bond.cpp: test theoretical yield of a fixed rate bond, = 0.0307
FixedRateBondYield(,99.282, 100000, as.Date("2004-11-30"), as.Date("2008-11-30"), 3, , c(0.02875), , , , ,as.Date("2004-11-30"))

## bond.cpp: test theoretical price of a fixed rate bond  = 99.2708
FixedRateBondPriceByYield(,0.0307, 100000, as.Date("2004-11-30"), as.Date("2008-11-30"), 3, , c(0.02875), , , , ,as.Date("2004-11-30"))

## bond.cpp

#Simple call with a flat curve
bond <- list(settlementDays=1,
             issueDate=as.Date("2004-11-30"),
             faceAmount=100,
             accrualDayCounter='Thirty360',
             paymentConvention='Unadjusted')
schedule <- list(effectiveDate=as.Date("2004-11-30"),
                 maturityDate=as.Date("2008-11-30"),
                 period='Semiannual',
                 calendar='UnitedStates/GovernmentBond',
                 businessDayConvention='Unadjusted',
                 terminationDateConvention='Unadjusted',
                 dateGeneration='Forward',
                 endOfMonth=1)
calc=list(dayCounter='Actual360',
          compounding='Compounded',
          freq='Annual',
          durationType='Modified')
coupon.rate <- c(0.02875)
params <- list(tradeDate=as.Date('2002-2-15'),
               settleDate=as.Date('2002-2-19'),
               dt=.25,
               interpWhat="discount",
               interpHow="loglinear")
discountCurve.flat <- DiscountCurve(params, list(flat=0.05))
FixedRateBond(bond,
              coupon.rate,
              schedule,
              calc,
              discountCurve=discountCurve.flat)

#Same bond calculated from yield rather than from the discount curve
yield <- 0.02
FixedRateBond(bond,
              coupon.rate,
              schedule,
              calc,
              yield=yield)


#same example with clean price
price <- 103.31
FixedRateBond(bond,
              coupon.rate,
              schedule,
              calc,
              price = price)

## bond.cpp FloatingRateBond, following test-suite/bonds.cpp

bond <- list(faceAmount=100, issueDate=as.Date("2004-11-30"),
             maturityDate=as.Date("2008-11-30"), redemption=100, 
             effectiveDate=as.Date("2004-11-30"))
dateparams <- list(settlementDays=1, calendar="UnitedStates/GovernmentBond",
                   dayCounter = 'ActualActual', period=2, 
                   businessDayConvention = 1, terminationDateConvention=1,
                   dateGeneration=0, endOfMonth=0, fixingDays = 1)

gearings <- spreads <- caps <- floors <- vector()

params <- list(tradeDate=as.Date('2002-2-15'),
               settleDate=as.Date('2002-2-19'),
               dt=.25,
               interpWhat="discount",
               interpHow="loglinear")

tsQuotes <- list(d1w  =0.0382,
                 d1m  =0.0372,
                 fut1=96.2875,
                 fut2=96.7875,
                 fut3=96.9875,
                 fut4=96.6875,
                 fut5=96.4875,
                 fut6=96.3875,
                 fut7=96.2875,
                 fut8=96.0875,
                 s3y  =0.0398,
                 s5y  =0.0443,
                 s10y =0.05165,
                 s15y =0.055175)


## when both discount and libor curves are flat.

discountCurve.flat <- DiscountCurve(params, list(flat=0.05))
termstructure <- DiscountCurve(params, list(flat=0.03))
iborIndex.params <- list(type="USDLibor", length=6, 
                  inTermOf="Month", term=termstructure)                      
FloatingRateBond(bond, gearings, spreads, caps, floors, 
                 iborIndex.params, discountCurve.flat, dateparams)

