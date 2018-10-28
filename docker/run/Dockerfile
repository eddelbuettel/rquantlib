## Emacs, make this -*- mode: sh; -*-

FROM rquantlib/ci

LABEL org.label-schema.license="GPL-2.0" \
      org.label-schema.vcs-url="https://github.com/eddelbuettel/rquantlib" \
      maintainer="Dirk Eddelbuettel <edd@debian.org>"

## If on CRAN, install the latest version from CRAN
#RUN install.r ...

## Alternatively, install from Github (after first installing remotes)
RUN install.r remotes && installGithub.r eddelbuettel/rquantlib

CMD ["bash"]

