##  RQuantLib -- R interface to the QuantLib libraries
##
##  Copyright (C) 2010        Dirk Eddelbuettel and Khanh Nguyen
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

hullWhiteCalibrateUsingCap <- function(termStrc, capHelpers,
                                       index, evaluationDate) {
    capData <- capHelpers$data
    ibor <- index$term
    val <- calibrateHullWhiteUsingCapsEngine(termStrc$table$date,
                                             termStrc$table$zeroRates,
                                             capData,
                                             ibor$table$date,
                                             ibor$table$zeroRates,
                                             index$type,
                                             evaluationDate)
}

hullWhiteCalibrateUsingSwap <- function(termStrc, swapHelpers,
                                        index, evaluationDate) {
    swapData <- swapHelpers$data
    ibor <- index$term
    val <- calibrateHullWhiteUsingSwapsEngine(termStrc$table$date,
                                              termStrc$table$zeroRates,
                                              swapData,
                                              ibor$table$date,
                                              ibor$table$zeroRates,
                                              index$type,
                                              evaluationDate)
}
