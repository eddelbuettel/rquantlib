## RQuantLib  [![Build Status](https://travis-ci.org/eddelbuettel/rquantlib.svg)](https://travis-ci.org/eddelbuettel/rquantlib) [![License](http://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl-2.0.html) [![CRAN](http://www.r-pkg.org/badges/version/RQuantLib)](http://cran.r-project.org/package=RQuantLib) [![Downloads](http://cranlogs.r-pkg.org/badges/RQuantLib?color=brightgreen)](http://www.r-pkg.org/pkg/RQuantLib)

### About

The RQuantLib package makes parts of
[QuantLib](https://github.com/lballabio/quantlib) visible to the R
user. Currently a number option pricing functions are included, both
vanilla and exotic, as well as a broad range of fixed-income
functions. Also included are general calendaring and holiday
utilities. Further software contributions are welcome.

The QuantLib project aims to provide a comprehensive software framework for
quantitative finance. The goal is to provide a standard open source library
for quantitative analysis, modeling, trading, and risk management of
financial assets.

### Status

The package is actively maintained, and is still being
extended. Contributions are welcome, and initial discussions via
[GitHub issue tickets](https://github.com/eddelbuettel/rquantlib/issues)
are encouraged as suggested in the
[Contributing guide](https://github.com/eddelbuettel/rquantlib/blob/master/Contributing.md).

### Installation 

#### From Source

The package is on [CRAN](https://cran.r-project.org) and can be installed _from source_ as usual:

```{r}
R> install.packages(RQuantLib)
```

#### Windows Binaries

Starting with release 0.4.3, binaries are provided via the [ghrr drat repo](http://ghrr.github.io/drat)
and be installed as

```{r}
R> drat::addRepo("ghrr")       #  maybe use 'install.package("drat")' first 
R> install.packages(RQuantLib)
```

### Authors

Dirk Eddelbuettel, Khanh Nguyen (during 2009-2010) and Terry Leitch (since 2016)

### License

GPL (>= 2)

