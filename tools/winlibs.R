# Build against mingw-w64 build of quantlib
if (!file.exists("../windows/quantlib-1.14/include/ql/quantlib.hpp")) {
  if (getRversion() < "3.3.0") setInternet2()
  download.file("https://github.com/rwinlib/quantlib/archive/v1.14.zip", "lib.zip", quiet = TRUE)
  dir.create("../windows", showWarnings = FALSE)
  unzip("lib.zip", exdir = "../windows")
  unzip("../windows/quantlib-1.14/lib.zip", exdir = "../windows/quantlib-1.14")
  unlink("lib.zip")
}
