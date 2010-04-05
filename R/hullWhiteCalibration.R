hullWhiteCalibrateUsingCap <- function(termStrc, capHelpers,
                               index, evaluationDate){
  capData <- capHelpers$data

  indexparams <- list(type=index$type);
  ibor <- index$term

  val <- .Call("QL_HullWhiteCalibrationUsingCap",
               c(termStrc$table$date),
               termStrc$table$zeroRates,
               capData, 
               c(ibor$table$date),
               ibor$table$zeroRates,
               indexparams, evaluationDate)
}

hullWhiteCalibrateUsingSwap <- function(termStrc, swapHelpers,
                               index, evaluationDate){
  swapData <- swapHelpers$data

  indexparams <- list(type=index$type);
  ibor <- index$term

  val <- .Call("QL_HullWhiteCalibrationUsingSwap",
               c(termStrc$table$date),
               termStrc$table$zeroRates,
               swapData, 
               c(ibor$table$date),
               ibor$table$zeroRates,
               indexparams, evaluationDate)
}
