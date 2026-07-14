
if (requireNamespace("tinytest", quietly=TRUE)) {
    ## there are several more granular ways to test files in a tinytest directory,
    ## see its package vignette; tests can also run once the package is installed
    ## using the same command `test_package(pkgName)`, or by directory or file
    tinytest::test_package("RQuantLib", ncpu=getOption("Ncpus", 1))
}
