##  RQuantLib function BermudanSwaption
##
##  Copyright (C) 2005         Dominick Samperi
##  Copyright (C) 2007 - 2014  Dirk Eddelbuettel
##  Copyright (C) 2016         Terry Leitch 
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

BermudanSwaption <- function(params, ts, swaptionMaturities,
                             swapTenors, volMatrix) {
    UseMethod("BermudanSwaption")
}

BermudanSwaption.default <- function(params, ts, swaptionMaturities,
                                     swapTenors, volMatrix) {
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
    matYears=as.numeric(params$maturity-params$tradeDate)/365
    expYears=as.numeric(params$startDate-params$tradeDate)/365
    increment=min(matYears/6,1.0)
    numObs=floor(matYears/increment)+1
    optStart=as.numeric(params$startDate-params$tradeDate)/365
    
    # find closest option to our target to ensure it is in calibration
    tenor=expiry=vol=vector(length=numObs,mode="numeric")
    
    expiryIDX=findInterval(expYears,swaptionMaturities)
    tenorIDX=findInterval(matYears-expYears,swapTenors)
    if(tenorIDX >0 & expiryIDX>0){
        vol[1]=volMatrix[expiryIDX,tenorIDX]
        expiry[1]=swaptionMaturities[expiryIDX]
        tenor[1]=swapTenors[tenorIDX]
    } else {
        vol[1]=expiry[1]=tenor[1]=0
    }
    
    for(i in 2:numObs){
        expiryIDX=findInterval(i*increment,swaptionMaturities)
        tenorIDX=findInterval(matYears-(i-1)*increment,swapTenors)
        if(tenorIDX >0 & expiryIDX>0){
            vol[i]=volMatrix[expiryIDX,tenorIDX]
            expiry[i]=swaptionMaturities[expiryIDX]
            tenor[i]=swapTenors[tenorIDX]
            
        } else {
            vol[i]=volMatrix[expiryIDX,tenorIDX+1]
            expiry[i]=swaptionMaturities[expiryIDX]
            tenor[i]=swapTenors[tenorIDX+1]
        }
    }
    
    # remove if search was out of bounds
    expiry=expiry[expiry>0];tenor=tenor[tenor>0];vol=vol[vol>0]
    if(length(expiry)<5){
        warning("Insufficent vols to fit affine model")
        return(NULL)
    }
    #Take 1st 5 which includes closest to initial date
    expiry=expiry[1:5];tenor=tenor[1:5];vol=vol[1:5]
    
#
# Check that the term structure quotes are properly formatted.
#         if(is)
#             if (!is.list(ts) || length(ts) == 0) {
#                 stop("Term structure quotes must be a non-empty list", call.=FALSE)
#             }
#         if (length(ts) != length(names(ts))) {
#             stop("Term structure quotes must include labels", call.=FALSE)
#         }
#         if (!is.numeric(unlist(ts))) {
#             stop("Term structure quotes must have numeric values", call.=FALSE)
#         }


    
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
        val <- bermudanWithRebuiltCurveEngine(params, c(ts$table$date), ts$table$zeroRates,
                                      swaptionMaturities,
                                      swapTenors, volMatrix)   
    } else{
        if (!is.numeric(ts) | length(ts) !=1) {
            stop("Flat Term structure yield must have single numeric value", call.=FALSE)
        }
        val <- bermudanFromYieldEngine(params, ts,
                                  swaptionMaturities,
                                  swapTenors, volMatrix)
    }
    class(val) <- c(params$method, "BermudanSwaption")
    val
}

summary.G2Analytic <- function(object,...) {
    cat('\n\tSummary of pricing results for Bermudan Swaption\n')
    cat('\nPrice (in bp) of Bermudan swaption is ', object$price)
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
    cat('\n\tSummary of pricing results for Bermudan Swaption\n')
    cat('\nPrice (in bp) of Bermudan swaption is ', object$price)
    cat('\nStike is ', format(object$params$strike,digits=6))
    cat(' (ATM strike is ', format(object$ATMStrike,digits=6), ')')
    cat('\nModel used is: Hull-White using analytic formulas')
    cat('\nCalibrated model parameters are:')
    cat('\na = ', format(object$a,digits=4))
    cat('\nsigma = ', format(object$sigma,digits=4))
    cat('\n\n')
}

summary.HWTree <- function(object,...) {
    cat('\n\tSummary of pricing results for Bermudan Swaption\n')
    cat('\nPrice (in bp) of Bermudan swaption is ', object$price)
    cat('\nStike is ', format(object$params$strike,digits=6))
    cat(' (ATM strike is ', format(object$ATMStrike,digits=6), ')')
    cat('\nModel used is: Hull-White using a tree')
    cat('\nCalibrated model parameters are:')
    cat('\na = ', format(object$a,digits=4))
    cat('\nsigma = ', format(object$sigma,digits=4))
    cat('\n\n')
}

summary.BKTree <- function(object,...) {
    cat('\n\tSummary of pricing results for Bermudan Swaption\n')
    cat('\nPrice (in bp) of Bermudan swaption is ', object$price)
    cat('\nStike is ', format(object$params$strike,digits=6))
    cat(' (ATM strike is ', format(object$ATMStrike,digits=6), ')')
    cat('\nModel used is: Black-Karasinski using a tree')
    cat('\nCalibrated model parameters are:')
    cat('\na = ', format(object$a,digits=4))
    cat('\nsigma = ', format(object$sigma,digits=4))
    cat('\n\n')
}
