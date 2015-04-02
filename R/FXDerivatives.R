DoubleNoTouchOption<-function(barrierUp,barrierDown,underlying,dividendYield,riskFreeRate,
                              maturity,volatility,BF25D,RR25D){

  UseMethod("DoubleNoTouchOption")
  
}

DoubleNoTouchOption.default<-function(barrierUp,barrierDown,underlying,dividendYield,riskFreeRate,
                              maturity,volatility,BF25D,RR25D){
  
  volATM=volatility
  vol25Put=volatility+BF25D-0.5*RR25D
  vol25Call=volatility+BF25D+0.5*RR25D
  
  barrierDown=ifelse(barrierDown<=0,0.001,barrierDown)
  
  callDKO=DoubleKnockOutOption("call",barrierUp =barrierUp,  barrierDown =  barrierDown,
                               underlying = underlying,strike = barrierDown,
                               dividendYield, riskFreeRate,maturity,   vol25Put, volATM,   
                               vol25Call,volatility)$value
  putDKO=DoubleKnockOutOption("put",barrierUp =barrierUp,  barrierDown =  barrierDown,
                              underlying = underlying,strike = barrierUp,dividendYield, 
                              riskFreeRate,maturity, vol25Put, volATM,   vol25Call,volatility)$value
  
  DNT=list(value=1/(barrierUp-barrierDown)*callDKO+1/(barrierUp-barrierDown)*putDKO)
  class(DNT) <- c("TouchOption", "Option")
  return(DNT)
  
}

AmericanBinaryOption<-function(type,underlying, strike, dividendYield,riskFreeRate, 
                               maturity, vol, cashPayoff,volStrikes,volMaturities,volMatrix){
  
  #volStrikes is a vector of double
  #volMaturities is a vector of integer, being the number of days until maturity
  
  UseMethod("AmericanBinaryOption")
  
}

AmericanBinaryOption.default<-function(type,underlying, strike, dividendYield,riskFreeRate, 
                                       maturity, vol, cashPayoff,volStrikes,volMaturities,volMatrix){
  
  type <- match.arg(type, c("call", "put"))
  
  # Check for correct matrix/vector types
  if (!is.matrix(volMatrix)
      || !is.vector(volStrikes)
      || !is.vector(volMaturities)) {
    stop("Vol must be a matrix, maturities/strikes must be vectors",
         call.=FALSE)
  }
  
  # Check that matrix/vectors have compatible dimensions
  if (prod(dim(volMatrix)) != length(volStrikes)*length(volMaturities)) {
    stop("Dimensions of vol matrix not compatible with maturity/strikes vectors",
         call.=FALSE)
  }
  
  val<-oneTouchEngine(type=type,underlying=underlying, strike=strike, 
                        dividendYield=(dividendYield),riskFreeRate=(riskFreeRate), 
                        maturity=maturity, volatility=vol, cashPayoff=cashPayoff,
                        volStrikes,volMaturities,volMatrix)
  class(val) <- c("BinaryOption", "Option")
  return(val)
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
  
  if(barrierUp<barrierDown | barrierUp<0 | barrierDown<0){
    stop("barrierUp must be > barrierDown and both must be > 0",
         call.=FALSE)
  }
  
  val <- doubleKOVannaVolgaEngine(type,barrierUp, barrierDown,underlying,strike,
                                  dividendYield, riskFreeRate,maturity,   vol25Put,
                                  volATM,   vol25Call,volatility)
  class(val) <- c("BarrierOption", "Option")
  val
}



plot.Option <- function(x, ...) {
  warning("No plotting available for class", class(x)[1],"\n")
  invisible(x)
}

print.Option <- function(x, digits=4, ...) {
  cat("Concise summary of valuation for", class(x)[1], "\n")
  print(round(unlist(x[1:7]), digits))
  invisible(x)
}

summary.Option <- function(object, digits=4, ...) {
  cat("Detailed summary of valuation for", class(object)[1], "\n")
  print(round(unlist(object[1:7]), digits))
  cat("with parameters\n")
  print(unlist(object[["parameters"]]))
  invisible(object)
}