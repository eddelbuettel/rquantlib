## RQuantLib -- R interface to the QuantLib libraries
##
##  Copyright (C) 2014         Michele Salvadore and Dirk Eddelbuettel
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

Schedule <- function(params){
    UseMethod("Schedule")
}

Schedule.default <- function(params) {
    val <- 0
    
    # check schedule params
    if (is.null(params$effectiveDate)){
        stop("schedule effective date undefined.")
    }
    if (is.null(params$maturityDate)){
        stop("schedule maturity date undefined.")
    }
    if (is.null(params$period)) params$period <- 'Semiannual'
    if (is.null(params$calendar)) params$calendar <- 'TARGET'
    if (is.null(params$businessDayConvention)) params$businessDayConvention <- 'Following'
    if (is.null(params$terminationDateConvention)) params$terminationDateConvention <- 'Following'
    if (is.null(params$dateGeneration)) params$dateGeneration <- 'Backward'
    if (is.null(params$endOfMonth)) params$endOfMonth <- 0
    params <- matchParams(params)
    
    CreateSchedule(params)
}
