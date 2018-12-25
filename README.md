## RQuantLib  [![Build Status](https://travis-ci.org/eddelbuettel/rquantlib.svg)](https://travis-ci.org/eddelbuettel/rquantlib) [![License](http://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl-2.0.html) [![CRAN](http://www.r-pkg.org/badges/version/RQuantLib)](https://cran.r-project.org/package=RQuantLib) [![Dependencies](https://tinyverse.netlify.com/badge/RQuantLib)](https://cran.r-project.org/package=RQuantLib) [![Downloads](http://cranlogs.r-pkg.org/badges/RQuantLib?color=brightgreen)](http://www.r-pkg.org/pkg/RQuantLib)

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

The package is on [CRAN](https://cran.r-project.org) and can be installed as usual:

```{r}
install.packages("RQuantLib")
```

Windows binary packages are available via [CRAN](https://cran.r-project.org) thanks to the
library provided by [Joshua Ulrich](https://about.me/joshuaulrich) via the
[rwinlib/quantlib](https://github.com/rwinlib/quantlib) repository.

Binaries for macOS _could_ be provided if we had a similar binary library, ideally via the
[s-u/recipes](https://github.com/s-u/recipes) repository.  Some efforts are under way and
coordinated on the [rquantlib mailing list](http://rquantlib.groups.io) so stay tuned.

For more OS-specific installation options, please see [the wiki](https://github.com/eddelbuettel/rquantlib/wiki/RQuantLib).

### Support

Come to the friendly and low-volume [rquantlib mailing list](http://rquantlib.groups.io) for help.

### Authors

Dirk Eddelbuettel, Khanh Nguyen (during 2009-2010) and Terry Leitch (since 2016)

### License

GPL (>= 2)

