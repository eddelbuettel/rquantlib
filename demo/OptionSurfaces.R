
## RQuantLib Demo for (European) Option surfaces
## Dirk Eddelbuettel, September 2005
## $Id: OptionSurfaces.R,v 1.1 2005/10/12 03:42:45 edd Exp $

OptionSurface <- function(EOres, label, fov=60) {
    axis.col <- "black"
    text.col <- axis.col
    ylab <- label
    xlab <- "Underlying"
    zlab <- "Volatility"
    y <- EOres

    ## clear scene:
    clear3d()
    clear3d(type="bbox")
    clear3d(type="lights")

    ## setup env:
    ##  bg3d(color="#887777")
    bg3d(color="#DDDDDD")
    light3d()

    rgl.viewpoint(fov=fov)
    ##rgl.bg(col="white", fogtype="exp2")
    ##rgl.bg(col="black", fogtype="exp2")
    ##rgl.bg(col="black", fogtype="exp")
    ##rgl.bg(col="white", fogtype="exp")

    x <- (1:nrow(y))
    z <- (1:ncol(y))
    x <- (x-min(x))/(max(x)-min(x))
    y <- (y-min(y))/(max(y)-min(y))
    z <- (z-min(z))/(max(z)-min(z))
    rgl.surface(x, z, y, alpha=0.6, lit=TRUE, color="blue")
    rgl.lines(c(0,1), c(0,0), c(0,0), col=axis.col)
    rgl.lines(c(0,0), c(0,1), c(0,0), col=axis.col)
    rgl.lines(c(0,0),c(0,0), c(0,1), col=axis.col)
    rgl.texts(1,0,0, xlab, adj=1, col=text.col)
    rgl.texts(0,1,0, ylab, adj=1, col=text.col)
    rgl.texts(0,0,1, zlab, adj=1, col=text.col)

    ## add grid (credit's to John Fox scatter3d)
    xgridind <- round(seq(1, nrow(y), length=25))
    zgridind <- round(seq(1, ncol(y), length=25))
    rgl.surface(x[xgridind], z[zgridind], y[xgridind,zgridind],
                color="darkgray", alpha=0.5, lit=TRUE,
                front="lines", back="lines")

    ## animate (credit to rgl.viewpoint() example)
    start <- proc.time()[3]
    while ((i <- 36*(proc.time()[3]-start)) < 360) {
        rgl.viewpoint(i,i/8);
    }
}

RQuantLib.demo.OptionSurfaces <- function() {

    und.seq <- seq(10, 200, by = 2.5)
    vol.seq <- seq(0.05, 2, by = 0.025)

    cat("Calculating surface ...")
    EOarr <- EuropeanOptionArrays("call", underlying = und.seq, strike = 100,
                                  dividendYield = 0.01, riskFreeRate = 0.03,
                                  maturity = 1, volatility = vol.seq)
    cat(" done.\n")

    rgl.open()
    OptionSurface(EOarr$value, "Value")
    OptionSurface(EOarr$delta, "Delta")
    OptionSurface(EOarr$gamma, "Gamma")
    OptionSurface(EOarr$vega, "Vega")
    OptionSurface(EOarr$theta, "Theta")

}

require(rgl,quiet=TRUE)
require(RQuantLib,quiet=TRUE)

RQuantLib.demo.OptionSurfaces()
