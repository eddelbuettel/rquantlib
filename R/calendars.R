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

businessDay <- function(calendar="TARGET", dates=Sys.Date()) {
  stopifnot(is.character(calendar))
  stopifnot(class(dates)=="Date")
  val <- .Call("QL_isBusinessDay",
               calendar, dates,
               PACKAGE="RQuantLib")
  val <- as.logical(val[[1]])
  names(val) <- dates
  val
}

isHoliday <- function(calendar="TARGET", dates=Sys.Date()) {
  stopifnot(is.character(calendar))
  stopifnot(class(dates)=="Date")
  val <- .Call("QL_isHoliday",
               calendar, dates,
               PACKAGE="RQuantLib")
  val <- as.logical(val[[1]])
  names(val) <- dates
  val
}

isWeekend <- function(calendar="TARGET", dates=Sys.Date()) {
  stopifnot(is.character(calendar))
  stopifnot(class(dates)=="Date")
  val <- .Call("QL_isWeekend",
               calendar, dates,
               PACKAGE="RQuantLib")
  val <- as.logical(val[[1]])
  names(val) <- dates
  val
}

isEndOfMonth <- function(calendar="TARGET", dates=Sys.Date()) {
  stopifnot(is.character(calendar))
  stopifnot(class(dates)=="Date")
  val <- .Call("QL_isEndOfMonth",
               calendar, dates,
               PACKAGE="RQuantLib")
  val <- as.logical(val[[1]])
  names(val) <- dates
  val
}

endOfMonth <- function(calendar="TARGET", dates=Sys.Date()) {
  stopifnot(is.character(calendar))
  stopifnot(class(dates)=="Date")
  val <- .Call("QL_endOfMonth",
               calendar, dates,
               PACKAGE="RQuantLib")
  val <- val[[1]]
  names(val) <- dates
  val
}

adjust <- function(calendar="TARGET", dates=Sys.Date(), bdc = 0 ) {
  stopifnot(is.character(calendar))
  stopifnot(class(dates)=="Date")
  val <- .Call("QL_adjust",
               calendar, as.double(bdc), dates,
               PACKAGE="RQuantLib")
  val <- val[[1]]
  names(val) <- dates
  val
}

advance <- function(calendar="TARGET", dates=Sys.Date(),
                    n, timeUnit, # call 1
                    period,      # call 2
                    bdc = 0, emr = 0) {
  stopifnot(is.character(calendar))
  stopifnot(class(dates)=="Date")
  call1 <- missing(period) & !missing(n) & !missing(timeUnit)  ## or was it && ?
  call2 <- !missing(period) & missing(n) & missing(timeUnit)
  stopifnot(call1 | call2)
  val <- NULL
  if (call1)
    val <- .Call("QL_advance1",
                 calendar,
                 list(amount = as.double(n),
                      unit = as.double(timeUnit),
                      bdc = as.double(bdc),
                      emr = as.double(emr)),
                 dates,
                 PACKAGE="RQuantLib")
  if (call2)
    val <- .Call("QL_advance2",
                 calendar,
                 list(period = as.double(period),
                      bdc = as.double(bdc),
                      emr = as.double(emr)),
                 dates,
                 PACKAGE="RQuantLib")
  stopifnot( !is.null(val) )
  val <- val[[1]]
  val
}

businessDaysBetween  <- function(calendar="TARGET",
                                 from=Sys.Date(),
                                 to = Sys.Date() + 5,
                                 includeFirst = 1,
                                 includeLast = 0
                                 ) {
  stopifnot(is.character(calendar))
  stopifnot(class(from)=="Date")
  stopifnot(class(to)=="Date")
  val <- .Call("QL_businessDaysBetween",
               calendar,
               list(includeFirst = as.double(includeFirst),
                    includeLast = as.double(includeLast)),
               from, to,
               PACKAGE="RQuantLib")
  val <- val[[1]]
  val
}

holidayList <- function(calendar="TARGET",
                        from=Sys.Date(),
                        to=Sys.Date() + 5,
                        includeWeekends=0) {
  stopifnot(is.character(calendar))
  stopifnot(class(from)=="Date")
  stopifnot(class(to)=="Date")
  val <- .Call("QL_holidayList",
               calendar,
               list(includeWeekends=as.double(includeWeekends), from=from, to=to),
               PACKAGE="RQuantLib")
  val
}
