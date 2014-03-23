## RQuantLib -- R interface to the QuantLib libraries
##
## Copyright (C) 2002 - 2009 Dirk Eddelbuettel
## Copyright (C) 2010        Dirk Eddelbuettel and Khanh Nguyen
##
## $Id$
##
## This file is part of the RQuantLib library for GNU R.
## It is made available under the terms of the GNU General Public
## License, version 2, or at your option, any later version,
## incorporated herein by reference.
##
## This program is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
## PURPOSE.  See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public
## License along with this program; if not, write to the Free
## Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
## MA 02111-1307, USA

## isBusinessDay <- function(calendar="TARGET", dates=Sys.Date()) {
##     stopifnot(is.character(calendar))
##     stopifnot(class(dates)=="Date")
##     val <- .Call("isBusinessDay", calendar, dates, PACKAGE="RQuantLib")
##     val <- as.logical(val)
##     names(val) <- dates
##     val
## }

businessDay <- function(calendar="TARGET", dates=Sys.Date()) {  ## may get deprecated one day
    isBusinessDay(calendar, dates)
}

## isHoliday <- function(calendar="TARGET", dates=Sys.Date()) {
##     stopifnot(is.character(calendar))
##     stopifnot(class(dates)=="Date")
##     val <- .Call("isHoliday", calendar, dates, PACKAGE="RQuantLib")
##     val <- as.logical(val)
##     names(val) <- dates
##     val
## }

## isWeekend <- function(calendar="TARGET", dates=Sys.Date()) {
##     stopifnot(is.character(calendar))
##     stopifnot(class(dates)=="Date")
##     val <- .Call("isWeekend", calendar, dates, PACKAGE="RQuantLib")
##     val <- as.logical(val)
##     names(val) <- dates
##     val
## }

## isEndOfMonth <- function(calendar="TARGET", dates=Sys.Date()) {
##     stopifnot(is.character(calendar))
##     stopifnot(class(dates)=="Date")
##     val <- .Call("isEndOfMonth", calendar, dates, PACKAGE="RQuantLib")
##     val <- as.logical(val)
##     names(val) <- dates
##     val
## }

## getEndOfMonth <- function(calendar="TARGET", dates=Sys.Date()) {
##     stopifnot(is.character(calendar))
##     stopifnot(class(dates)=="Date")
##     val <- .Call("endOfMonth", calendar, dates, PACKAGE="RQuantLib")
##     names(val) <- dates
##     val
## }

endOfMonth <- function(calendar="TARGET", dates=Sys.Date()) {
    getEndOfMonth(calendar, dates)
}

## adjust <- function(calendar="TARGET", dates=Sys.Date(), bdc = 0 ) {
##     stopifnot(is.character(calendar))
##     stopifnot(class(dates)=="Date")
##     val <- .Call("adjust", calendar, as.double(bdc), dates, PACKAGE="RQuantLib")
##     names(val) <- dates
##     val
## }

advance <- function(calendar="TARGET", dates=Sys.Date(),
                    n, timeUnit, # call 1
                    period,      # call 2
                    bdc = 0, emr = 0) {
    stopifnot(is.character(calendar))
    stopifnot(class(dates)=="Date")
    call1 <- missing(period) && !missing(n) && !missing(timeUnit)  
    call2 <- !missing(period) && missing(n) && missing(timeUnit)
    stopifnot(call1 | call2)
    val <- NULL
    if (call1) {
        ## val <- .Call("advance1",
        ##              calendar,
        ##              list(amount = as.double(n),
        ##                   unit = as.double(timeUnit),
        ##                   bdc = as.double(bdc),
        ##                   emr = as.double(emr)),
        ##              dates,
        ##              PACKAGE="RQuantLib")
        val <- advance1(calendar, n, timeUnit, bdc, emr, dates)
    }
    if (call2) {
        ## val <- .Call("advance2",
        ##              calendar,
        ##              list(period = as.double(period),
        ##                   bdc = as.double(bdc),
        ##                   emr = as.double(emr)),
        ##              dates,
        ##              PACKAGE="RQuantLib")
        val <- advance2(calendar, period, bdc, emr, dates)
    }
    stopifnot( !is.null(val) )
    val
}

## businessDaysBetween  <- function(calendar="TARGET",
##                                  from=Sys.Date(),
##                                  to = Sys.Date() + 5,
##                                  includeFirst = 1,
##                                  includeLast = 0
##                                  ) {
##     stopifnot(is.character(calendar))
##     stopifnot(class(from)=="Date")
##     stopifnot(class(to)=="Date")
##     val <- .Call("businessDaysBetween",
##                  calendar,
##                  list(includeFirst = as.double(includeFirst),
##                       includeLast = as.double(includeLast)),
##                  from, to,
##                  PACKAGE="RQuantLib")
##     val <- val
##     val
## }

## getHolidayList <- function(calendar="TARGET",
##                            from=Sys.Date(),
##                            to=Sys.Date() + 5,
##                            includeWeekends=0) {
##     stopifnot(is.character(calendar))
##     stopifnot(class(from)=="Date")
##     stopifnot(class(to)=="Date")
##     val <- .Call("holidayList",
##                  calendar,
##                  list(includeWeekends=as.double(includeWeekends), from=from, to=to),
##                  PACKAGE="RQuantLib")
##     val
## }
 
holidayList <- function(calendar="TARGET", from=Sys.Date(), to=Sys.Date() + 5, includeWeekends=FALSE) {
    getHolidayList(calendar, from, to, includeWeekends)
}

#setCalendarContext <- function(calendar="TARGET",
#                               fixingDays = 2,
#                               settleDate = Sys.Date() + 2) {
#    val <- .Call("setContext",
#                 list(calendar = calendar,
#                      fixingDays = fixingDays,
#                      settleDate = settleDate),
#                 PACKAGE="RQuantLib")
#}
