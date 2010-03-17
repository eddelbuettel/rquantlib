## borrowed from Rmetrics' unit testing framework
## removed test for interactive()
##
## Dirk Eddelbuettel, 29 Dec 2007

pkg <- "RQuantLib"

if (require("RUnit", quietly = TRUE)) {
    library(package=pkg, character.only = TRUE)
    if(!(exists("path") && file.exists(path)))
        path <- system.file("unitTests", package = pkg)

    ## Define tests
    testSuite <- defineTestSuite(name = paste(pkg, "unit testing"), dirs = path)

    ## Run tests
    tests <- runTestSuite(testSuite)

    if(file.access(path, 02) != 0) {
        ## cannot write to path -> use writable one
        tdir <- tempfile(paste(pkg, "unitTests", sep="_"))
        dir.create(tdir)
        pathReport <- file.path(tdir, "report")
        cat("RUnit reports are written into ", tdir, "/report.(txt|html)", sep = "")
    } else {
        pathReport <- file.path(path, "report")
    }

    ## Print Results:
    printTextProtocol(tests)
    printTextProtocol(tests, fileName = paste(pathReport, ".txt", sep = ""))

    ## Print HTML Version to a File:
    printHTMLProtocol(tests, fileName = paste(pathReport, ".html", sep = ""))

    ## stop() if there are any failures i.e. FALSE to unit test.
    ## This will cause R CMD check to return error and stop
    if(getErrors(tests)$nFail > 0) {
        stop("one of the unit tests failed")
    }
} else {
    cat("R package 'RUnit' cannot be loaded -- no unit tests run\n", "for package", pkg,"\n")
}
