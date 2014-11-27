\name{NEWS}
\title{News for Package 'RQuantLib'}
\newcommand{\cpkg}{\href{http://CRAN.R-project.org/package=#1}{\pkg{#1}}}

\section{Changes in RQuantLib version 0.4.0 [UNRELEASED]}{
  \itemize{
    \item All function interfaces have been rewritten using \cpkg{Rcpp}
    Attributes. No \code{SEXP} remain in the function signatures. This
    make the code shorter, more readable and more easily extensible.
    \item Plugin support has been added, allowing more ad-hoc use via Rcpp
    Attributes.
    \item The header files have been reorganized so that plugin use is
    possible. An \code{impl.h} files is imported once for each compilation
    unit: for RQuantLib from the file \code{src/dates.cpp} directory, from 
    a sourced file via a \code{#define} set by the plugin wrapper.
    \item Several Fixed Income functions have been added.
    \item New unit tests for dates have been added.
    \item C++ code for the unit tests has also been converted to
    \cpkg{Rcpp} Attributes use.
    \item Continuous Integration via Travis is now enabled from the
    GitHub repo.
    \item This NEWS file has been added. Better late than never, as they say.
  }
}
