
library(shiny)
library(RQuantLib)

shinyServer(function(input, output) {

    ## This data is taken from sample code shipped with QuantLib 0.9.7
    ## from the file Examples/Swap/swapvaluation
    params <- list(tradeDate=as.Date('2004-09-20'),
                   settleDate=as.Date('2004-09-22'),
                   dt=.25,
                   interpWhat="discount",
                   interpHow="loglinear")
    setEvaluationDate(as.Date("2004-09-20"))

    ## We get numerical issue for the spline interpolation if we add
    ## any on of these three extra futures -- the original example
    ## creates different curves based on different deposit, fra, futures
    ## and swap data
    ## Removing s2y helps, as kindly pointed out by Luigi Ballabio
    tsQuotes <- list(d1w = 0.0382,
                     d1m = 0.0372,
                     d3m = 0.0363,
                     d6m = 0.0353,
                     d9m = 0.0348,
                     d1y = 0.0345,
                     fut1=96.2875,
                     fut2=96.7875,
                     fut3=96.9875,
                     fut4=96.6875,
                     fut5=96.4875,
                     fut6=96.3875,
                     fut7=96.2875,
                     fut8=96.0875,
                     ## s2y = 0.037125,
                     s3y = 0.0398,
                     s5y = 0.0443,
                     s10y = 0.05165,
                     s15y = 0.055175)

    times <- seq(0,10,.1)
   
    ## Reactive expression to generate the requested curves.
    data <- reactive({
        params$interpHow <- input$interpolation
        curve <- DiscountCurve(params, tsQuotes, times)
    })

    ## Generate a plot of the data.
    output$plot <- renderPlot({
        interp <- input$interpolation
        crv <- input$curve
        dat <- data()
        plot(dat[["times"]], dat[[crv]],
             type='l', main=paste(interp, crv),
             ylab=crv, xlab="time in years")
    })
  
    ## Generate a summary of the data
    output$summary <- renderPrint({
        dat <- data()
        cat("Return Object Structure\n")
        str(dat)
        cat("\n\nSummary of first four elements\n")
        summary(data.frame(dat[1:4]))
    })
  
    ## Generate an HTML table view of the data
    output$table <- renderTable({
        data.frame(x=data()[1:4])
    })
    
})
