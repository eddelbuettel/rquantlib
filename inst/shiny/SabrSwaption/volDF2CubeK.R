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
  atmMat=acast(tbl3,Expiry~Tenor,value.var = "LogNormalVol")
  #atmMat=matrix(data=NA,nrow=length(expLvl),ncol=length(tenorLvl),dimnames=list(expLvl,tenorLvl))
  # for(i in 1:length(expLvl)){
  #   for(j in 1:length(tenorLvl)){
  #     atmMat[i,j]=tbl[tbl$Expiry==expLvl[i]& tbl$Tenor==tenorLvl[j] & tbl$Spread==0,]$LogNormalVol
  #   }
  # }
  smirk=matrix(ncol=length(strikes),nrow=length(expLvl)*length(tenorLvl))
  
  tmp3=acast(tbl,Expiry~Tenor~Spread,value.var="LogNormalVol")
  k=0
  for(i in 1:length(expLvl)){
    for(j in 1:length(tenorLvl)){
      k=k+1
      #for(n in 1:length(strikes)){
      smirk[k,]=tmp3[i,j,]-tmp3[i,j,"0"]
      #smirk[k,n]=tbl[tbl$Expiry==expLvl[i]& tbl$Tenor==tenorLvl[j] & tbl$Spread==strikes[n],]$LogNormalVol -
      # tbl[tbl$Expiry==expLvl[i]& tbl$Tenor==tenorLvl[j] & tbl$Spread==0,]$LogNormalVol
      #}
    }
  }
  
  
  smirk <- na.spline(smirk,method="natural")
  
  tmp <- list(atmVol=atmMat,tenors=tenors,expiries=expiries,smirk=smirk,strikes=strikes/10000)
  class(tmp) <- "volcube"
  
  return(tmp)
}