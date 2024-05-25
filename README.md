## RQuantLib: R Interface to the [QuantLib](https://www.quantlib.org) Library

[![CI](https://github.com/eddelbuettel/rquantlib/workflows/ci/badge.svg)](https://github.com/eddelbuettel/rquantlib/actions?query=workflow%3Aci)
[![License](http://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](http://www.gnu.org/licenses/gpl-2.0.html)
[![CRAN](http://www.r-pkg.org/badges/version/RQuantLib)](https://cran.r-project.org/package=RQuantLib)
[![r-universe](https://eddelbuettel.r-universe.dev/badges/RQuantLib)](https://eddelbuettel.r-universe.dev/RQuantLib)
[![Debian package](https://img.shields.io/debian/v/r-cran-rquantlib/sid?color=brightgreen)](https://packages.debian.org/sid/r-cran-rquantlib)
[![Dependencies](https://tinyverse.netlify.app/badge/RQuantLib)](https://cran.r-project.org/package=RQuantLib)
[![Downloads](http://cranlogs.r-pkg.org/badges/RQuantLib?color=brightgreen)](https://www.r-pkg.org:443/pkg/RQuantLib)
[![Last Commit](https://img.shields.io/github/last-commit/eddelbuettel/rquantlib)](https://github.com/eddelbuettel/rquantlib)

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

The package is always tested against the most recent version of QuantLib itself, and
it generally updated (should a change be needed) when QuantLib releases updates.

### Installation

#### From Source

The package is on [CRAN](https://cran.r-project.org) and can be installed as usual:

```{r}
install.packages("RQuantLib")
```

Windows binary packages are available via [CRAN](https://cran.r-project.org) thanks to the work by
[Joshua Ulrich](https://about.me/joshuaulrich) and [Jeroen Ooms](https://github.com/jeroen)
providing a QuantLib binary for the CRAN builders. Similarly, binaries for macOS _can_ be provided
when a suitable macOS library of QuantLib is prepared, possibly via
[s-u/recipes](https://github.com/R-macos/recipes).  If and when these binary libraries may be outdated,
please raise the issue on the [rquantlib mailing list](http://rquantlib.groups.io).

For more OS-specific installation options, please see [the wiki](https://github.com/eddelbuettel/rquantlib/wiki/RQuantLib).

### Support

Come to the friendly and low-volume [rquantlib mailing list](http://rquantlib.groups.io) for help.

### See Also

The [qlcal](https://github.com/qlcal/qlcal-r) R package provides the calendaring functionality of
[QuantLib](https://github.com/lballabio/quantlib) in a standalone R package that does not require
[QuantLib](https://github.com/lballabio/quantlib).

### Authors

Dirk Eddelbuettel, Khanh Nguyen (during 2009-2010) and Terry Leitch (since 2016)

### License

GPL (>= 2)
