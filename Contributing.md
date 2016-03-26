
## Contributing to RQuantLib

Interested in contributing? Great!  We really welcome bug reports and pull
requests that expand and improve the functionality of RQuantLib from all
contributors. 

### Reporting an Issue

When reporting an issue, the most important thing you can provide is a
[reproducible example](http://www.sscce.org/). Please include the smallest
possible example that illustrates the issue -- when possible, provide a
snippet of C++ code that can be run using `Rcpp::sourceCpp()`; if it's not
possible or feasible to provide such an example, provide clear instructions
on how to reproduce the problem. The less effort it takes to reproduce an
issue, the more likely we will be able to investigate the issue + resolve the
problem. Also see this StackOverflow answer on
[creating a reproducible example](http://stackoverflow.com/a/5963610/143305).

Issues that cannot be reproduced are unlikely to receive attention, as it is
often difficult, if not impossible, to ascertain whether RQuantLib is truly
the culprit, or whether it is underlying calculations in
[QuantLib](http://quantlib.org), or whether it is the Rcpp layer.

Please also supply the output of `sessionInfo()` when reporting an issue.

### Submitting a Pull Request

Considering submitting a pull request? It is strongly recommended that you first
post an issue outlining some motivation for your pull request to ensure that
some discussion around the appropriate resolution for the issue occurs first.

We would prefer it if your PR also included
[unit tests](https://github.com/eddelbuettel/rquantlib/tree/master/inst/unitTests). Additions
to the [ChangeLog](https://github.com/eddelbuettel/rquantlib/blob/master/ChangeLog) and
[NEWS](https://github.com/eddelbuettel/rquantlib/blob/master/inst/NEWS.Rd) are also
appreciated.

### Asking Questions

Please consider directing general questions to the
[rquantlib-devel](http://lists.r-forge.r-project.org/pipermail/rquantlib-devel/)
mailing list (preferred, note that only subscribers can post), or alternatively
post a question on
[Stack Overflow](http://stackoverflow.com) using these two 
`[quantlib] [r]` tags. 

### Copyright

RQuantLib is released as a
[CRAN](https://cran.r-project.org/package=RQuantLib) package, licensed under
the [GPL-2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) (or
later) [Open Source](https://opensource.org) /
[Free Software](http://www.gnu.org/philosophy/free-sw.en.html) license --
just like R itself -- while QuantLib is released under a more permissive
modified BSD [license](http://quantlib.org/license.shtml). However,
aggregated works, i.e. code linking to R such as RQuantLib will always be
under the same license as R (which Rcpp and RQuantLib share).  Your
contribution has to be under a compatible licence.  And to simplify matters,
we strongly prefer contributions under the same terms of the GPL (>= 2) and
will in general assume this license unless explicitly stated otherwise.
