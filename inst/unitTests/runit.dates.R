#!/usr/bin/r -t
#
# Copyright (C) 2014  Dirk Eddelbuettel
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

if (!.onWindows) {
    .setUp <- RQuantLib:::unitTestSetup("dates.cpp")
}

test.date.conversion <- function() { 
    if (!.onWindows) {
        given <- as.Date("2000-01-01")
        inc <- 2
        expected <- given + inc

        checkEquals(advanceDateRR(given, inc), expected, msg="date conversion from R to R")
        checkEquals(advanceDateQR(given, inc), expected, msg="date conversion from QuantLib to R")
        checkEquals(advanceDateQQ(given, inc), expected, msg="date conversion from QuantLib to QuantLib")
    }
}

test.datevector.conversion <- function() { 
    if (!.onWindows) {
        given <- as.Date("2000-01-01") + 0:3
        inc <- 2
        expected <- given + inc

        checkEquals(advanceDatesRR(given, inc), expected, msg="date conversion from R to R")
        checkEquals(advanceDatesQR(given, inc), expected, msg="date conversion from QuantLib to QuantLib")
        checkEquals(advanceDatesQQ(given, inc), expected, msg="date conversion from QuantLib to R")
    }
}
