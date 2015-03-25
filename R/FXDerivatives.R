
DNTVannaVolga=function(barrierUp,barrierDown,underlying,dividendYield,riskFreeRate,maturity,volatility,BF25D,RR25D){
  
  volATM=volatility
  vol25Put=volatility+BF25D-0.5*RR25D
  vol25Call=volatility+BF25D+0.5*RR25D
  
  if(barrierUp<barrierDown | barrierUp<0 | barrierDown<0){
    # 		temp=barrierUp
    # 		barrierUp=barrierDown
    # 		barrierDown=temp
    return(NA)
  }
  else{
    barrierDown=ifelse(barrierDown<=0,0.01,barrierDown)
    
    callDKO=doubleKOVannaVolga("call",barrierUp =barrierUp,  barrierDown =  barrierDown,underlying = underlying,strike = barrierDown,
                               dividendYield, riskFreeRate,maturity,   vol25Put, volATM,   vol25Call,volatility)$calc
    putDKO=doubleKOVannaVolga("put",barrierUp =barrierUp,  barrierDown =  barrierDown,underlying = underlying,strike = barrierUp,
                              dividendYield, riskFreeRate,maturity, vol25Put, volATM,   vol25Call,volatility)$calc
    
    DNT=1/(barrierUp-barrierDown)*callDKO+1/(barrierUp-barrierDown)*putDKO
    
    return(DNT)
  }
}

oneTouch=function(type,underlying, strike, dividendYield,riskFreeRate, maturity, vol, cashPayoff,strikesMat,expirations,volMatrix){
  
  #strikes is a vector of double
  #expiration is a vector of integer, being the number of days until expirations
  
  calc = tryCatch({
    oneTouchEngine(type=type,underlying=underlying, strike=strike, dividendYield=(dividendYield)/100,riskFreeRate=(riskFreeRate)/100, maturity=maturity/12, volatility=vol/100, cashPayoff=100,strikesMat,expirations,volMatrix/100)$calc
  }, warning = function(w) {
    oneTouchEngine(type=type,underlying=underlying, strike=strike, dividendYield=(dividendYield)/100,riskFreeRate=(riskFreeRate)/100, maturity=maturity/12, volatility=vol/100, cashPayoff=100,strikesMat,expirations,volMatrix/100)$calc
  }, error = function(e) {
    NA
  }, finally = {
    NA
  })
  
  return(calc)
}


doubleKOVannaVolga=function(type,barrierUp,  barrierDown,underlying,strike,dividendYield, riskFreeRate,maturity,   vol25Put, volATM,   vol25Call,volatility){
  
  DKO=doubleKOVannaVolga(type,barrierUp,  barrierDown,underlying,strike, dividendYield, riskFreeRate,maturity,   vol25Put, volATM,   vol25Call,volatility)
  
  return(DKO)
}