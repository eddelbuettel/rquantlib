#!/usr/bin/r -t
#
# Copyright (C) 2014 - 2019  Dirk Eddelbuettel
#
# This file is part of RQuantLib.
#
# RQuantLib is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# RQuantLib is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with RQuantLib.  If not, see <http://www.gnu.org/licenses/>.

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
