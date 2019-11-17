
## test for calendaring functions
##
## cf test-suite/calendars.cpp in the QL sources

library(RQuantLib)

#test.isHoliday <- function() {
expect_true(isHoliday("UnitedStates", as.Date("2004-05-01")), info="isHoliday.US")

#test.isBusinessDay <- function() {
expect_true(isBusinessDay("UnitedStates", as.Date("2004-04-26")), info="isBusinessDay.US")
