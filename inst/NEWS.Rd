\name{NEWS}
\title{News for Package 'RQuantLib'}
\newcommand{\cpkg}{\href{http://CRAN.R-project.org/package=#1}{\pkg{#1}}}

\section{Changes in RQuantLib version 0.4.0 [UNRELEASED]}{
  \itemize{
    \item All function interfaces have been rewritten using \cpkg{Rcpp}
    Attributes. No \code{SEXP} remain in the function signatures. This
    make the code shorter, more readable and more easily extensible.
    \item The header files have been reorganized so that plugin use is possible.
    \item A plugin has been added, allowing more ad-hoc use via Rcpp
    Attributes.
    \item Several Fixed Income functions have been added.
    \item This NEWS file has been added. Better late than never.
  }
}
