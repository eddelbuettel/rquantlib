##  RQuantLib function AffineSwaption
##
##  Copyright (C) 2005         Dominick Samperi
##  Copyright (C) 2007 - 2014  Dirk Eddelbuettel

##  Copyright (C) 2016         Terry Leitch and Dirk Eddelbuettel
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

Swap <- function(params,ts,  legparams=list(dayCounter="Thirty360",
                                          fixFreq="Annual",
                                          floatFreq="Semiannual")) {
    UseMethod("AffineSwaption")
}

Swap.default <- function(params,ts,  legparams=list(dayCounter="Thirty360",
                                                    fixFreq="Annual",
                                                    floatFreq="Semiannual")) {
    # Check that params list names
    
    if (!is.list(params) || length(params) == 0) {
        stop("The params parameter must be a non-empty list", call.=FALSE)
    }
    if(is.null(params$startDate)){
        params$startDate=advance("UnitedStates",params$tradeDate, 1, 3)
        warning("swaption start date not set, defaulting to 1 year from trade date using US calendar")
    }
    if(is.null(params$maturity)){
        params$maturity=advance("UnitedStates",params$startDate, 5, 3)
        warning("swaption maturity not set, defaulting to 5 years from startDate using US calendar")
    }

    if(is.null(params$payFix)){
        params$payFix=TRUE
        warning("affine swaption payFix flag not set defaulting to pay fix swap")
    }
    
    matYears=as.numeric(params$maturity-params$tradeDate)/365
    startYears=as.numeric(params$startDate-params$tradeDate)/365
    increment=min(matYears/6,1.0)
    numObs=floor(matYears/increment)+1
    swapStart=as.numeric(params$startDate-params$tradeDate)/365
    
    # find closest option to our target to ensure it is in calibration
    tenor=expiry=vol=vector(length=numObs,mode="numeric")
    
    # Finally ready to make the call...
    # We could coerce types here and pass as.integer(round(swapTenors)),
    # temp <- as.double(volMatrix), dim(temp) < dim(a) [and pass temp instead
    # of volMatrix]. But this is taken care of in the C/C++ code.
    if(class(ts)=="DiscountCurve"){
        matchlegs<-matchParams(legparams)
        val <- swapWithRebuiltCurveEngine(params, matchlegs, c(ts$table$date), ts$table$zeroRates)   
    } else{
        stop("DiscountCurve class term structure required", call.=FALSE)
        
    }
    class(val) <- "Swap"
    summary(val)
    val
}
