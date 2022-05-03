## Emacs, make this -*- mode: sh; -*-

FROM r-base:latest

LABEL org.opencontainers.image.licenses="GPL-2.0-or-later" \
      org.opencontainers.image.source="https://github.com/eddelbuettel/rquantlib" \
      org.opencontainers.image.authors="Dirk Eddelbuettel <edd@debian.org>"

RUN apt-get update -qq \
	&& apt-get install -t unstable -y --no-install-recommends \
		r-cran-rcpp \
                r-cran-remotes \
                r-cran-tinytest \
                r-cran-zoo \
		libquantlib0-dev \
                ccache \
        && mkdir ~/.R \
        && echo _R_CHECK_FORCE_SUGGESTS_=FALSE > ~/.R/check.Renviron \
        && echo CC=ccache gcc > ~/.R/Makevars \
        && echo CXX=ccache g++ >> ~/.R/Makevars \
        && echo CXX11=ccache g++ >> ~/.R/Makevars \
        && echo CXX14=ccache g++ >> ~/.R/Makevars

CMD ["bash"]
