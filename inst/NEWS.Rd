\name{NEWS}
\title{News for Package \pkg{RQuantLib}}
\newcommand{\ghpr}{\href{https://github.com/eddelbuettel/rquantlib/pull/#1}{##1}}
\newcommand{\ghit}{\href{https://github.com/eddelbuettel/rquantlib/issues/#1}{##1}}
\newcommand{\ghrp}{\href{https://github.com/#1}{##1}}

\section{Changes in \pkg{RQuantLib} version 0.4.23 (2024-07-23)}{
  \itemize{
    \item Adjustments for QuantLib 1.35 and removal of deprecated code
    (in utility functions and dividend case of vanilla options)
    \item Adjustments for new changes in QuantLib 1.35
    \item Refactoring most C++ files making more use of both \code{auto} and
    \code{make_shared} to simplify and shorten expressions
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.22 (2024-04-25)}{
  \itemize{
    \item Small code cleanup removing duplicate R code
    \item Small improvements to C++ compilation flags
    \item Robustify internal version comparison to accommodate RC releases
    \item Adjustments to two C++ files for QuantLib 1.34
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.21 (2024-02-01)}{
  \itemize{
    \item Generalize macOS build to universal build (Jeroen in \ghpr{179})
    \item Generalize Windows build to arm64 (Jeroen in \ghpr{181})
    \item Generalize version string use to support \code{cmake} use (Jeroen
    in \ghpr{181} fixing \ghit{180})
    \item Minor update to `ci.yaml` github action (Dirk)
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.20 (2023-11-26)}{
  \itemize{
    \item Correct three help pages with stray curly braces
    \item Correct two printf format strings
    \item Comment-out explicit selection of C++14
    \item Wrap one example inside \code{if (interactive())} to not exceed
    total running time limit at CRAN checks
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.19 (2023-08-07)}{
  \itemize{
    \item Three calendaring / schedule tests were adjusted for slightly
    changed values under QuantLib 1.31
    \item Several checks in the \code{configure} script have been updated to
    reflect current versions of packages.
    \item Five examples no longer run because, even while extremely short,
    use of (too many default) threads was seen.
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.18 (2023-05-01)}{
  \itemize{
    \item Use of several \code{rgl} functions was updated to a new naming
    scheme in the package (kindly contributed by Duncan Murdoch in \ghpr{174})
    \item A default argument is now given for option surface plots
    \item Changed call from SwaptionVolCube1 to SabrSwaptionVolatilityCube
    (conditional on using QuantLib 1.30 or later)
    \item Some other deprecation warnings were tweaked as in QL test file
    \item Builds for macOS and Windows were updated with more library build
    (changes kindly contributed by Jeron Ooms in \ghpr{176} and \ghpr{175})
    \item Some remaining \code{throw} calls were replace by \code{Rcpp::stop}
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.17 (2022-10-25)}{
  \itemize{
    \item Switch compilation to C++14 which is required by QuantLib 1.28
    and, while standard with R 4.2.*, may be needed for R 4.1.*
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.16 (2022-05-05)}{
  \itemize{
    \item Documentationn for daycounters was updated and extended (Kai Lin)
    \item Deprecated daycounters were approtiately updated (Kai Lin)
    \item One example parameterization was changed to avoid error (Dirk)
    \item The Docker files were updated
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.15 (2022-01-19)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
    \itemize{
      \item Calendar support has been updated and completed to current
      QuantLib standards (Dirk in \ghpr{161})
      \item More daycounters have been added (Kai Lin in \ghpr{163}
      fixing \ghit{162}, \ghit{164})
      \item The bonds pricers were update to changes in QuantLib 1.25 (Dirk)
    }
    \item Changes in \pkg{RQuantLib} package and setup:
    \itemize{
      \item Some package metadata was removed from the README.md (Dirk)
    }
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.14 (2021-10-06)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
    \itemize{
      \item Several new calendars were added (Dirk in \ghpr{159} closing
      \ghit{155})
    }
    \item Changes in \pkg{RQuantLib} package and setup:
    \itemize{
      \item Docker containers are now updated on a monthly schedule via
      GitHub Actions
      \item The \code{configure} files were updated to the standard of
      version 2.69 following a CRAN request
    }
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.13 (2021-09-02)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
    \itemize{
      \item Small updates to \code{DESCRIPTION} and \code{README.md}
      \item Switch CI use to \href{https://eddelbuettel.github.io/r-ci/}{r-ci}
      \item An eleven-year old typo was correct in a manual page (Dirk in
      \ghpr{156} fixing \ghit{155} with thanks to @klin133)
      \item Daycount convention calls deprecated in QuantLib 1.23 were
      updated (Dirk in \ghpr{157})
      \item Updated remaining http:// URLs in documentation to https://
    }
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.12 (2020-04-01)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
    \itemize{
      \item Calls deprecated-in-QuantLib 1.18 were updated (Dirk in
      \ghpr{146}).
    }
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.11 (2020-01-15)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
    \itemize{
      \item The 'Null' calendar without weekends or holidays is now
      recognized.
      \item The package now uses \pkg{tinytest} for unit tests (Dirk in
      \ghpr{140}).
      \item Calls deprecated-in-QuantLib 1.17 were updated (Dirk in
      \ghpr{144}).
    }
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.10 (2019-08-07)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} build system:
    \itemize{
      \item The \code{src/Makevars.win} and \code{tools/winlibs.R} file
      get QuantLib 1.16 for either toolchain (Jeroes in \ghpr{136}).
      \item The custom Docker container now downloads QuantLib from
      Debian unstable to get release 1.16 (from yesterday, no less)
    }
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.9 (2019-05-15)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
    \itemize{
      \item Completed switch to \code{QuantLib::ext} namespace wrappers
      for either \code{shared_ptr} use started in 0.4.8.
    }
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.8 (2019-03-17)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
    \itemize{
      \item Source code supports Boost \code{shared_ptr} and C+11
      \code{shared_ptr} via \code{QuantLib::ext} namespace like upstream.
    }
    \item Changes in \pkg{RQuantLib} build system:
    \itemize{
      \item The \code{configure.ac} file no longer upsets \code{R CMD
        check}; the change does not actually change \code{configure}.
    }
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.7 (2018-12-10)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} tests:
    \itemize{
      \item Thanks to the updated \ghrp{rwinlib/quantlib} Windows
      library provided by Josh, all tests that previously exhibited
      issues have been re-enabled (Dirk in \ghpr{126}).
    }
    \item Changes in \pkg{RQuantLib} documentation:
    \itemize{
      \item The \code{CallableBonds} example now sets an evaluation date
      (\ghit{124}).
      \item Thanks to the updated \ghrp{rwinlib/quantlib} Windows
      library provided by Josh, examples that were set to \code{dontrun}
      are re-activated (Dirk in \ghpr{126}). \code{AffineSwaption}
      remains the sole holdout.
    }
    \item Changes in \pkg{RQuantLib} build system:
    \itemize{
      \item The \code{src/Makevars.win} file was updated to reflect the
      new layout used by the upstream build.
      \item The \code{-DBOOST_NO_AUTO_PTR} compilation flag is now set.
    }
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.6 (2018-11-25)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
    \itemize{
      \item The code was updated for release 1.14 of QuantLib.
      \item The code was updated for Boost 1.67 or later (\ghpr{120}
      fixing \ghit{119}, Debian bug report #911957) to support
      \item Fewer examples and tests are running on Windows.
      \item Corrected bond function documentation to use
      \code{dayCounter} instead of \code{accruedDayCounter}.
      \item Two new functions \code{addHolidays} and
      \code{removeHolidays} enable custom local holiday management
      (\ghit{115}).
      \item Corrected bond function documentation to use
      \code{dayCounter} instead of \code{accruedDayCounter}.
      \item Travis CI now uses a custom Docker container, also added a
      deployment container building on the testing container.
    }
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.5 (2018-08-10)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
    \itemize{
      \item The old \code{rquantlib.h} header is deprecated and moved to
      a subdirectory. (Some OS confuse it with \code{RQuantLib.h} which
      Rcpp Attributes like to be the same name as the package.)
      (Dirk in \ghpr{100} addressing \ghit{99}).
      \item The files in \code{src/} now include
      \code{rquantlib_internal.h} directly.
      \item Several \sQuote{unused variable} warnings have been taken
      care of.
      \item The Windows build has been updated, and now uses an external
      QuantLib library from 'rwinlib' (Jeroen Ooms in \ghpr{105}).
      \item Three curve-building example are no longer running by
      default as win32 has seen some numerical issues.
      \item Two \code{Rcpp::compileAttributes} generated files have been
      updated.
    }
  }
}

\section{Changes in \pkg{RQuantLib} version 0.4.4 (2017-11-07)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
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

\section{Changes in \pkg{RQuantLib} version 0.4.3 (2016-08-19)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
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

\section{Changes in \pkg{RQuantLib} version 0.4.2 (2015-12-30)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
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

\section{Changes in \pkg{RQuantLib} version 0.4.1 (2015-09-11)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
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

\section{Changes in \pkg{RQuantLib} version 0.4.0 (2014-12-01)}{
  \itemize{
    \item Changes in \pkg{RQuantLib} code:
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
    \item Changes in \pkg{RQuantLib} tests:
    \itemize{
      \item New unit tests for dates have been added.
      \item C++ code for the unit tests has also been converted to
      \CRANpkg{Rcpp} Attributes use; a helper function
      \code{unitTestSetup()} has been added.
      \item Continuous Integration via Travis is now enabled from the
      GitHub repo.
    }
    \item Changes in \pkg{RQuantLib} documentation:
    \itemize{
      \item This NEWS file has been added. Better late than never, as
      they say.
    }
  }
}
