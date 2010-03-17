## doRUnit.R --- Run RUnit tests
##
## Origianlly follows Gregor Gojanc's example in CRAN package  'gdata'
## and the corresponding section in the R Wiki:
## http://wiki.r-project.org/rwiki/doku.php?id=developers:runit
##
## Changed by Martin Maechler for Rmetrics, making them "runnable"
## for *installed* packages without ./tests/ directory by placing
## the bulk of the code e.g. in  ../inst/unitTests/runTests.R
##
## Adapted for RQuantLib by Dirk Eddelbuettel, 29 Dec 2007

if (require("RUnit", quietly=TRUE)) {

    #wd <- getwd()
    #pkg <- sub("\\.Rcheck$", '', basename(dirname(wd))) 	# sub out trailing .Rcheck
    #pkg <- gsub("[0-9.-]*$", '', pkg)				# sub out -0.1.2 number

    pkg <- "RQuantLib"						# cannot read from current dir in SVN tree

    library(package=pkg, character.only=TRUE)

    path <- system.file("unitTests", package = pkg)

    stopifnot(file.exists(path), file.info(path.expand(path))$isdir)

    source(file.path(path, "runTests.R"), echo = TRUE)
}

