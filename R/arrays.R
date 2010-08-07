## RQuantLib -- R interface to the QuantLib libraries
##
## Copyright (C) 2002 - 2010 Dirk Eddelbuettel <edd@debian.org>
##
## $Id: arrays.R,v 1.2 2002/11/15 01:49:28 edd Exp $
##
## This file is part of the RQuantLib library for GNU R.
## It is made available under the terms of the GNU General Public
## License, version 2, or at your option, any later version,
## incorporated herein by reference.
##
## This program is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
## PURPOSE.  See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public
## License along with this program; if not, write to the Free
## Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
## MA 02111-1307, USA

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
  for (s in 1:n.underlying)
    for (k in 1:n.strike)
      for (y in 1:n.dividendYield)
        for (r in 1:n.riskFreeRate)
          for (t in 1:n.maturity)
            for (v in 1:n.volatility) {
              val <- .Call("EuropeanOption",
                           list(type=as.character(type),
                                underlying=as.double(underlying[s]),
                                strike=as.double(strike[k]),
                                dividendYield=as.double(dividendYield[y]),
                                riskFreeRate=as.double(riskFreeRate[r]),
                                maturity=as.double(maturity[t]),
                                volatility=as.double(volatility[v])),
                           PACKAGE="RQuantLib")
              value[s,k,y,r,t,v] <- val$value
              delta[s,k,y,r,t,v] <- val$delta
              gamma[s,k,y,r,t,v] <- val$gamma
              vega[s,k,y,r,t,v] <- val$vega
              theta[s,k,y,r,t,v] <- val$theta
              rho[s,k,y,r,t,v] <- val$rho
              divRho[s,k,y,r,t,v] <- val$divRho
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

    val <- .Call("EuropeanOptionArrays", type, as.matrix(pars), PACKAGE="RQuantLib")

    ## turn list of vectors in to list of matrices
    par1 <- unique(pars[, nonconst[1]])
    par2 <- unique(pars[, nonconst[2]])
    len1 <- length(par1)
    len2 <- length(par2)
    ml <- lapply(val, function(x) matrix(x, len1, len2, dimnames=list(par1,par2)))
}
