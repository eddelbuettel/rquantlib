
## test for calendaring functions
##
## cf test-suite/calendars.cpp in the QL sources

test.isHoliday <- function() {
    checkEquals(isHoliday("UnitedStates", as.Date("2004-05-01")), TRUE, msg="isHoliday.US")
}

test.isBusinessDay <- function() {
    checkEquals(isBusinessDay("UnitedStates", as.Date("2004-04-26")), TRUE, msg="isBusinessDay.US")
}
