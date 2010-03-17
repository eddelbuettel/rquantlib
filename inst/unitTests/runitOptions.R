
## American option tests based on Haug's book
test.american.Haug <- function() {
    ## see QL's test-suite/americanoption.cpp
    #/* The data below are from
    #   "Option pricing formulas", E.G. Haug, McGraw-Hill 1998
    #   pag 24
    #
    #   The following values were replicated only up to the second digit
    #   by the VB code provided by Haug, which was used as base for the
    #   C++ implementation
    #
    #*/
    Lines <- "type  strike  spot  q  r  t  v  value
call 100.00  90.00 0.10 0.10 0.10 0.15  0.0206
call 100.00 100.00 0.10 0.10 0.10 0.15  1.8771
call 100.00 110.00 0.10 0.10 0.10 0.15 10.0089
call 100.00  90.00 0.10 0.10 0.10 0.25  0.3159
call 100.00 100.00 0.10 0.10 0.10 0.25  3.1280
call 100.00 110.00 0.10 0.10 0.10 0.25 10.3919
call 100.00  90.00 0.10 0.10 0.10 0.35  0.9495
call 100.00 100.00 0.10 0.10 0.10 0.35  4.3777
call 100.00 110.00 0.10 0.10 0.10 0.35 11.1679
call 100.00  90.00 0.10 0.10 0.50 0.15  0.8208
call 100.00 100.00 0.10 0.10 0.50 0.15  4.0842
call 100.00 110.00 0.10 0.10 0.50 0.15 10.8087
call 100.00  90.00 0.10 0.10 0.50 0.25  2.7437
call 100.00 100.00 0.10 0.10 0.50 0.25  6.8015
call 100.00 110.00 0.10 0.10 0.50 0.25 13.0170
call 100.00  90.00 0.10 0.10 0.50 0.35  5.0063
call 100.00 100.00 0.10 0.10 0.50 0.35  9.5106
call 100.00 110.00 0.10 0.10 0.50 0.35 15.5689
put  100.00  90.00 0.10 0.10 0.10 0.15 10.0000
put  100.00 100.00 0.10 0.10 0.10 0.15  1.8770
put  100.00 110.00 0.10 0.10 0.10 0.15  0.0410
put  100.00  90.00 0.10 0.10 0.10 0.25 10.2533
put  100.00 100.00 0.10 0.10 0.10 0.25  3.1277
put  100.00 110.00 0.10 0.10 0.10 0.25  0.4562
put  100.00  90.00 0.10 0.10 0.10 0.35 10.8787
put  100.00 100.00 0.10 0.10 0.10 0.35  4.3777
put  100.00 110.00 0.10 0.10 0.10 0.35  1.2402
put  100.00  90.00 0.10 0.10 0.50 0.15 10.5595
put  100.00 100.00 0.10 0.10 0.50 0.15  4.0842
put  100.00 110.00 0.10 0.10 0.50 0.15  1.0822
put  100.00  90.00 0.10 0.10 0.50 0.25 12.4419
put  100.00 100.00 0.10 0.10 0.50 0.25  6.8014
put  100.00 110.00 0.10 0.10 0.50 0.25  3.3226
put  100.00  90.00 0.10 0.10 0.50 0.35 14.6945
put  100.00 100.00 0.10 0.10 0.50 0.35  9.5104
put  100.00 110.00 0.10 0.10 0.50 0.35  5.8823"

    ## now loop over all tests -- note that we need a fairly loose tolerance value
    Params <- read.delim(textConnection(Lines), sep="")
    for (i in NROW(Params)) {
        P <- Params[i,]
        rc <- AmericanOption(type=as.character(P$type), underlying=P$spot, strike=P$strike,
                             dividendYield=P$q, riskFreeRate=P$r, maturity=P$t, volatility=P$v)
        checkEquals(rc$value, P$value, tolerance=1.0e-4)
    }
    return()
}

## Barrier Options
test.barrier <- function() {
    checkEquals(BarrierOption(barrType="downin", type="call", underlying=100,
                              strike=100, dividendYield=0.02, riskFreeRate=0.03,
                              maturity=0.5, volatility=0.4, barrier=90)$value,
                3.738254414)

    checkEquals(BarrierOption("downout", barrier=95, rebate=3, type="call",
                              strike=90, underlying=100, div=0.04, riskF=0.08,
                              mat=0.5, vol=0.25)$value,
                9.024567695)

    checkEquals(BarrierOption("downin", barrier=95, rebate=3, type="call",
                              strike=90, underlying=100, div=0.04, riskF=0.08,
                              mat=0.5, vol=0.25)$value,
                7.76267021)
    return()
}

test.barrier.Haug <- function() {
    ## see QL's test-suite/barrieroption.cpp
    #    /* The data below are from
    #     * "Option pricing formulas", E.G. Haug, McGraw-Hill 1998 pag. 72
    #     */
    #
    #    //     barrierType, barrier, rebate,         type, strike,     s,    q,    r,    t,    v,  result, tol
    Lines <- "barrierType  barrier  rebate  type  strike  spot  q  r  t  v  result  tol
downout     95.0     3.0  call      90  100.0  0.04  0.08  0.50  0.25   9.0246  1.0e-4
downout     95.0     3.0  call     100  100.0  0.04  0.08  0.50  0.25   6.7924  1.0e-4
downout     95.0     3.0  call     110  100.0  0.04  0.08  0.50  0.25   4.8759  1.0e-4
downout    100.0     3.0  call      90  100.0  0.04  0.08  0.50  0.25   3.0000  1.0e-4
downout    100.0     3.0  call     100  100.0  0.04  0.08  0.50  0.25   3.0000  1.0e-4
downout    100.0     3.0  call     110  100.0  0.04  0.08  0.50  0.25   3.0000  1.0e-4
upout      105.0     3.0  call      90  100.0  0.04  0.08  0.50  0.25   2.6789  1.0e-4
upout      105.0     3.0  call     100  100.0  0.04  0.08  0.50  0.25   2.3580  1.0e-4
upout      105.0     3.0  call     110  100.0  0.04  0.08  0.50  0.25   2.3453  1.0e-4
downin      95.0     3.0  call     90  100.0  0.04  0.08  0.50  0.25   7.7627  1.0e-4
downin      95.0     3.0  call    100  100.0  0.04  0.08  0.50  0.25   4.0109  1.0e-4
downin      95.0     3.0  call    110  100.0  0.04  0.08  0.50  0.25   2.0576  1.0e-4
downin     100.0     3.0  call     90  100.0  0.04  0.08  0.50  0.25  13.8333  1.0e-4
downin     100.0     3.0  call    100  100.0  0.04  0.08  0.50  0.25   7.8494  1.0e-4
downin     100.0     3.0  call    110  100.0  0.04  0.08  0.50  0.25   3.9795  1.0e-4
upin       105.0     3.0  call     90  100.0  0.04  0.08  0.50  0.25  14.1112  1.0e-4
upin       105.0     3.0  call    100  100.0  0.04  0.08  0.50  0.25   8.4482  1.0e-4
upin       105.0     3.0  call    110  100.0  0.04  0.08  0.50  0.25   4.5910  1.0e-4
downout     95.0     3.0  call     90  100.0  0.04  0.08  0.50  0.30   8.8334  1.0e-4
downout     95.0     3.0  call    100  100.0  0.04  0.08  0.50  0.30   7.0285  1.0e-4
downout     95.0     3.0  call    110  100.0  0.04  0.08  0.50  0.30   5.4137  1.0e-4
downout    100.0     3.0  call     90  100.0  0.04  0.08  0.50  0.30   3.0000  1.0e-4
downout    100.0     3.0  call    100  100.0  0.04  0.08  0.50  0.30   3.0000  1.0e-4
downout    100.0     3.0  call    110  100.0  0.04  0.08  0.50  0.30   3.0000  1.0e-4
upout      105.0     3.0  call     90  100.0  0.04  0.08  0.50  0.30   2.6341  1.0e-4
upout      105.0     3.0  call    100  100.0  0.04  0.08  0.50  0.30   2.4389  1.0e-4
upout      105.0     3.0  call    110  100.0  0.04  0.08  0.50  0.30   2.4315  1.0e-4
downin      95.0     3.0  call     90  100.0  0.04  0.08  0.50  0.30   9.0093  1.0e-4
downin      95.0     3.0  call    100  100.0  0.04  0.08  0.50  0.30   5.1370  1.0e-4
downin      95.0     3.0  call    110  100.0  0.04  0.08  0.50  0.30   2.8517  1.0e-4
downin     100.0     3.0  call     90  100.0  0.04  0.08  0.50  0.30  14.8816  1.0e-4
downin     100.0     3.0  call    100  100.0  0.04  0.08  0.50  0.30   9.2045  1.0e-4
downin     100.0     3.0  call    110  100.0  0.04  0.08  0.50  0.30   5.3043  1.0e-4
upin       105.0     3.0  call     90  100.0  0.04  0.08  0.50  0.30  15.2098  1.0e-4
upin       105.0     3.0  call    100  100.0  0.04  0.08  0.50  0.30   9.7278  1.0e-4
upin       105.0     3.0  call    110  100.0  0.04  0.08  0.50  0.30   5.8350  1.0e-4
downout     95.0     3.0   put     90  100.0  0.04  0.08  0.50  0.25   2.2798  1.0e-4
downout     95.0     3.0   put    100  100.0  0.04  0.08  0.50  0.25   2.2947  1.0e-4
downout     95.0     3.0   put    110  100.0  0.04  0.08  0.50  0.25   2.6252  1.0e-4
downout    100.0     3.0   put     90  100.0  0.04  0.08  0.50  0.25   3.0000  1.0e-4
downout    100.0     3.0   put    100  100.0  0.04  0.08  0.50  0.25   3.0000  1.0e-4
downout    100.0     3.0   put    110  100.0  0.04  0.08  0.50  0.25   3.0000  1.0e-4
upout      105.0     3.0   put     90  100.0  0.04  0.08  0.50  0.25   3.7760  1.0e-4
upout      105.0     3.0   put    100  100.0  0.04  0.08  0.50  0.25   5.4932  1.0e-4
upout      105.0     3.0   put    110  100.0  0.04  0.08  0.50  0.25   7.5187  1.0e-4
downin      95.0     3.0   put     90  100.0  0.04  0.08  0.50  0.25   2.9586  1.0e-4
downin      95.0     3.0   put    100  100.0  0.04  0.08  0.50  0.25   6.5677  1.0e-4
downin      95.0     3.0   put    110  100.0  0.04  0.08  0.50  0.25  11.9752  1.0e-4
downin     100.0     3.0   put     90  100.0  0.04  0.08  0.50  0.25   2.2845  1.0e-4
downin     100.0     3.0   put    100  100.0  0.04  0.08  0.50  0.25   5.9085  1.0e-4
downin     100.0     3.0   put    110  100.0  0.04  0.08  0.50  0.25  11.6465  1.0e-4
upin       105.0     3.0   put     90  100.0  0.04  0.08  0.50  0.25   1.4653  1.0e-4
upin       105.0     3.0   put    100  100.0  0.04  0.08  0.50  0.25   3.3721  1.0e-4
upin       105.0     3.0   put    110  100.0  0.04  0.08  0.50  0.25   7.0846  1.0e-4
downout     95.0     3.0   put     90  100.0  0.04  0.08  0.50  0.30   2.4170  1.0e-4
downout     95.0     3.0   put    100  100.0  0.04  0.08  0.50  0.30   2.4258  1.0e-4
downout     95.0     3.0   put    110  100.0  0.04  0.08  0.50  0.30   2.6246  1.0e-4
downout    100.0     3.0   put     90  100.0  0.04  0.08  0.50  0.30   3.0000  1.0e-4
downout    100.0     3.0   put    100  100.0  0.04  0.08  0.50  0.30   3.0000  1.0e-4
downout    100.0     3.0   put    110  100.0  0.04  0.08  0.50  0.30   3.0000  1.0e-4
upout      105.0     3.0   put     90  100.0  0.04  0.08  0.50  0.30   4.2293  1.0e-4
upout      105.0     3.0   put    100  100.0  0.04  0.08  0.50  0.30   5.8032  1.0e-4
upout      105.0     3.0   put    110  100.0  0.04  0.08  0.50  0.30   7.5649  1.0e-4
downin      95.0     3.0   put     90  100.0  0.04  0.08  0.50  0.30   3.8769  1.0e-4
downin      95.0     3.0   put    100  100.0  0.04  0.08  0.50  0.30   7.7989  1.0e-4
downin      95.0     3.0   put    110  100.0  0.04  0.08  0.50  0.30  13.3078  1.0e-4
downin     100.0     3.0   put     90  100.0  0.04  0.08  0.50  0.30   3.3328  1.0e-4
downin     100.0     3.0   put    100  100.0  0.04  0.08  0.50  0.30   7.2636  1.0e-4
downin     100.0     3.0   put    110  100.0  0.04  0.08  0.50  0.30  12.9713  1.0e-4
upin       105.0     3.0   put     90  100.0  0.04  0.08  0.50  0.30   2.0658  1.0e-4
upin       105.0     3.0   put    100  100.0  0.04  0.08  0.50  0.30   4.4226  1.0e-4
upin       105.0     3.0   put    110  100.0  0.04  0.08  0.50  0.30   8.3686  1.0e-4"

    ## now loop over all tests -- note that we need a fairly loose tolerance value
    Params <- read.delim(textConnection(Lines), sep="")
    for (i in NROW(Params)) {
        P <- Params[i,]
        rc <- BarrierOption(barrType=as.character(P$barrierType), type=as.character(P$type), underlying=P$spot, strike=P$strike,
                            dividendYield=P$q, riskFreeRate=P$r, maturity=P$t, volatility=P$v, barrier=P$barrier)
        checkEquals(rc$value, P$result, tolerance=0.1)
    }
    return(Params)
}

## Binary Options aka Digitals
test.binary <- function() {
    ## cash or nothing European
    rc <- BinaryOption("put", strike=80, underl=100, div=0.06, r=0.06, mat=0.75, vol=0.35, cash=10)
    checkEquals(rc$value, 2.671045684)
    checkEquals(rc[1:7],
                list(value=2.671045, delta=-0.1060594, gamma=0.00310624, vega=8.153881, theta=-1.742309, rho=-9.9577, divRho=7.9544),
                tolerance=1.0e-5)
    return()
}

## European option tests based on Haug's book
test.european.Haug <- function() {
    ## see QL's test-suite/europeanoption.cpp
    #/* The data below are from
    #   "Option pricing formulas", E.G. Haug, McGraw-Hill 1998
    #*/
    #  // pag 2-8, pg 24, p 27
    #
    Lines <- "type  strike  spot  q  r  t  v  value tol
call  65.00  60.00 0.00 0.08 0.25 0.30  2.1334 1.0e-4
put   95.00 100.00 0.05 0.10 0.50 0.20  2.4648 1.0e-4
put   19.00  19.00 0.10 0.10 0.75 0.28  1.7011 1.0e-4
call  19.00  19.00 0.10 0.10 0.75 0.28  1.7011 1.0e-4
call   1.60   1.56 0.08 0.06 0.50 0.12  0.0291 1.0e-4
put   70.00  75.00 0.05 0.10 0.50 0.35  4.0870 1.0e-4
call 100.00  90.00 0.10 0.10 0.10 0.15  0.0205 1.0e-4
call 100.00 100.00 0.10 0.10 0.10 0.15  1.8734 1.0e-4
call 100.00 110.00 0.10 0.10 0.10 0.15  9.9413 1.0e-4
call 100.00  90.00 0.10 0.10 0.10 0.25  0.3150 1.0e-4
call 100.00 100.00 0.10 0.10 0.10 0.25  3.1217 1.0e-4
call 100.00 110.00 0.10 0.10 0.10 0.25 10.3556 1.0e-4
call 100.00  90.00 0.10 0.10 0.10 0.35  0.9474 1.0e-4
call 100.00 100.00 0.10 0.10 0.10 0.35  4.3693 1.0e-4
call 100.00 110.00 0.10 0.10 0.10 0.35 11.1381 1.0e-4
call 100.00  90.00 0.10 0.10 0.50 0.15  0.8069 1.0e-4
call 100.00 100.00 0.10 0.10 0.50 0.15  4.0232 1.0e-4
call 100.00 110.00 0.10 0.10 0.50 0.15 10.5769 1.0e-4
call 100.00  90.00 0.10 0.10 0.50 0.25  2.7026 1.0e-4
call 100.00 100.00 0.10 0.10 0.50 0.25  6.6997 1.0e-4
call 100.00 110.00 0.10 0.10 0.50 0.25 12.7857 1.0e-4
call 100.00  90.00 0.10 0.10 0.50 0.35  4.9329 1.0e-4
call 100.00 100.00 0.10 0.10 0.50 0.35  9.3679 1.0e-4
call 100.00 110.00 0.10 0.10 0.50 0.35 15.3086 1.0e-4
put  100.00  90.00 0.10 0.10 0.10 0.15  9.9210 1.0e-4
put  100.00 100.00 0.10 0.10 0.10 0.15  1.8734 1.0e-4
put  100.00 110.00 0.10 0.10 0.10 0.15  0.0408 1.0e-4
put  100.00  90.00 0.10 0.10 0.10 0.25 10.2155 1.0e-4
put  100.00 100.00 0.10 0.10 0.10 0.25  3.1217 1.0e-4
put  100.00 110.00 0.10 0.10 0.10 0.25  0.4551 1.0e-4
put  100.00  90.00 0.10 0.10 0.10 0.35 10.8479 1.0e-4
put  100.00 100.00 0.10 0.10 0.10 0.35  4.3693 1.0e-4
put  100.00 110.00 0.10 0.10 0.10 0.35  1.2376 1.0e-4
put  100.00  90.00 0.10 0.10 0.50 0.15 10.3192 1.0e-4
put  100.00 100.00 0.10 0.10 0.50 0.15  4.0232 1.0e-4
put  100.00 110.00 0.10 0.10 0.50 0.15  1.0646 1.0e-4
put  100.00  90.00 0.10 0.10 0.50 0.25 12.2149 1.0e-4
put  100.00 100.00 0.10 0.10 0.50 0.25  6.6997 1.0e-4
put  100.00 110.00 0.10 0.10 0.50 0.25  3.2734 1.0e-4
put  100.00  90.00 0.10 0.10 0.50 0.35 14.4452 1.0e-4
put  100.00 100.00 0.10 0.10 0.50 0.35  9.3679 1.0e-4
put  100.00 110.00 0.10 0.10 0.50 0.35  5.7963 1.0e-4
call  40.00  42.00 0.08 0.04 0.75 0.35  5.0975 1.0e-4"

    ## now loop over all tests -- note that we need a fairly loose tolerance value
    Params <- read.delim(textConnection(Lines), sep="")
    for (i in NROW(Params)) {
        P <- Params[i,]
        rc <- EuropeanOption(type=as.character(P$type), underlying=P$spot, strike=P$strike,
                             dividendYield=P$q, riskFreeRate=P$r, maturity=P$t, volatility=P$v)
        checkEquals(rc$value, P$value, tolerance=P$tol)
    }
    return()
}
