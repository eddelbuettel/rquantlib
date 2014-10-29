
##  RQuantLib -- R interface to the QuantLib libraries
##
##  Copyright (C) 2014  Dirk Eddelbuettel <edd@debian.org>
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

unitTestSetup <- function(file = NULL, packages = NULL) {
    function(){
        if (!is.null(packages)) {
            for (p in packages) {
                suppressMessages(require(p, character.only = TRUE))
            }
        }
        if (!is.null(file)) {
            if (exists("pathRQuantLibTests")) {
                sourceCpp(file.path(get("pathRQuantLibTests"), "cpp", file))
            } else if (file.exists(file.path("cpp", file))) {
                sourceCpp(file.path("cpp", file))
            } else {
                sourceCpp(system.file("unitTests", "cpp", file, package="RQuantLib"))
            }
        }
    }
}

