## RQuantLib -- R interface to the QuantLib libraries
##
## Copyright (C) 2010        Dirk Eddelbuettel and Khanh Nguyen
##
## $Id$
##
## This file is part of the RQuantLib library for GNU R.
## It is made available under the terms of the GNU General Public
## License, version 2, or at your option, any later version,
## incorporated herein by reference.
##
## This program is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
## PURPOSE.  See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public
## License along with this program; if not, write to the Free
## Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
## MA 02111-1307, USA

hullWhiteCalibrateUsingCap <- function(termStrc, capHelpers,
                                       index, evaluationDate) {
  capData <- capHelpers$data

  indexparams <- list(type=index$type);
  ibor <- index$term

  val <- .Call("HullWhiteCalibrationUsingCap",
               termStrc$table$date,
               termStrc$table$zeroRates,
               capData,
               ibor$table$date,
               ibor$table$zeroRates,
               indexparams,
               evaluationDate)
}

hullWhiteCalibrateUsingSwap <- function(termStrc, swapHelpers,
                               index, evaluationDate){
  swapData <- swapHelpers$data

  indexparams <- list(type=index$type);
  ibor <- index$term

  val <- .Call("HullWhiteCalibrationUsingSwap",
               termStrc$table$date,
               termStrc$table$zeroRates,
               swapData,
               ibor$table$date,
               ibor$table$zeroRates,
               indexparams,
               evaluationDate)
}
