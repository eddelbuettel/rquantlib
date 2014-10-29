##  RQuantLib function DiscountCurve
##
##  Copyright (C) 2005         Dominick Samperi
##  Copyright (C) 2007 - 2014  Dirk Eddelbuettel
##  Copyright (C) 2009 - 2010  Dirk Eddelbuettel and Khanh Nguyen
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

DiscountCurve <- function(params, tsQuotes, times=seq(0,10,.1)) {
    UseMethod("DiscountCurve")
}

DiscountCurve.default <- function(params, tsQuotes, times=seq(0,10,.1)) {

    ## Check that params is properly formatted.
    if (!is.list(params) || length(params) == 0) {
        stop("The params parameter must be a non-empty list", call.=FALSE)
    }

    ## Check that the term structure quotes are properly formatted.
    if (!is.list(tsQuotes) || length(tsQuotes) == 0) {
        stop("Term structure quotes must be a non-empty list", call.=FALSE)
    }
    if (length(tsQuotes) != length(names(tsQuotes))) {
        stop("Term structure quotes must include labels", call.=FALSE)
    }
    if (!is.numeric(unlist(tsQuotes))) {
        stop("Term structure quotes must have numeric values", call.=FALSE)
    }

    ## Check the times vector
    if (!is.numeric(times) || length(times) == 0) {
        stop("The times parameter must be a non-emptry numeric vector", call.=FALSE)
    }

    ## Finally ready to make the call...
    #val <- .Call("DiscountCurve", params, tsQuotes, times, PACKAGE="RQuantLib")
    val <- discountCurveEngine(params, tsQuotes, times)

    val[["table"]] <- as.data.frame(val[["table"]])  ## Windows all of a sudden needs this
    class(val) <- c("DiscountCurve")
    val
}

plot.DiscountCurve <- function(x, setpar=TRUE, dolegend=TRUE,...) {
    if (setpar) {
        savepar <- par(mfrow=c(3,1))
    }
    if (x$flatQuotes) {
        ## Don't want to plot noise when we look at a flat yield curve
        plot(c(x$times[1],x$times[length(x$times)]), c(0,.5),type='n',
             main='forwards', xlab='time',ylab='forward rate')
        lines(x$times, x$forwards, type='l')
        if (dolegend) {
            legend('center','center','flat',bty='n',text.col='red')
        }
        plot(c(x$times[1],x$times[length(x$times)]), c(0,.5),type='n',
             main='zero rates', xlab='time',ylab='zero rate')
        lines(x$times, x$zerorates, type='l')
        if (dolegend) {
            legend('center','center','flat',bty='n',text.col='red')
        }
    } else {
        plot(x$times, x$forwards, type='l', main='forwards',xlab='time',ylab='fwd rate')
        if (dolegend) {
            legend('center','center',paste(x$params$interpHow, 'discount'), bty='n', text.col='red')
        }
        plot(x$times, x$zerorates, type='l', main='zero rates',xlab='time',ylab='zero rate')
        if (dolegend) {
            legend('center','center',paste(x$params$interpHow, 'discount'),bty='n', text.col='red')
        }
    }
    plot(x$times, x$discounts, type='l', main='discounts',xlab='time',ylab='discount')
    if (dolegend) {
        if (x$flatQuotes) {
            legend('center','center','flat',bty='n',text.col='red')
        } else {
            legend('center','center',paste(x$params$interpHow, 'discount'),bty='n', text.col='red')
        }
    }
    if (setpar) {
        par(savepar)
    }
}
