library(shiny)

shinyUI(fluidPage(
    
    ## Application title
    titlePanel("Simple DiscountCurve Example from RQuantLib"),
  
    ## Sidebar with controls to select parameters
    sidebarLayout(
        sidebarPanel(
            radioButtons("interpolation", "Interpolation type:",
                         c("loglinear" = "loglinear",
                           "linear" = "linear",
                           "spline" = "spline")),

            br(),

            radioButtons("curve", "Curve type:",
                         c("forwards" = "forwards",
                           "zero rates" = "zerorates",
                           "discounts" = "discounts"))
        ),
        
        ## Show a tabset that includes a plot, summary, and table view
        mainPanel(
            tabsetPanel(type = "tabs", 
                        tabPanel("Plot", plotOutput("plot")), 
                        tabPanel("Summary", verbatimTextOutput("summary")), 
                        tabPanel("Table", tableOutput("table"))
                        )
        )
    )
))
