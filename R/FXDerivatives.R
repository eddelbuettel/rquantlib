
DoubleNoTouchOption<-function(barrierUp,barrierDown,underlying,dividendYield,riskFreeRate,maturity,volatility,BF25D,RR25D){
  
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
    
    callDKO=DoubleKnockOutOption("call",barrierUp =barrierUp,  barrierDown =  barrierDown,underlying = underlying,strike = barrierDown,
                               dividendYield, riskFreeRate,maturity,   vol25Put, volATM,   vol25Call,volatility)$value
    putDKO=DoubleKnockOutOption("put",barrierUp =barrierUp,  barrierDown =  barrierDown,underlying = underlying,strike = barrierUp,
                              dividendYield, riskFreeRate,maturity, vol25Put, volATM,   vol25Call,volatility)$value
    
    DNT=1/(barrierUp-barrierDown)*callDKO+1/(barrierUp-barrierDown)*putDKO
    
    return(DNT)
  }
}

AmericanBinaryOption<-function(type,underlying, strike, dividendYield,riskFreeRate, maturity, vol, cashPayoff,strikesMat,expirations,volMatrix){
  
  #strikes is a vector of double
  #expiration is a vector of integer, being the number of days until expirations
  
#   value = tryCatch({
#      }, warning = function(w) {
#     oneTouchEngine(type=type,underlying=underlying, strike=strike, dividendYield=(dividendYield),riskFreeRate=(riskFreeRate), maturity=maturity, volatility=vol, cashPayoff=100,strikesMat,expirations,volMatrix)$value
#   }, error = function(e) {
#     NA
#   }, finally = {
#     NA
#   })
#   
  value=oneTouchEngine(type=type,underlying=underlying, strike=strike, dividendYield=(dividendYield),riskFreeRate=(riskFreeRate), maturity=maturity, volatility=vol, cashPayoff=cashPayoff,strikesMat,expirations,volMatrix)$value
  
  
  return(value)
}


DoubleKnockOutOption <- function(type,barrierUp,  barrierDown,underlying,strike,
                                 dividendYield, riskFreeRate,maturity,   vol25Put,
                                 volATM,   vol25Call,volatility) {
  UseMethod("DoubleKnockOutOption")
}

DoubleKnockOutOption.default <- function(type,barrierUp,  barrierDown,underlying,strike,
                                         dividendYield, riskFreeRate,maturity,   vol25Put,
                                         volATM,   vol25Call,volatility) {
  type <- match.arg(type, c("call", "put"))
  val <- doubleKOVannaVolgaEngine(type,barrierUp,  barrierDown,underlying,strike,
                                  dividendYield, riskFreeRate,maturity,   vol25Put,
                                  volATM,   vol25Call,volatility)
  class(val) <- c("Option")
  val
}