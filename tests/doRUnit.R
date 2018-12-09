## doRUnit.R --- Run RUnit tests
##
## with credits to package fUtilities in RMetrics
## which credits Gregor Gojanc's example in CRAN package  'gdata'
## as per the R Wiki http://wiki.r-project.org/rwiki/doku.php?id=developers:runit
## and changed further by Martin Maechler
## and more changes by Murray Stokely in HistogramTools
## and then used adapted in RProtoBuf
## and now used in Rcpp* and here
##
## (Initially) inAdapted for RQuantLib by Dirk Eddelbuettel, 29 Dec 2007

if (requireNamespace("RUnit", quietly=TRUE) &&
    requireNamespace("RQuantLib", quietly=TRUE)) {

    library(RUnit)
    library(RQuantLib)

    ## Define tests
    testSuite <- defineTestSuite(name="RQuantLib Unit Tests",
                                 dirs=system.file("unitTests", package = "RQuantLib"),
                                 testFuncRegexp = "^[Tt]est.+")

    ## without this, we get (or used to get) unit test failures
    Sys.setenv("R_TESTS"="")

    .onWindows <- .Platform$OS.type == "windows"

    tests <- runTestSuite(testSuite)	# Run tests

    printTextProtocol(tests)			# Print results

    ## Return success or failure to R CMD CHECK
    if (getErrors(tests)$nFail > 0) stop("TEST FAILED!")
    if (getErrors(tests)$nErr > 0) stop("TEST HAD ERRORS!")
    if (getErrors(tests)$nTestFunc < 1) stop("NO TEST FUNCTIONS RUN!")
}
