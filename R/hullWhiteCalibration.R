
hullWhiteCalibrate <- function(termStrc, capHelpers,
                               index, evaluationDate){
  capData <- capHelpers$data

  indexparams <- list(type=index$type);
  ibor <- index$term

  val <- .Call("QL_HullWhiteCalibration",
               c(termStrc$table$date),
               termStrc$table$zeroRates,
               capData, 
               c(ibor$table$date),
               ibor$table$zeroRates,
               indexparams, evaluationDate)
}
