
.onWindows <- .Platform$OS.type == "windows"

if (.onWindows) exit_file("Skipping dates test on Windows.")

Rcpp::sourceCpp("cpp/dates.cpp")

#test.date.conversion <- function() {
given <- as.Date("2000-01-01")
inc <- 2
expected <- given + inc

expect_equal(advanceDateRR(given, inc), expected, info="date conversion from R to R")
expect_equal(advanceDateQR(given, inc), expected, info="date conversion from QuantLib to R")
expect_equal(advanceDateQQ(given, inc), expected, info="date conversion from QuantLib to QuantLib")

#test.datevector.conversion <- function() {
given <- as.Date("2000-01-01") + 0:3
inc <- 2
expected <- given + inc

expect_equal(advanceDatesRR(given, inc), expected, info="date conversion from R to R")
expect_equal(advanceDatesQR(given, inc), expected, info="date conversion from QuantLib to QuantLib")
expect_equal(advanceDatesQQ(given, inc), expected, info="date conversion from QuantLib to R")
