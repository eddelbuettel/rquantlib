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

AffineSwaption <- function(params,
                           ts, swaptionMaturities,
                           swapTenors, volMatrix, 
                           legparams=list(dayCounter="Thirty360",
                                          freq="Annual",
                                          floatFreq="Semiannual")) {
    UseMethod("AffineSwaption")
}

AffineSwaption.default <- function(params,
                                   ts, swaptionMaturities,
                                   swapTenors, volMatrix, 
                                   legparams=list(dayCounter="Thirty360",
                                                  freq="Annual",
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
    if(is.null(params$european)){
        params$european=TRUE
        warning("affine swaption european flag not set defaulting to european")
    }
    if(is.null(params$payFix)){
        params$payFix=TRUE
        warning("affine swaption payFix flag not set defaulting to pay fix swap")
    }
    

    
    matYears=as.numeric(params$maturity-params$tradeDate)/365
    optStart=as.numeric(params$startDate-params$tradeDate)/365
    numObs=round(matYears-optStart)
    
    tenor=expiry=vol=vector(length=numObs,mode="numeric")
    for(i in 1:numObs){
        expiryIDX=findInterval(optStart+i-1+.5,swaptionMaturities)
        tenorIDX=findInterval(matYears-optStart-i+1,swapTenors)
        if(tenorIDX >0 & expiryIDX>0){
            vol[i]=volMatrix[expiryIDX,tenorIDX]
            expiry[i]=swaptionMaturities[expiryIDX]
            tenor[i]=swapTenors[tenorIDX]
        } else {
            vol[i]=expiry[i]=tenor[i]=0
        }
    }

    expiry=expiry[expiry>0];tenor=tenor[tenor>0];vol=vol[vol>0]

    # Check for correct matrix/vector types
    if (!is.matrix(volMatrix)
        || !is.vector(swaptionMaturities)
        || !is.vector(swapTenors)) {
        stop("Swaption vol must be a matrix, maturities/tenors must be vectors",
             call.=FALSE)
    }
    
    # Check that matrix/vectors have compatible dimensions
    if (prod(dim(volMatrix)) != length(swaptionMaturities)*length(swapTenors)) {
        stop("Dimensions of swaption vol matrix not compatible with maturity/tenor vectors",
             call.=FALSE)
    }
    
    # Finally ready to make the call...
    # We could coerce types here and pass as.integer(round(swapTenors)),
    # temp <- as.double(volMatrix), dim(temp) < dim(a) [and pass temp instead
    # of volMatrix]. But this is taken care of in the C/C++ code.
    if(class(ts)=="DiscountCurve"){
        matchlegs<-matchParams(legparams)
        val <- affineWithRebuiltCurveEngine(params, matchlegs, c(ts$table$date), ts$table$zeroRates,
                                      expiry,tenor,vol)   
    } else{
            stop("DiscountCurve class term structure required", call.=FALSE)

    }
    class(val) <- c(params$method, "AffineSwaption")
    val
}

summary.G2Analytic <- function(object,...) {
    cat('\n\tSummary of pricing results for Affine Swaption\n')
    cat('\nPrice (in bp) of Affine swaption is ', object$NPV)
    cat('\nStike is ', format(object$params$strike,digits=6))
    cat(' (ATM strike is ', format(object$ATMStrike,digits=6), ')')
    cat('\nModel used is: G2/Jamshidian using analytic formulas')
    cat('\nCalibrated model parameters are:')
    cat('\na = ', format(object$a,digits=4))
    cat('\nb = ', format(object$b,digits=4))
    cat('\nsigma = ', format(object$sigma,digits=4))
    cat('\neta = ', format(object$eta,digits=4))
    cat('\nrho = ', format(object$rho,digits=4))
    cat('\n\n')
}

summary.HWAnalytic <- function(object,...) {
    cat('\n\tSummary of pricing results for Affine Swaption\n')
    cat('\nPrice (in bp) of Affine swaption is ', object$NPV)
    cat('\nStike is ', format(object$params$strike,digits=6))
    cat(' (ATM strike is ', format(object$ATMStrike,digits=6), ')')
    cat('\nModel used is: Hull-White using analytic formulas')
    cat('\nCalibrated model parameters are:')
    cat('\na = ', format(object$a,digits=4))
    cat('\nsigma = ', format(object$sigma,digits=4))
    cat('\n\n')
}

summary.HWTree <- function(object,...) {
    cat('\n\tSummary of pricing results for Affine Swaption\n')
    cat('\nPrice (in bp) of Affine swaption is ', object$NPV)
    cat('\nStike is ', format(object$params$strike,digits=6))
    cat(' (ATM strike is ', format(object$ATMStrike,digits=6), ')')
    cat('\nModel used is: Hull-White using a tree')
    cat('\nCalibrated model parameters are:')
    cat('\na = ', format(object$a,digits=4))
    cat('\nsigma = ', format(object$sigma,digits=4))
    cat('\n\n')
}

summary.BKTree <- function(object,...) {
    cat('\n\tSummary of pricing results for Affine Swaption\n')
    cat('\nPrice (in bp) of Affine swaption is ', object$NPV)
    cat('\nStike is ', format(object$params$strike,digits=6))
    cat(' (ATM strike is ', format(object$ATMStrike,digits=6), ')')
    cat('\nModel used is: Black-Karasinski using a tree')
    cat('\nCalibrated model parameters are:')
    cat('\na = ', format(object$a,digits=4))
    cat('\nsigma = ', format(object$sigma,digits=4))
    cat('\n\n')
}
