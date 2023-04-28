# Build against mingw-w64 build of quantlib
if (!file.exists("../windows/quantlib-1.29/include/ql/quantlib.hpp")) {
  download.file("https://github.com/rwinlib/quantlib/archive/v1.29.zip", "quantlib-1.29.zip", quiet = TRUE)
  dir.create("../windows", showWarnings = FALSE)
  unzip("quantlib-1.29.zip", exdir = "../windows")
  unlink("quantlib-1.29.zip")
  unzip("../windows/quantlib-1.29/lib.zip", exdir = "../windows/quantlib-1.29")
}
