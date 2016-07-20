
# This is the user-interface definition of a Shiny web application.
# You can find out more about building applications with Shiny here:
#
# http://shiny.rstudio.com
#

library(shiny)

shinyUI(fluidPage(

  # Application title
  titlePanel("SABR Vol Cube Tool"),
  checkboxGroupInput("expiries", label = h3("Option Expiries"), inline=T,
                     choices=list("1M"="1M","3M"="3M","6M"="6M","1Y"="1Y" , "2Y"="2Y", "3Y"="3Y",  
                                   "4Y"="4Y",  "5Y"="5Y",  "6Y"="6Y",  "7Y"="7Y",  "8Y"="8Y",  "9Y"="9Y","10Y"="10Y"),
                    # selected = c("1M","3M","6M","1Y" , "2Y", "3Y",  "4Y",  "5Y",  "6Y",  "7Y",  "8Y",  "9Y","10Y")),
  selected = c( "3Y",  "4Y","5Y")),
  checkboxGroupInput("tenors", label = h3("Swap Tenors"), inline=T,
                     choices=list("1Y"="1Y" , "2Y"="2Y",  "5Y"="5Y","10Y"="10Y","15Y"="15Y","20Y"="20Y","30Y"="30Y"),
                     #selected = c("1Y" , "2Y",  "5Y", "10Y","15Y","20Y","30Y")),
  selected = c( "5Y")),
  checkboxInput("bermudan", label = "Bermudan Exercise?", value = F), 
  

    # Show a plot of the generated distribution
    mainPanel(
        rHandsontableOutput(outputId="table")
    )
  
))
