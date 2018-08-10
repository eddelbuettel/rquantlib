\name{NEWS}
\title{News for Package \pkg{RQuantLib}}
\newcommand{\ghpr}{\href{https://github.com/eddelbuettel/rquantlib/pull/#1}{##1}}
\newcommand{\ghit}{\href{https://github.com/eddelbuettel/rquantlib/issues/#1}{##1}}

\section{Changes in RQuantLib version 0.4.5 (2018-xx-yy)}{
  \itemize{
    \item Changes in RQuantLib code:
    \itemize{
      \item The old \code{rquantlib.h} header is deprecated and moved to
      a subdirectory. (Some OS confuse it with \code{RQuantLib.h} which
      Rcpp Attributes like to be the same name as the package.)
      (Dirk in \ghpr{100} addressing \ghit{99}).
      \item The files in \code{src/} now include
      \code{rquantlib_internal.h} directly.
      \item Several \sQuote{unused variable} warnings have been taken
      care of.
      \item Three curve-building example are no longer running by
      default as win32 has seen some numerical issues.
      \item Two \code{Rcpp::compileAttributes} generated files have been
      updated. 
    }
  }
}

\section{Changes in RQuantLib version 0.4.4 (2017-11-07)}{
  \itemize{
    \item Changes in RQuantLib code:
    \itemize{
      \item Equity options can now be analyzed via discrete dividends
      through two vectors of dividend dates and values (Francois
      Cocquemas in \ghpr{73} fixing \ghit{72})
      \item Some package and dependency information was updated in files
      \code{DESCRIPTION} and \code{NAMESPACE}.
      \item The new \code{Date(time)Vector} classes introduced with Rcpp
      0.12.8 are now used when available.
      \item Minor corrections were applied to \code{BKTree}, to vanilla
      options for the case of intraday time stamps, to the
      \code{SabrSwaption} documentation, and to bond utilities for the
      most recent QuantLib release.
    }
  }
}

\section{Changes in RQuantLib version 0.4.3 (2016-08-19)}{
  \itemize{
    \item Changes in RQuantLib code:
    \itemize{
      \item Discount curve creation has been made more general by
      allowing additional arguments for day counter and fixed and
      floating frequency (contributed by Terry Leitch in \ghpr{31},
      plus some work by Dirk in \ghpr{32}).
      \item Swap leg parameters are now in combined variable and allow
      textual description (Terry Leitch in \ghpr{34} and \ghpr{35})
      \item BermudanSwaption has been modfied to take option expiration
      and swap tenors in order to enable more general swaption structure
      pricing; a more general search for the swaptions was developed to
      accomodate this. Also, a DiscountCurve is allowed as an
      alternative to market quotes to reduce computation time for a
      portfolio on a given valuation date (Terry Leitch in \ghpr{42}
      closing issue \ghpr{41}).
      \item A new AffineSwaption model was added with similar interface
      to BermudanSwaption but allowing for valuation of a European
      exercise swaption utlizing the same affine methods available in
      BermudanSwaption. AffineSwaption will also value a Bermudan
      swaption, but does not take rate market quotes to build a term
      structure and a DiscountCurve object is required (Terry Leitch in
      \ghpr{43}).
      \item Swap tenors can now be defined up to 100 years (Terry Leitch
      in \ghpr{48} fising issue \ghit{46}).
      \item Additional (shorter term) swap tenors are now defined
      (Guillaume Horel in \ghpr{49}, \ghpr{54}, \ghpr{55}).
      \item New SABR swaption pricer (Terry Leitch in \ghpr{60} and
      \ghpr{64}, small follow-up by Dirk in \ghpr{65}).
      \item Use of Travis CI has been updated and switch to maintained
      fork of deprecated mainline.
    }
  }
}

\section{Changes in RQuantLib version 0.4.2 (2015-12-30)}{
  \itemize{
    \item Changes in RQuantLib code:
    \itemize{
      \item Intra-day times are now available if QuantLib 1.7 or later is
      used, and has been configured with \code{--enable-intraday}
      \item New helper functions \code{getQuantLibVersion()} and
      \code{getQuantLibCapabilties()}
      \item New package startup code detects and warns about outdated
      QuantLib versions, or missing intra-day capability, unless not
      interactive.
      \item The missing \code{Monthly} parameter has been added to
      \code{matchFrequency} (fixing issue ticket \ghit{19})
    }
  }
}

\section{Changes in RQuantLib version 0.4.1 (2015-09-11)}{
  \itemize{
    \item Changes in RQuantLib code:
    \itemize{
      \item A simple \CRANpkg{shiny} application is now included in the
      directory \code{shiny/DiscountCurve/} and accessible via the new
      demo function \code{ShinyDiscountCurve}.
      \item The option surface plotting example in \code{arrays.R} now
      checks for \CRANpkg{rgl} by using \code{requireNamespace}.
      \item The files \code{NAMESPACE} and \code{DESCRIPTION} have been
      updated to reflect all the suggestions of \code{R CMD check}.
      \item The Travis CI tests now use binary Debian packages for all
      package dependencies making the tests a little faster.
    }
  }
}

\section{Changes in RQuantLib version 0.4.0 (2014-12-01)}{
  \itemize{
    \item Changes in RQuantLib code:
    \itemize{
      \item All function interfaces have been rewritten using \CRANpkg{Rcpp}
      Attributes. No \code{SEXP} remain in the function signatures. This
      make the code shorter, more readable and more easily extensible. 
      \item The header files have been reorganized so that plugin use is
      possible. An \code{impl.h} files is imported once for each compilation
      unit: for RQuantLib from the file \code{src/dates.cpp} directory, from 
      a sourced file via a \code{#define} set by the plugin wrapper.
      \item \code{as<>()} and \code{wrap()} converters have added for
      QuantLib Date types.
      \item Plugin support has been added, allowing more ad-hoc use via Rcpp
      Attributes.
      \item Several Fixed Income functions have been added, and/or
      rewritten to better match the QuantLib signatures; this was done
      mostly by Michele Salvadore.
      \item Several Date and Calendar functions have been added.
      \item Calendar support has been greatly expanded thanks to Danilo
      Dias da Silva. 
      \item Exported curve objects are now more parsimonious and advance
      entries in the \code{table} object roughly one business month at a
      time.
      \item The \code{DiscountCurve} and \code{Bond} curve construction
      has been fixed via a corrected evaluation date and omitted the
      two-year swap rate, as suggested by Luigi Ballabio.
      \item The \code{NAMESPACE} file has a tighter rule for export of
      \code{*.default} functions, as suggested by Bill Dunlap
      \item Builds now use OpenMP where available.
      \item The package now depends on QuantLib 1.4.0 or later.
    }
    \item Changes in RQuantLib tests:
    \itemize{
      \item New unit tests for dates have been added.
      \item C++ code for the unit tests has also been converted to
      \CRANpkg{Rcpp} Attributes use; a helper function
      \code{unitTestSetup()} has been added.
      \item Continuous Integration via Travis is now enabled from the
      GitHub repo.
    }
    \item Changes in RQuantLib documentation:
    \itemize{
      \item This NEWS file has been added. Better late than never, as
      they say.
    }
  }
}
