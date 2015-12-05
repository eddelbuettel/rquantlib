
calendar <- "TARGET"

settlementDate <- as.Date("2008-09-18")
settlementDate <- adjust(calendar, settlementDate)

fixingDays <- 3
settlementDays <- 3
todaysDate <- advance(calendar, settlementDate, n=-fixingDays, timeUnit=0)
setEvaluationDate(todaysDate)

cat('Today          : ', format(todaysDate), "\n")
cat('Settlement Date: ', format(settlementDate), "\n")

## market quotes
## constructing bond yield curve
zcQuotes <- c(d3m = 0.0096,   # Period(3, "Months")
              d6m = 0.0145,   # Period(6, "Months")
              d1y = 0.0194)   # Period(1, "Years")

zcBondsDayCounter <- RQuantLib:::matchDayCounter("ActualFixed")

#bondInstruments <- RateHelperVector()
bondInstruments <- rep(NA, 3)
for (i in 1:3) {
    rate <- zcQuotes[i]
    tenor <- names(zcQuotes)[i]
    drh <- DepositRateHelper(r, #QuoteHandle(SimpleQuote(r)),
                             tenor,
                             fixingDays,
                             calendar,
                             "ModifiedFollowing",
                             TRUE,
                             zcBondsDayCounter)
    RateHelperVector_push_back(bondInstruments, drh)
    
}
