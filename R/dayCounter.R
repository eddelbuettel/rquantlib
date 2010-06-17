dayCount <- function(startDates, endDates, dayCounters){

  val <- .Call('dayCount', startDates, endDates, dayCounters,
               PACKAGE="RQuantLib")
  val
}

yearFraction <- function(startDates, endDates, dayCounters){
  val <- .Call('yearFraction', startDates, endDates, dayCounters,
               PACKAGE="RQuantLib")
  val
}
