## RQuantLib -- R interface to the QuantLib libraries
##
##  Copyright (C) 2002 - 2014  Dirk Eddelbuettel <edd@debian.org>
##  Copyright (C) 2009         Khanh Nguyen <knguyen@cs.umb.edu>
##  Copyright (C) 2010 - 2011  Dirk Eddelbuettel and Khanh Nguyen
##
##  This file is part of RQuantLib.
##
##  RQuantLib is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 2 of the License, or
##  (at your option) any later version.
##
##  RQuantLib is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with RQuantLib.  If not, see <http://www.gnu.org/licenses/>.

ZeroCouponBond <- function(bond, discountCurve, dateparams ) {
    UseMethod("ZeroCouponBond")
}

## TODO: redo interface here
ZeroCouponBond.default <- function(bond,
                                   discountCurve,
                                   dateparams=list(refDate=bond$issueDate,
                                   settlementDays=1,
                                   calendar='UnitedStates/GovernmentBond',
                                   businessDayConvention='Following')) {
    val <- 0

    if (is.null(bond$faceAmount)) bond$faceAmount <- 100
    if (is.null(bond$redemption)) bond$redemption <- 100

    if (is.null(dateparams$settlementDays)) dateparams$settlementDays <- 1
    if (is.null(dateparams$calendar)) dateparams$calendar <- 'UnitedStates/GovernmentBond'
    if (is.null(dateparams$businessDayConvention)) dateparams$businessDayConvention <- 'Following'
    if (is.null(dateparams$refDate)) dateparams$refDate <- bond$issueDate
    dateparams <- matchParams(dateparams)


    val <- ZeroBondWithRebuiltCurve(bond, c(discountCurve$table$date),
                                    discountCurve$table$zeroRates, dateparams)
    class(val) <- c("ZeroCouponBond", "Bond")
    val
}



ZeroPriceByYield <- function(yield, faceAmount,
                             issueDate, maturityDate,
                             dayCounter, frequency,
                             compound, businessDayConvention){
    UseMethod("ZeroPriceByYield")
}

ZeroPriceByYield.default <- function(yield, faceAmount=100,
                                     issueDate, maturityDate,
                                     dayCounter=2, frequency=2,
                                     compound=0, businessDayConvention=4) {

    val <- zeroPriceByYieldEngine(yield, faceAmount, dayCounter,
                                  frequency, businessDayConvention,
                                  compound, maturityDate, issueDate)
    class(val) <- c("ZeroPriceByYield")
    val
}

ZeroYield <- function(price, faceAmount, issueDate, maturityDate,
                      dayCounter, frequency, compound,	businessDayConvention) {
    UseMethod("ZeroYield")
}

ZeroYield.default <- function(price, faceAmount=100,
                              issueDate, maturityDate,
                              dayCounter=2, frequency=2,
                              compound=0, businessDayConvention=4) {

    val <- zeroYieldByPriceEngine(price, faceAmount, dayCounter, frequency,
                                  businessDayConvention, compound,
                                  maturityDate, issueDate)
    class(val) <- c("ZeroYield")
    val
}



FixedRateBond <- function(bond, rates, schedule, calc, discountCurve, yield, price){
    UseMethod("FixedRateBond")
}
FixedRateBond.default <- function(bond = list(),
                                  rates,
                                  schedule,
                                  calc=list(
                                    dayCounter='ActualActual.ISMA',
                                    compounding='Compounded',
                                    freq='Annual',
                                    durationType='Modified'),
                                  discountCurve = NULL,
                                  yield = NA,
                                  price = NA){
    val <- 0

    # check bond params
    if (is.null(bond$settlementDays)) bond$settlementDays <- 1
    if (is.null(bond$faceAmount)) bond$faceAmount <- 100
    if (is.null(bond$dayCounter)) bond$dayCounter <- 'Thirty360'
    # additional parameters have default values on cpp side (see QuantLib::FixedRateBond first ctor)
    # paymentConvention
    # redemption
    # issueDate
    # paymentCalendar
    # exCouponPeriod
    # exCouponCalendar
    # exCouponConvention
    # exCouponEndOfMonth
    bond <- matchParams(bond)
    
    # check schedule params
    if (is.null(schedule$effectiveDate)){
      stop("schedule effective date undefined.")
    }
    if (is.null(schedule$maturityDate)){
      stop("schedule maturity date undefined.")
    }
    if (is.null(schedule$period)) schedule$period <- 'Semiannual'
    if (is.null(schedule$calendar)) schedule$calendar <- 'TARGET'
    if (is.null(schedule$businessDayConvention)) schedule$businessDayConvention <- 'Following'
    if (is.null(schedule$terminationDateConvention)) schedule$terminationDateConvention <- 'Following'
    if (is.null(schedule$dateGeneration)) schedule$dateGeneration <- 'Backward'
    if (is.null(schedule$endOfMonth)) schedule$endOfMonth <- 0
    schedule <- matchParams(schedule)
    
    # check calc params
    if (is.null(calc$dayCounter)) calc$dayCounter <- 'ActualActual.ISMA'
    if (is.null(calc$compounding)) calc$compounding <- 'Compounded'
    if (is.null(calc$freq)) calc$freq <- 'Annual'
    if (is.null(calc$durationType)) calc$durationType <- 'Simple'
    if (is.null(calc$accuracy)) calc$accuracy <- 1.0e-8
    if (is.null(calc$maxEvaluations)) calc$maxEvaluations <- 100
    calc <- matchParams(calc)

    which.calc <- !c(is.null(discountCurve), is.na(yield), is.na(price))
    if (sum(which.calc) != 1)
        stop("one and only one of discountCurve, yield or price must be defined.")
    
    if (!is.null(discountCurve)) {
        val <- FixedRateWithRebuiltCurve(
            bond, rates, schedule, calc, c(discountCurve$table$date), discountCurve$table$zeroRates)
      
    } else if (!is.na(yield)) {
      val <- FixedRateWithYield(bond, rates, schedule, calc, yield)
      
    } else if (!is.na(price)) {
      val <- FixedRateWithPrice(bond, rates, schedule, calc, price)
    }

    class(val) <- c("FixedRateBond", "Bond")
    val
}


FixedRateBondYield <- function(settlementDays, price, faceAmount,
                               effectiveDate, maturityDate,
                               period, calendar, rates,
                               dayCounter, businessDayConvention,
                               compound, redemption, issueDate) {
     UseMethod("FixedRateBondYield")
}
FixedRateBondYield.default <- function(settlementDays = 1, price, faceAmount=100,
                                       effectiveDate, maturityDate,
                                       period, calendar = "UnitedStates/GovernmentBond", rates,
                                       dayCounter=2, businessDayConvention=0,
                                       compound = 0, redemption = 100, issueDate) {

    val <- fixedRateBondYieldByPriceEngine(settlementDays, price, calendar, faceAmount,
                                           businessDayConvention,
                                           compound, redemption, dayCounter,
                                           period, 	## aka frequency
                                           maturityDate, issueDate, effectiveDate,
                                           rates)
    class(val) <- c("FixedRateBondYield")
    val
}


FixedRateBondPriceByYield <- function(settlementDays, yield, faceAmount,
                                      effectiveDate, maturityDate,
                                      period, calendar, rates,
                                      dayCounter, businessDayConvention,
                                      compound, redemption, issueDate) {
    UseMethod("FixedRateBondPriceByYield")
}

FixedRateBondPriceByYield.default <- function(settlementDays = 1, yield, faceAmount=100,
                                              effectiveDate=issueDate, maturityDate,
                                              period, calendar = "UnitedStates/GovernmentBond", rates,
                                              dayCounter=2, businessDayConvention=0,
                                              compound = 0, redemption = 100, issueDate) {
    val <- fixedRateBondPriceByYieldEngine(settlementDays, yield, calendar, faceAmount,
                                           businessDayConvention,
                                           compound, redemption, dayCounter, period,
                                           maturityDate, issueDate, effectiveDate,
                                           rates)
    class(val) <- c("FixedRateBondPriceByYield")
    val
}


FloatingRateBond <- function(bond, gearings, spreads, caps, floors,
                             index, curve, dateparams){
    UseMethod("FloatingRateBond")
}

FloatingRateBond.default <- function(bond,
                                     gearings=vector(),
                                     spreads=vector(),
                                     caps=vector(),
                                     floors=vector(),
                                     index,
                                     curve,
                                     dateparams=list(refDate=bond$issueDate-2,
                                                     settlementDays=1,
                                                     calendar='UnitedStates/GovernmentBond',
                                                     businessDayConvention='Following',
                                                     terminationDateConvention='Following',
                                                     dayCounter='Thirty360',
                                                     period='Semiannual',
                                                     dateGeneration='Backward',
                                                     endOfMonth=0,
                                                     fixingDays=2)
                                     ) {
    val <- 0

    if (is.null(bond$faceAmount)) bond$faceAmount <- 100
    if (is.null(bond$redemption)) bond$redemption <- 100
    if (is.null(bond$effectiveDate)) bond$effectiveDate <- bond$issueDate


    if (is.null(dateparams$settlementDays)) dateparams$settlementDays <- 1
    if (is.null(dateparams$calendar)) dateparams$calendar <- 'UnitedStates/GovernmentBond'
    if (is.null(dateparams$businessDayConvention)) dateparams$businessDayConvention <- 'Following'
    if (is.null(dateparams$terminationDateConvention)) dateparams$terminationDateConvention <- 'Following'
    if (is.null(dateparams$dayCounter)) dateparams$dayCounter <- 'Thirty360'
    if (is.null(dateparams$period)) dateparams$period <- 'Semiannual'
    if (is.null(dateparams$dateGeneration)) dateparams$dateGeneration <- 'Backward'
    if (is.null(dateparams$endOfMonth)) dateparams$endOfMonth <- 0
    if (is.null(dateparams$fixingDays)) dateparams$fixingDays <- 2
    if (is.null(dateparams$refDate))  dateparams$refDate <- bond$issueDate-2

    dateparams <- matchParams(dateparams)

    indexparams <- list(type=index$type, length=index$length, inTermOf=index$inTermOf)
    ibor <- index$term
    val <- floatingWithRebuiltCurveEngine(bond, gearings, spreads, caps, floors, indexparams,
                                          c(ibor$table$date), ibor$table$zeroRates,
                                          c(curve$table$date), curve$table$zeroRates,
                                          dateparams)
    class(val) <- c("FloatingRateBond", "Bond")
    val

}


ConvertibleZeroCouponBond <- function(bondparams, process, dateparams){
    UseMethod("ConvertibleZeroCouponBond")
}

ConvertibleZeroCouponBond.default <- function(bondparams,
                                              process,
                                              dateparams=list(
                                              settlementDays=1,
                                              calendar='UnitedStates/GovernmentBond',
                                              dayCounter='Thirty360',
                                                period='Semiannual',
                                                businessDayConvention='Following'
                                              )
                                              ) {
    val <- 0

    if (is.null(bondparams$exercise)) bondparams$exercise <- 'am'
    if (is.null(bondparams$faceAmount)) bondparams$faceAmount <- 100
    if (is.null(bondparams$redemption)) bondparams$redemption <- 100
    if (is.null(bondparams$divSch)) {
        bondparams$divSch <- data.frame(Type=character(0), Amount=numeric(0),
        Rate <- numeric(0), Date = as.Date(character(0)))
    }
    if (is.null(bondparams$callSch)){
        bondparams$callSch <- data.frame(Price=numeric(0), Type=character(0),
        Date <- as.Date(character(0)))
    }


    if (is.null(dateparams$settlementDays)) dateparams$settlementDays <- 1
    if (is.null(dateparams$calendar)) dateparams$calendar <- 'UnitedStates/GovernmentBond'
    if (is.null(dateparams$businessDayConvention)) dateparams$businessDayConvention <- 'Following'
    if (is.null(dateparams$dayCounter)) dateparams$dayCounter <- 'Thirty360'
    if (is.null(dateparams$period)) dateparams$period <- 'Semiannual'

    dateparams <- matchParams(dateparams)
    callabilitySchedule <- bondparams$callSch
    dividendSchedule <- bondparams$divSch
    dividendYield <- process$divYield
    riskFreeRate <- process$rff
    val <- convertibleZeroBondEngine(bondparams, process,
                                     c(dividendYield$table$date),
                                     dividendYield$table$zeroRates,
                                     c(riskFreeRate$table$date),
                                     riskFreeRate$table$zeroRates,
                                     dividendSchedule, callabilitySchedule, dateparams)
    class(val) <- c("ConvertibleZeroCouponBond", "Bond")
    val
}


ConvertibleFixedCouponBond <- function(bondparams, coupon, process, dateparams){
    UseMethod("ConvertibleFixedCouponBond")
}

ConvertibleFixedCouponBond.default <- function(bondparams,
                                               coupon,
                                               process,
                                               dateparams=list(
                                                 settlementDays=1,
                                                 calendar='UnitedStates/GovernmentBond',
                                                 dayCounter='Thirty360',
                                                 period='Semiannual',
                                                 businessDayConvention='Following'
                                                 )
                                               ){

    val <- 0

    if (is.null(bondparams$exercise)) bondparams$exercise <- 'am'
    if (is.null(bondparams$faceAmount)) bondparams$faceAmount <- 100
    if (is.null(bondparams$redemption)) bondparams$redemption <- 100
    if (is.null(bondparams$divSch)) {
      bondparams$divSch <- data.frame(Type=character(0), Amount=numeric(0),
                                      Rate = numeric(0), Date = as.Date(character(0)))
    }
    if (is.null(bondparams$callSch)) {
      bondparams$callSch <- data.frame(Price=numeric(0), Type=character(0),
                                       Date=as.Date(character(0)))
    }

    if (is.null(dateparams$settlementDays)) dateparams$settlementDays <- 1
    if (is.null(dateparams$calendar)) dateparams$calendar <- 'UnitedStates/GovernmentBond'
    if (is.null(dateparams$businessDayConvention)) dateparams$businessDayConvention <- 'Following'
    if (is.null(dateparams$dayCounter)) dateparams$dayCounter <- 'Thirty360'
    if (is.null(dateparams$period)) dateparams$period <- 'Semiannual'

    dateparams <- matchParams(dateparams)
    callabilitySchedule <- bondparams$callSch
    dividendSchedule <- bondparams$divSch
    dividendYield <- process$divYield
    riskFreeRate <- process$rff
    val <- convertibleFixedBondEngine(bondparams, coupon, process,
                                      c(dividendYield$table$date),
                                      dividendYield$table$zeroRates,
                                      c(riskFreeRate$table$date),
                                      riskFreeRate$table$zeroRates,
                                      dividendSchedule, callabilitySchedule, dateparams)
    class(val) <- c("ConvertibleFixedCouponBond", "Bond")
    val
}

ConvertibleFloatingCouponBond <- function(bondparams, iborindex,spread, process, dateparams){
    UseMethod("ConvertibleFloatingCouponBond")
}

ConvertibleFloatingCouponBond.default <- function(bondparams,
                                                  iborindex,
                                                  spread,
                                                  process,
                                                  dateparams=list(
                                                    settlementDays=1,
                                                    calendar='UnitedStates/GovernmentBond',
                                                    dayCounter='Thirty360',
                                                    period='Semiannual',
                                                    businessDayConvention='Following'
                                                    )){
    val <- 0

    if (is.null(bondparams$exercise)) bondparams$exercise <- 'am'
    if (is.null(bondparams$faceAmount)) bondparams$faceAmount <- 100
    if (is.null(bondparams$redemption)) bondparams$redemption <- 100
    if (is.null(bondparams$divSch)) {
      bondparams$divSch <- data.frame(Type=character(0), Amount=numeric(0),
                                      Rate = numeric(0), Date = as.Date(character(0)))
    }
    if (is.null(bondparams$callSch)) {
        bondparams$callSch <- data.frame(Price=numeric(0), Type=character(0),
                                         Date=as.Date(character(0)))
    }


    if (is.null(dateparams$settlementDays)) dateparams$settlementDays <- 1
    if (is.null(dateparams$calendar)) dateparams$calendar <- 'UnitedStates/GovernmentBond'
    if (is.null(dateparams$businessDayConvention)) dateparams$businessDayConvention <- 'Following'
    if (is.null(dateparams$dayCounter)) dateparams$dayCounter <- 'Thirty360'
    if (is.null(dateparams$period)) dateparams$period <- 'Semiannual'


    dateparams <- matchParams(dateparams)
    callabilitySchedule <- bondparams$callSch
    dividendSchedule <- bondparams$divSch
    dividendYield <- process$divYield
    riskFreeRate <- process$rff

    indexparams <- list(type=iborindex$type, length=iborindex$length,
                        inTermOf=iborindex$inTermOf)
    ibor <- iborindex$term

    val <- convertibleFloatingBondEngine(bondparams,  process,
                                         c(dividendYield$table$date),
                                         dividendYield$table$zeroRates,
                                         c(riskFreeRate$table$date),
                                         riskFreeRate$table$zeroRates,
                                         c(ibor$table$date),
                                         ibor$table$zeroRates,
                                         indexparams,spread,
                                         dividendSchedule, callabilitySchedule,
                                         dateparams)
    class(val) <- c("ConvertibleFloatingCouponBond", "Bond")
    val
}

CallableBond <- function(bondparams, hullWhite, coupon, dateparams){
    UseMethod("CallableBond")
}

CallableBond.default <- function(bondparams, hullWhite,
                                 coupon,
                                 dateparams=list(
                                   settlementDays=1,
                                   calendar='UnitedStates/GovernmentBond',
                                   dayCounter='Thirty360',
                                   period='Semiannual',
                                   businessDayConvention='Following',
                                   terminationDateConvention='Following'
                                 )){
    val <- 0

    if (is.null(bondparams$faceAmount)) bondparams$faceAmount <- 100
    if (is.null(bondparams$redemption)) bondparams$redemption <- 100
    if (is.null(bondparams$callSch)) {
        bondparams$callSch <- data.frame(Price=numeric(0), Type=character(0),
                                         Date=as.Date(character(0)))
    }

    if (is.null(dateparams$settlementDays)) dateparams$settlementDays <- 1
    if (is.null(dateparams$calendar)) dateparams$calendar <- 'UnitedStates/GovernmentBond'
    if (is.null(dateparams$businessDayConvention)) dateparams$businessDayConvention <- 'Following'
    if (is.null(dateparams$terminationDateConvention)) dateparams$terminationDateConvention <- 'Following'
    if (is.null(dateparams$dayCounter)) dateparams$dayCounter <- 'Thirty360'
    if (is.null(dateparams$period)) dateparams$period <- 'Semiannual'

    dateparams <- matchParams(dateparams)
    callSch <- bondparams$callSch
#    hw.termStructure <- hullWhite$term

    val <- callableBondEngine(bondparams, hullWhite,coupon,
                              ## c(hw.termStructure$table$date),
                              ## hw.termStructure$table$zeroRates,
                              callSch, dateparams)
    class(val) <- c("CallableBond", "Bond")
    val
}

FittedBondCurve <- function(curveparams,
                            lengths,
                            coupons,
                            marketQuotes,
                            dateparams){
    UseMethod("FittedBondCurve")
}

FittedBondCurve.default <- function(curveparams,
                                    lengths,
                                    coupons,
                                    marketQuotes,
                                    dateparams){
    val <- 0
    dateparams <- matchParams(dateparams)
    val <- fittedBondCurveEngine(curveparams, lengths, coupons, marketQuotes, dateparams)
    class(val) <- c("DiscountCurve")
    val
}


#CMSBond <- function(bondparams, swapIndex, cap, floor, gearings, spreads
#                    pricer, iborIndex){
#   UseMethod("CMSBond")
#}

#CMSBond.default <- function(bondparams, iborIndex, swapIndex, cap, floor, gearings,
#                spreads, pricer){
#   val <- 0
#   swaptionVol <- pricer$swaptionVol
#   atmOptionTenors <- swaptionVol$atmOptionTenors
#   atmSwapTenors <- swaptionVol$atmSwapTenors
#   volMatrix <- swaptionVol$volatilityMatrix
#   swapIndex <- matchParams(swapIndex)
#   ibor <- iborIndex$term
#   val <- .Call("CMSBond", bondparams, iborIndex, swapIndex, cap, floor, gearings, spreads,
#                swaptionVol, atmOptionTenors, atmSwapTenors, volMatrix, pricer
#                ibor$table$dates, ibor$table$zeroRates)
#}

# matching functions

matchDayCounter <- function(daycounter = c("Actual360", "ActualFixed", "ActualActual",
                 "Business252", "OneDayCounter", "SimpleDayCounter", "Thirty360", "Actual365NoLeap",
                 "ActualActual.ISMA", "ActualActual.Bond", "ActualActual.ISDA", "ActualActual.Historical", "ActualActual.AFB", "ActualActual.Euro"))
{
  
     if (!is.numeric(daycounter)) {
         daycounter <- match.arg(daycounter)
         daycounter <- switch(daycounter,
                              Actual360 = 0,
                              ActualFixed = 1,
                              ActualActual = 2,
                              Business252 = 3,
                              OneDayCounter = 4,
                              SimpleDayCounter = 5,
                              Thirty360 = 6,
                              Actual365NoLeap = 7,
                              ActualActual.ISMA = 8,
                              ActualActual.Bond = 9,
                              ActualActual.ISDA = 10,
                              ActualActual.Historical = 11,
                              ActualActual.AFB = 12,
                              ActualActual.Euro = 13)
     }
     daycounter
}

matchBDC <- function(bdc = c("Following", "ModifiedFollowing",
                             "Preceding", "ModifiedPreceding",
                             "Unadjusted")) {
     if (!is.numeric(bdc)){
         bdc <- match.arg(bdc)
         bdc <- switch(bdc,
                       Following = 0,
                       ModifiedFollowing = 1,
                       Preceding = 2,
                       ModifiedPreceding = 3,
                       Unadjusted = 4)
     }
     bdc
}

matchCompounding <- function(cp = c("Simple", "Compounded",
                                    "Continuous", "SimpleThenCompounded")) {
     if (!is.numeric(cp)){
        cp <- match.arg(cp)
        cp <- switch(cp,
                     Simple = 0,
                     Compounded = 1,
                     Continuous = 2,
                     SimpleThenCompounded = 3)
     }
     cp
}
matchFrequency <- function(freq = c("NoFrequency","Once", "Annual",
                                    "Semiannual", "EveryFourthMonth",
                                    "Quarterly", "Bimonthly", "Monthly",
                                    "EveryFourthWeek", "Biweekly",
                                    "Weekly", "Daily")) {
    if (!is.numeric(freq)){
       freq <- match.arg(freq)
       freq <- switch(freq,
                      NoFrequency = -1, Once = 0, Annual = 1,
                      Semiannual = 2, EveryFourthMonth = 3,
                      Quarterly = 4, Bimonthly = 6,
                      Monthly = 12, EveryFourthWeek = 13,
                      Biweekly = 26, Weekly = 52, Daily = 365)
    }
    freq
}
matchDateGen <- function(dg = c("Backward", "Forward", "Zero",
                                "ThirdWednesday", "Twentieth",
                                "TwentiethIMM")){
   if (!is.numeric(dg)){
      dg <- match.arg(dg)
      dg <- switch(dg,
                   Backward = 0, Forward = 1,
                   Zero = 2, ThirdWednesday = 3,
                   Twentieth = 4, TwentiethIMM = 5)
   }
   dg
}


matchDurationType <- function(dt = c("Simple", "Macaulay", "Modified")) {
  if (!is.numeric(dt)){
    dt <- match.arg(dt)
    dt <- switch(dt,
                 Simple = 0,
                 Macaulay = 1,
                 Modified = 2)
  }
  dt
}


matchParams <- function(params) {

    if (!is.null(params$dayCounter)) {
        params$dayCounter <- matchDayCounter(params$dayCounter)
    }
    if (!is.null(params$compounding)) {
      params$compounding <- matchCompounding(params$compounding)
    }
    if (!is.null(params$period)) {
        params$period <- matchFrequency(params$period)
    }
    if (!is.null(params$freq)) {
      params$freq <- matchFrequency(params$freq)
    }
    if (!is.null(params$businessDayConvention)) {
        params$businessDayConvention <- matchBDC(params$businessDayConvention)
    }
    if (!is.null(params$terminationDateConvention)) {
        params$terminationDateConvention <- matchBDC(params$terminationDateConvention)
    }
    if (!is.null(params$paymentConvention)) {
      params$paymentConvention <- matchBDC(params$paymentConvention)
    }
    if (!is.null(params$exCouponConvention)) {
      params$exCouponConvention <- matchBDC(params$exCouponConvention)
    }
    if (!is.null(params$dateGeneration)) {
        params$dateGeneration <- matchDateGen(params$dateGeneration)
    }
    if (!is.null(params$fixedLegConvention)) {
        params$fixedLegConvention <- matchBDC(params$fixedLegConvention)
    }
    if (!is.null(params$fixedLegDayCounter)) {
        params$fixedLegDayCounter <- matchDayCounter(params$fixedLegDayCounter)
    }
    if (!is.null(params$durationType)) {
      params$durationType <- matchDurationType(params$durationType)
    }
    params

}

# Generic methods
plot.Bond <- function(x, ...) {
    warning("No plotting available for class", class(x)[1],"\n")
    invisible(x)
}

print.Bond <- function(x, digits=5, ...) {
    cat("Concise summary of valuation for", class(x)[1], "\n")
    cat(" Net present value : ", format(x$NPV), "\n")
    cat("       clean price : ", format(x$cleanPrice, digits=digits), "\n")
    cat("       dirty price : ", format(x$dirtyPrice, digits=digits), "\n")
    cat("    accrued coupon : ", format(x$accruedCoupon, digits=digits), "\n")
    cat("             yield : ", format(x$yield, digits=digits), "\n")
    cat("        cash flows : \n")
    print(x$cashFlow, row.names=FALSE, digits=digits)
    #print(round(unlist(x[1:5]), digits))
    invisible(x)
}

print.FixedRateBond <- function(x, digits=5, ...) {
  cat("Concise summary of valuation for", class(x)[1], "\n")
  cat(" Net present value : ", format(x$NPV), "\n")
  cat("       clean price : ", format(x$cleanPrice, digits=digits), "\n")
  cat("       dirty price : ", format(x$dirtyPrice, digits=digits), "\n")
  cat("    accrued coupon : ", format(x$accruedCoupon, digits=digits), "\n")
  cat("             yield : ", format(x$yield, digits=digits), "\n")
  cat("          duration : ", format(x$duration, digits=digits), "\n")
  cat("   settlement date : ", format(x$settlementDate, format="%Y-%m-%d"), "\n")
  cat("        cash flows : \n")
  print(x$cashFlow, row.names=FALSE, digits=digits)
  #print(round(unlist(x[1:5]), digits))
  invisible(x)
}

summary.Bond <- function(object, digits=5, ...) {
    cat("Detailed summary of valuation for", class(object)[1], "\n")
    print(round(unlist(object[1:5]), digits))
    cat("with parameters\n")
    print(unlist(object[["parameters"]]))
    invisible(object)
}
