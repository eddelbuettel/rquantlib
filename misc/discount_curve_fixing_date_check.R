
library(RQuantLib)

params <- list(tradeDate=as.Date('2016-2-15'),
               settleDate=as.Date('2016-2-17'),
               startDate=as.Date('2017-2-17'),
               maturity=as.Date('2022-2-17'),
               european=TRUE,
               dt=.25,
               expiryDate=as.Date('2017-2-17'),
               strike=.02,
               interpWhat="discount",
               interpHow="loglinear")

# Set leg paramters for generating discount curve
dclegparams=list(dayCounter="Thirty360",
                 fixFreq="Annual",
                 floatFreq="Semiannual")

setEvaluationDate(as.Date("2016-2-15"))
times<-times <- seq(0,14.75,.25)

data(tsQuotes)
dcurve <- DiscountCurve(params, tsQuotes, times=times,dclegparams)

# Price the Bermudan swaption
swaplegparams=list(fixFreq="Semiannual",floatFreq="Quarterly")

data(vcube)
pricing <- SabrSwaption(params, dcurve,vcube,swaplegparams)
pricing
