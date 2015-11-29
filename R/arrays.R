##  RQuantLib -- R interface to the QuantLib libraries
##
##  Copyright (C) 2002 - 2014 Dirk Eddelbuettel <edd@debian.org>
##
##  This file is part of RQuantLib.
##
##  RQuantLib is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 2 of the License, or
##  (at your option) any later version.
##
##  RQuantLib is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with RQuantLib.  If not, see <http://www.gnu.org/licenses/>.

oldEuropeanOptionArrays <- function(type, underlying, strike, dividendYield,
                                    riskFreeRate, maturity, volatility) {
    n.underlying <- length(underlying)
    n.strike <- length(strike)
    n.dividendYield <- length(dividendYield)
    n.riskFreeRate <- length(riskFreeRate)
    n.maturity <- length(maturity)
    n.volatility <- length(volatility)

    res <- numeric(n.underlying * n.strike * n.dividendYield * n.riskFreeRate *
                   n.maturity * n.volatility)
    dim(res) <- c(n.underlying, n.strike, n.dividendYield, n.riskFreeRate,
                  n.maturity, n.volatility)
    dimnames(res) <- list(paste("s",underlying,sep="="),
                          paste("k",strike,sep="="),
                          paste("y",dividendYield,sep="="),
                          paste("r",riskFreeRate,sep="="),
                          paste("t",maturity,sep="="),
                          paste("v",volatility,sep="="))
    value <- delta <- gamma <- vega <- theta <- rho <- divRho <- res
    for (s in 1:n.underlying) {
        for (k in 1:n.strike) {
            for (y in 1:n.dividendYield) {
                for (r in 1:n.riskFreeRate) {
                    for (t in 1:n.maturity) {
                        for (v in 1:n.volatility) {
                            val <- europeanOptionEngine(type, underlying, strike,
                                                        dividendYield, riskFreeRate,
                                                        maturity, volatility)
                            value[s,k,y,r,t,v] <- val$value
                            delta[s,k,y,r,t,v] <- val$delta
                            gamma[s,k,y,r,t,v] <- val$gamma
                            vega[s,k,y,r,t,v] <- val$vega
                            theta[s,k,y,r,t,v] <- val$theta
                            rho[s,k,y,r,t,v] <- val$rho
                            divRho[s,k,y,r,t,v] <- val$divRho
                        }
                    }
                }
            }
        }
    }
    value <- drop(value)
    delta <- drop(delta)
    gamma <- drop(gamma)
    vega <- drop(vega)
    theta <- drop(theta)
    rho <- drop(rho)
    divRho <- drop(divRho)
    invisible(list(value=value, delta=delta, gamma=gamma, vega=vega,
                   theta=theta, rho=rho, divRho=divRho,
                   parameters=list(type=type, underlying=underlying,
                   strike=strike, dividendYield=dividendYield,
                   riskFreeRate=riskFreeRate, maturity=maturity,
                   volatility=volatility)))
}

EuropeanOptionArrays <- function(type, underlying, strike, dividendYield,
                                 riskFreeRate, maturity, volatility) {
    ## check that we have two vectors
    lv <- c(length(underlying) > 1,
            length(strike) > 1,
            length(dividendYield) > 1,
            length(riskFreeRate) > 1, +
            length(maturity) > 1, +
            length(volatility) > 1)
    if (sum(lv) != 2) {
        warning("Need exactly two arguments as vectors")
        return(NULL)
    }
    type <- match.arg(type, c("call", "put"))

    ## expand parameters
    pars <- expand.grid(underlying, strike, dividendYield,
                        riskFreeRate, maturity, volatility)
    nonconst <- which( apply(pars, 2, sd) != 0)
    colnames <- c("spot", "strike", "div", "rfrate", "mat", "vol")

    val <- europeanOptionArraysEngine(type, as.matrix(pars))

    ## turn list of vectors in to list of matrices
    par1 <- unique(pars[, nonconst[1]])
    par2 <- unique(pars[, nonconst[2]])
    len1 <- length(par1)
    len2 <- length(par2)
    ml <- lapply(val, function(x) matrix(x, len1, len2, dimnames=list(par1,par2)))
    return(c(ml, parameters=list(type=type, underlying=underlying,
                 strike=strike, dividendYield=dividendYield,
                 riskFreeRate=riskFreeRate, maturity=maturity,
                 volatility=volatility)))

}

plotOptionSurface <- function(EOres, ylabel="", xlabel="", zlabel="", fov=60) {
    if (requireNamespace("rgl", quietly=TRUE)) {
        utils::globalVariables(c("clear3d", "bg3d", "ligh3d", "rgl.viewpoint", "rgl.surface", "tgl.texts"))
        axis.col <- "black"
        text.col <- axis.col
        ylab <- ylabel
        xlab <- xlabel
        zlab <- zlabel
        y <- EOres

        ## clear scene:
        rgl::clear3d()
        rgl::clear3d(type="bbox")
        rgl::clear3d(type="lights")

        ## setup env:
        rgl::bg3d(color="#DDDDDD")
        rgl::light3d()

        rgl::rgl.viewpoint(fov=fov)

        x <- 1:nrow(y)
        z <- 1:ncol(y)
        x <- (x-min(x))/(max(x)-min(x))
        y <- (y-min(y))/(max(y)-min(y))
        z <- (z-min(z))/(max(z)-min(z))
        rgl::rgl.surface(x, z, y, alpha=0.6, lit=TRUE, color="blue")
        rgl::rgl.lines(c(0,1), c(0,0), c(0,0), col=axis.col)
        rgl::rgl.lines(c(0,0), c(0,1), c(0,0), col=axis.col)
        rgl::rgl.lines(c(0,0),c(0,0), c(0,1), col=axis.col)
        rgl::rgl.texts(1,0,0, xlab, adj=1, col=text.col)
        rgl::rgl.texts(0,1,0, ylab, adj=1, col=text.col)
        rgl::rgl.texts(0,0,1, zlab, adj=1, col=text.col)

        ## add grid (credit's to John Fox scatter3d)
        xgridind <- round(seq(1, nrow(y), length=25))
        zgridind <- round(seq(1, ncol(y), length=25))
        rgl::rgl.surface(x[xgridind], z[zgridind], y[xgridind,zgridind],
                         color="darkgray", alpha=0.5, lit=TRUE,
                         front="lines", back="lines")

        ## animate (credit to rgl.viewpoint() example)
        start <- proc.time()[3]
        while ((i <- 36*(proc.time()[3]-start)) < 360) {
            rgl::rgl.viewpoint(i,i/8);
        }
    } else {
        message("Please install the 'rgl' package before using this function.")
    }
}
