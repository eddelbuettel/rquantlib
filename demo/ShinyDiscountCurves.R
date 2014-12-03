
## use shiny, if installed to show Shiny App installed with package
if (require("shiny")) {
    library(RQuantLib)
    runApp(system.file("shiny", "DiscountCurve", package="RQuantLib"),
           port=8765
           ## add   host="0.0.0.0"   if access from outside current machine needed
           )
}
