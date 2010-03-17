## RQuantLib -- R interface to the QuantLib libraries
##
## Copyright (C) 2002 - 2009 Dirk Eddelbuettel <edd@debian.org>
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

EuropeanOptionArrays <- function(type, underlying, strike, dividendYield,
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
              val <- .Call("QL_EuropeanOption",
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

