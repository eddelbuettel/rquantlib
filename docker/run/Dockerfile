## Emacs, make this -*- mode: sh; -*-

FROM rquantlib/ci

LABEL org.opencontainers.image.licenses="GPL-2.0-or-later" \
      org.opencontainers.image.source="https://github.com/eddelbuettel/rquantlib" \
      org.opencontainers.image.authors="Dirk Eddelbuettel <edd@debian.org>"

## If on CRAN, install the latest version from CRAN
#RUN install.r ...

## Alternatively, install from Github (having first installed remotes)
RUN installGithub.r eddelbuettel/rquantlib

CMD ["bash"]
