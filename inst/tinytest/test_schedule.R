
library(RQuantLib)

#test.Schedule <- function() {
## these tests are taken from the QuantLib test-suite/schedule.cpp file
startDate <- as.Date("2012-1-17")
params <- list(effectiveDate=startDate,
               maturityDate=startDate+7,
               period = "Daily",
               businessDayConvention="Preceding")
expected <- as.Date(c("2012-1-17", "2012-1-18", "2012-1-19", "2012-1-20", "2012-1-23", "2012-1-24"))
expect_equal(Schedule(params), expected, info="Testing schedule with daily frequency...")

params <- list(effectiveDate= as.Date("2009-09-30"),
               maturityDate = as.Date("2012-06-15"),
               calendar = "Japan",
               period = "Semiannual",
               businessDayConvention="Following",
               terminationDateConvention="ModifiedFollowing",
               dateGeneration="Forward",
               endOfMonth=T)
expected <- as.Date(c("2009-09-30", "2010-03-31", "2010-09-30", "2011-03-31", "2011-09-30",
                      "2012-03-30", "2012-06-29"))
expect_equal(Schedule(params), expected, info="Testing end date for schedule with end-of-month adjustment...")

params$terminationDateConvention="Unadjusted"
expected[7] <- as.Date("2012-06-15")
expect_equal(Schedule(params), expected)

params <- list(effectiveDate = as.Date("2013-03-28"),
               maturityDate = as.Date("2015-03-30"),
               calendar = "TARGET",
               period="Annual",
               businessDayConvention="Unadjusted",
               terminationDateConvention="Unadjusted",
               dateGeneration="Forward",
               endOfMonth=T)
expected <- as.Date(c("2013-03-31", "2014-03-31", "2015-03-30"))
expect_equal(Schedule(params), expected, info="Testing that no dates are past the end date with EOM adjustment...")

params <- list(effectiveDate = as.Date("1996-08-31"),
               maturityDate = as.Date("1997-09-15"),
               calendar = "UnitedStates/GovernmentBond",
               period="Semiannual",
               businessDayConvention="Unadjusted",
               terminationDateConvention="Unadjusted",
               dateGeneration="Forward",
               endOfMonth=T)
expected <- as.Date(c("1996-08-31", "1997-02-28", "1997-08-31", "1997-09-15"))
expect_equal(Schedule(params), expected, info=paste("Testing that the last date is not adjusted for EOM",
                                                   "when termination date convention is unadjusted..."))

params <- list(effectiveDate = as.Date("1996-08-22"),
               maturityDate = as.Date("1997-08-31"),
               calendar = "UnitedStates/GovernmentBond",
               period="Semiannual",
               businessDayConvention="Unadjusted",
               terminationDateConvention="Unadjusted",
               dateGeneration="Backward",
               endOfMonth=T)

expected <- as.Date(c("1996-08-22", "1996-08-31", "1997-02-28", "1997-08-31"))
expect_equal(Schedule(params), expected, info=paste("Testing that the first date is not adjusted for EOM",
                                                   "going backward when termination date convention is unadjusted..."))

params <- list(effectiveDate = as.Date("1996-08-22"),
               maturityDate = as.Date("1997-08-31"),
               calendar = "UnitedStates/GovernmentBond",
               period="Semiannual",
               businessDayConvention="Following",
               terminationDateConvention="Following",
               dateGeneration="Backward",
               endOfMonth=T)
expected <- as.Date(c("1996-08-30", "1997-02-28", "1997-08-29"))
expect_equal(Schedule(params), expected, info=paste("Testing that the first date is not duplicated due to",
                                                   "EOM convention when going backwards..."))

if( compareVersion(getQuantLibVersion(), "1.7.1") >= 0 ) {
    params <- list(effectiveDate = as.Date("2016-01-13"),
                   maturityDate = as.Date("2016-05-04"),
                   calendar = "TARGET",
                   period = "EveryFourthWeek",
                   businessDayConvention="Following",
                   terminationDateConvention="Following",
                   dateGeneration="Forward")

    expected <- as.Date(c("2016-01-13", "2016-02-10", "2016-03-09", "2016-04-06", "2016-05-04"))
    expect_equal(Schedule(params), expected, info="Testing that a four-weeks tenor works...")
}


#test.CDS <- function() {
params <- list(effectiveDate = as.Date("2016-06-21"),
               maturityDate = as.Date("2021-06-20"),
               calendar = "UnitedStates/GovernmentBond",
               period = "Quarterly",
               businessDayConvention="ModifiedFollowing",
               terminationDateConvention="Unadjusted",
               dateGeneration="CDS")
## values come from Bloomberg (last date is wrong, because of a bug in QuantLib)
expected <-  as.Date(c("2016-06-20", "2016-09-20", "2016-12-20", "2017-03-20", "2017-06-20",
                       "2017-09-20", "2017-12-20", "2018-03-20", "2018-06-20", "2018-09-20",
                       "2018-12-20", "2019-03-20", "2019-06-20", "2019-09-20", "2019-12-20",
                       "2020-03-20", "2020-06-22", "2020-09-21", "2020-12-21", "2021-03-22",
                       "2021-06-20"))
expect_equal(Schedule(params), expected, info="Checking schedule of IG26")
