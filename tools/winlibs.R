# Build against mingw-w64 build of quantlib
if (!file.exists("../windows/quantlib-1.16/include/ql/quantlib.hpp")) {
  if (getRversion() < "3.3.0") setInternet2()
  download.file("https://github.com/rwinlib/quantlib/archive/v1.16.zip", "quantlib-1.16.zip", quiet = TRUE)
  dir.create("../windows", showWarnings = FALSE)
  unzip("quantlib-1.16.zip", exdir = "../windows")
  unlink("quantlib-1.16.zip")
  
  # Static libraries for the told toolchain
  unzip("../windows/quantlib-1.16/lib-4.9.3.zip", exdir = "../windows/quantlib-1.16")
  
  # Static libraries for the new toolchain
  if (getRversion() > "3.6.99"){
    unzip("../windows/quantlib-1.16/lib.zip", exdir = "../windows/quantlib-1.16")
  }
}
