
# This is the server logic for a Shiny web application.
# You can find out more about building applications with Shiny here:
#
# http://shiny.rstudio.com
#
library(rhandsontable)
library(lubridate)
library(reshape2)
library(zoo)
library(data.table)
data(vcube)
data(tsQuotes)
library(shiny)

assignInNamespace("volDF2CubeK",source("volDF2CubeK.R"),"RQuantLib")

# tsQuotes <- list(
#   d1w  =0.004,
#   d1y=.0091,
#   s2y=.0115,
#   s3y=.0138,
#   s4y =.011349,
#   s5y  =0.012375,
#   s6y =.0134,
#   s7y=.01446,
#   s10y =0.016829,
#   s15y =0.01944,
#   s30y=.02178,
#   s50y=.02205)

tsQuoteUp01=lapply(tsQuotes,"+",.0001)
tsQuoteDn01=lapply(tsQuotes,"-",.0001)

setEvaluationDate(as.Date("2016-4-30"))

params <- list(tradeDate=as.Date('2016-4-28'),
               settleDate=as.Date('2016-4-30'),
               payFixed=TRUE,
               #european=F,
               dt=.25,
               interpWhat="discount",
               interpHow="loglinear")

legparams=list(dayCounter="Thirty360",
               fixFreq="Annual",
               floatFreq="Semiannual")

times<-times <- seq(0,49.5,.25)

dcurve <- DiscountCurve(params, tsQuotes, times=times,legparams)
dcurveUp01 <- DiscountCurve(params, tsQuoteUp01, times=times,legparams)
dcurveDn01 <- DiscountCurve(params, tsQuoteDn01, times=times,legparams)

expiriesY=c(1/12,.25,.5,1,2,3,4,5,6,7,8,9,10)
expiriesM=c(1,3,6,0,0,0,0,0,0,0,0,0,0)
expLvl=c("1M","3M","6M","1Y" , "2Y", "3Y",  "4Y",  "5Y",  "6Y",  "7Y",  "8Y",  "9Y","10Y")

tenorLvl=c("1Y" , "2Y",  "5Y",  "10Y","15Y","20Y","30Y")
tenors=c(1,2,5,10,15,20,30)



shinyServer(function(input, output) {
    inputVol<-vcube
    output$table<- renderRHandsontable({
  
        tbl2=inputVol
        tbl2$Expiry<-factor(tbl2$Expiry,level=expLvl)
        tbl2$Tenor=factor(tbl2$Tenor,level=tenorLvl)
        tbl2=subset(tbl2,Expiry %in% input$expiries)
        tbl2=subset(tbl2,Tenor %in% input$tenors)

        tbl2=tbl2[with(tbl2,order(Expiry,Tenor,-Spread))]
        tbl2[,"payer"]<- -.0000001
        tbl2[,"payerDV01"]<- -.0000001
        tbl2[,"payerCnvx"]<- -.0000001
        tbl2[,"rcv"]<- -.0000001
        tbl2[,"rcvDV01"]<- -.0000001
        tbl2[,"rcvCnvx"]<- -.0000001
        tbl2[,"payerVega"]<- -.0000001
        tbl2[,"rcvVega"]<- -.0000001
        tbl2[,"sabrVol"]<- -.0000001
        tbl2[,"strike"]<- -.00000001
        for(expire in input$expiries){
          for(tenor in input$tenors){
            expIDX=match(expire,expLvl)
            expiry=params$settleDate;
            if(expIDX>3){
            year(expiry)=year(expiry)+expiriesY[expIDX]
            } else{
            expiry=expiry%m+% months(expiriesM[expIDX])
            }
            tenorIDX=match(tenor,tenorLvl)
            tenorVal=expiry;
            params$european=!input$bermudan
            year(tenorVal)=year(tenorVal)+tenors[tenorIDX]
            params$expiryDate=expiry
            if(params$european){params$startDate=expiry;}else{params$startDate=params$settleDate}
            params$maturity=tenorVal
            params$strike=.03
            tmp2<-SabrSwaption(params, dcurve, inputVol,legparams, vega=T)
            fwd1=tbl2[tbl2$Expiry==expire & tbl2$Tenor== tenor & tbl2$Spread==0,]$Strike
            fwdDiff=fwd1-tmp2$atmRate
            for(spread in unique(tbl2$Spread)){
              params$strike=tmp2$atmRate+spread/10000
              if(params$strike>0){
              tmp<-SabrSwaption(params, dcurve, inputVol,legparams,tsUp01=dcurveUp01,tsDn01=dcurveDn01,vega=T)
              tbl2[tbl2$Expiry==expire & tbl2$Tenor== tenor & tbl2$Spread==spread,rcv:=signif(tmp$rcv,digits=4)]
              tbl2[tbl2$Expiry==expire & tbl2$Tenor== tenor & tbl2$Spread==spread,payer:=signif(tmp$pay,digits=4)] 
              tbl2[tbl2$Expiry==expire & tbl2$Tenor== tenor & tbl2$Spread==spread,rcvDV01:=signif(tmp$rcvDV01,digits=6)]
              tbl2[tbl2$Expiry==expire & tbl2$Tenor== tenor & tbl2$Spread==spread,rcvCnvx:=signif(tmp$rcvCnvx,digits=6)]
              tbl2[tbl2$Expiry==expire & tbl2$Tenor== tenor & tbl2$Spread==spread,payerDV01:=signif(tmp$payDV01,digits=6)]
              tbl2[tbl2$Expiry==expire & tbl2$Tenor== tenor & tbl2$Spread==spread,payerCnvx:=signif(tmp$payCnvx,digits=6)]
              tbl2[tbl2$Expiry==expire & tbl2$Tenor== tenor & tbl2$Spread==spread,rcvVega:=signif(tmp$rcvVega,digits=4)]
              tbl2[tbl2$Expiry==expire & tbl2$Tenor== tenor & tbl2$Spread==spread,payerVega:=signif(tmp$payVega,digits=4)]
              tbl2[tbl2$Expiry==expire & tbl2$Tenor== tenor & tbl2$Spread==spread,sabrVol:=signif(tmp$sigma,digits=3)]
              tbl2[tbl2$Expiry==expire & tbl2$Tenor== tenor & tbl2$Spread==spread,strike:=signif(params$strike,digits=4)]
              }
            }
          }
        }
        tbl2=tbl2[,c("Expiry","Tenor","Spread","strike","LogNormalVol","sabrVol","payer","payerDV01","payerCnvx","payerVega","rcv","rcvDV01","rcvCnvx","rcvVega"),with=F]
        numRow=length(tbl2)
        rhandsontable(tbl2,readOnly = F,rowHeaders=NULL,selectCallback = TRUE,halign="htCenter")%>%
          hot_col("strike", format = "0.0000") %>%
          hot_col("LogNormalVol", format = "0.00") %>%
          hot_col("sabrVol", format = "0.00") %>%
          hot_col("payer", format = "0.0000") %>%
          hot_col("payerDV01", format = "0.000000") %>%
          hot_col("payerCnvx", format = "0.000000") %>%
          hot_col("payerVega", format = "0.0000") %>%
          hot_col("rcv", format = "0.000")%>%
          hot_col("rcvDV01", format = "0.00000")%>%
          hot_col("rcvCnvx", format = "0.00000")%>%
          hot_col("rcvVega", format = "0.0000")
    })
    


})
