if(!file.exists("../windows/quantlib/include/ql/quantlib.hpp")){
  unlink("../windows", recursive = TRUE)
  url <- if(grepl("aarch", R.version$platform)){
    "https://github.com/r-windows/bundles/releases/download/quantlib-1.32/quantlib-1.32-clang-aarch64.tar.xz"
  } else if(grepl("clang", Sys.getenv('R_COMPILED_BY'))){
    "https://github.com/r-windows/bundles/releases/download/quantlib-1.32/quantlib-1.32-clang-x86_64.tar.xz"
  } else if(getRversion() >= "4.3") {
    "https://github.com/r-windows/bundles/releases/download/quantlib-1.32/quantlib-1.32-ucrt-x86_64.tar.xz"
  } else {
    "https://github.com/rwinlib/quantlib/archive/v1.29.tar.gz"
  }
  download.file(url, basename(url), quiet = TRUE)
  dir.create("../windows", showWarnings = FALSE)
  untar(basename(url), exdir = "../windows", tar = 'internal')
  unlink(basename(url))
  setwd("../windows")
  file.rename(list.files(), 'quantlib')
  if(file.exists("quantlib/lib.zip")) unzip("quantlib/lib.zip", exdir = "quantlib")
}
