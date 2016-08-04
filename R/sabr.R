##  RQuantLib function sabrSwaption
##
##  Copyright (C) 2005         Dominick Samperi
##  Copyright (C) 2007 - 2015  Dirk Eddelbuettel
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

SabrSwaption <- function(params,
                         ts,volCubeDF, 
                         legparams=list(dayCounter="Thirty360",
                                        fixFreq="Annual",
                                        floatFreq="Semiannual"),
                         tsUp01=NA,tsDn01=NA,vega=FALSE) {

    UseMethod("SabrSwaption")
}

SabrSwaption.default <- function(params, 
                                 ts,  volCubeDF, 
                                 legparams=list(dayCounter="Thirty360",
                                                fixFreq="Annual",
                                                floatFreq="Semiannual"),
                                 tsUp01=NA,tsDn01=NA,vega=FALSE) {
    # Check that params list names

    if(is.null(params$startDate)){
        params$startDate=advance("UnitedStates",params$tradeDate, 1, 3)
        warning("swaption start date not set, defaulting to 1 year from trade date using US calendar")
    }
    if(is.null(params$expiryDate)){
        params$expiryDate=params$startDate
        warning("swaption expiry date not set, defaulting to 1 year from trade date using US calendar")
    }
    if(is.null(params$maturity)){
        params$maturity=advance("UnitedStates",params$startDate, 5, 3)
        warning("swaption maturity not set, defaulting to 5 years from startDate using US calendar")
    }
    volCube=volDF2CubeK(params,volCubeDF)
    if(vega){
        volCubeDF$LogNormalVol=volCubeDF$LogNormalVol+.01
        volCubeUp=volDF2CubeK(params,volCubeDF)
    }
    swapTenors=volCube$tenors
    if (!is.list(params) || length(params) == 0) {
        stop("The params parameter must be a non-empty list", call.=FALSE)
    }
    swaptionMaturities=volCube$expiries
    ##reshape dataframe to fit QL call
    vc=volDF2CubeK(params,volCubeDF)
    matYears=as.numeric(params$maturity-params$tradeDate)/365
    expYears=as.numeric(params$expiryDate-params$tradeDate)/365

    if(class(ts)=="DiscountCurve"){
        matchlegs<-matchParams(legparams)
        val <- sabrengine(params, matchlegs, c(ts$table$date), ts$table$zeroRates,
                          volCube$expiries,volCube$tenors,volCube$atmVol,volCube$strikes,volCube$smirk) 
        if(vega){
            valUp <- sabrengine(params, matchlegs, c(ts$table$date), ts$table$zeroRates,
                                volCubeUp$expiries,volCubeUp$tenors,volCubeUp$atmVol,volCubeUp$strikes,volCubeUp$smirk)

            val$payVega=valUp$pay-val$pay
            val$rcvVega=valUp$rcv-val$rcv
            if(anyNA(tsUp01)){
            }else{
                valTsUp <- sabrengine(params, matchlegs, c(tsUp01$table$date), tsUp01$table$zeroRates,
                                      volCube$expiries,volCube$tenors,volCube$atmVol,volCube$strikes,volCube$smirk) 

                val$payDV01=valTsUp$pay-val$pay
                val$rcvDV01=valTsUp$rcv-val$rcv
                if(anyNA(tsDn01)){
                } else{
                    valTsDn <- sabrengine(params, matchlegs, c(tsDn01$table$date), tsDn01$table$zeroRates,
                                          volCube$expiries,volCube$tenors,volCube$atmVol,volCube$strikes,volCube$smirk) 
                    val$payCnvx=(valTsUp$pay+valTsDn$pay-2*val$pay)/2
                    val$rcvCnvx=(valTsUp$rcv+valTsDn$rcv-2*val$rcv)/2
                }
            }
        }
    } else{
        stop("DiscountCurve class term structure required", call.=FALSE)
        
    }
    val$params=params
    val$atmRate=as.numeric(val$atmRate)
    class(val) <- "SabrSwaption"
    summary(val)
    val
}


volDF2CubeK <- function(params, tbl, source = "CME") {
    strikes <- levels(tbl$Spread)<-c(-200,-150,-100,-75,-50,-25,0,25,50,75,100,150,200)
  
    matYears <- as.numeric(params$maturity-params$tradeDate)/365
    expYears <- as.numeric(params$expiryDate-params$tradeDate)/365
    expLvl <- c( "1M","3M","6M","1Y","2Y","3Y","4Y",  "5Y",  "6Y",  "7Y",  "8Y",  "9Y","10Y")
    tbl$Expiry <- factor(tbl$Expiry, levels  <-  expLvl)
    expiries <- c(1/12,.25,.5,1,2,3,4,5,6,7,8,9,10)
    tenorLvl <- c( "1Y", "2Y",  "5Y",  "10Y","15Y","20Y","30Y")
    tbl$Tenor <- factor(tbl$Tenor, levels = tenorLvl)
    tenors <- c(1,2,5,10,15,20,30)
    tenorIDX <- max(findInterval(matYears-expYears,tenors),1)
    tenorIDX <- min(tenorIDX,length(tenors)-3)
    expiryIDX <- findInterval(expYears,expiries)
    expiryIDX <- min(expiryIDX,length(expiries)-3)
    
    strikeIDX <- 1
    expire <- expLvl[expiryIDX]
    tenor <- tenorLvl[tenorIDX]

    for(strike in levels(tbl$Spread)){
        if(!is.na(tbl[tbl$Expiry==expire & tbl$Spread==strike &tbl$Tenor==tenor,]$LogNormalVol))break;
        strikeIDX <- strikeIDX+1
    }
    strikes <- strikes[strikeIDX:length(strikes)]
    expLvl <- expLvl[expiryIDX:length(expLvl)]
    expiries <- expiries[expiryIDX:length(expiries)]
    tenorLvl <- tenorLvl[tenorIDX:length(tenorLvl)]
    tenors <- tenors[tenorIDX:length(tenors)]
    tbl <- tbl[tbl$Expiry%in%expLvl,]
    tbl <- tbl[tbl$Tenor%in%tenorLvl,]
    tbl <- tbl[tbl$Spread%in%strikes,]
    tbl <- tbl[with(tbl,order(Expiry,Tenor,Spread)),]
    tbl3 <- tbl[tbl$Spread==0,]
    # atm vol matrix
    #atmMat=acast(tbl3,Expiry~Tenor,value.var = "LogNormalVol")
    atmMat=matrix(data=NA,nrow=length(expLvl),ncol=length(tenorLvl),dimnames=list(expLvl,tenorLvl))
    for(i in 1:length(expLvl)){
      for(j in 1:length(tenorLvl)){
        atmMat[i,j]=tbl[tbl$Expiry==expLvl[i]& tbl$Tenor==tenorLvl[j] & tbl$Spread==0,]$LogNormalVol
      }
    }
    smirk=matrix(ncol=length(strikes),nrow=length(expLvl)*length(tenorLvl))
    
    #tmp3=acast(tbl,Expiry~Tenor~Spread,value.var="LogNormalVol")
    k=0
    for(i in 1:length(expLvl)){
      for(j in 1:length(tenorLvl)){
        k=k+1
        for(n in 1:length(strikes)){
          #smirk[k,]=tmp3[i,j,]-tmp3[i,j,"0"]
          smirk[k,n]=tbl[tbl$Expiry==expLvl[i]& tbl$Tenor==tenorLvl[j] & tbl$Spread==strikes[n],]$LogNormalVol -
            tbl[tbl$Expiry==expLvl[i]& tbl$Tenor==tenorLvl[j] & tbl$Spread==0,]$LogNormalVol
        }
      }
    }  
    
  
    smirk <- na.spline(smirk,method="natural")
  
    tmp <- list(atmVol=atmMat,tenors=tenors,expiries=expiries,smirk=smirk,strikes=strikes/10000)
    class(tmp) <- "volcube"
  
    return(tmp)
}
