## Emacs, make this -*- mode: sh; -*-

FROM r-base:latest

LABEL org.label-schema.license="GPL-2.0" \
      org.label-schema.vcs-url="https://github.com/eddelbuettel/rquantlib" \
      maintainer="Dirk Eddelbuettel <edd@debian.org>"

RUN apt-get update \
	&& apt-get install -y --no-install-recommends \
		r-cran-rcpp \
                r-cran-zoo \
                r-cran-runit \
		libquantlib0-dev \
                ccache \
        && mkdir ~/.R \
        && echo _R_CHECK_FORCE_SUGGESTS_=FALSE > ~/.R/check.Renviron \
        && echo CC=ccache gcc > ~/.R/Makevars \
        && echo CXX=ccache g++ >> ~/.R/Makevars \
        && echo CXX11=ccache g++ >> ~/.R/Makevars \
        && echo CXX14=ccache g++ >> ~/.R/Makevars 

CMD ["bash"]
