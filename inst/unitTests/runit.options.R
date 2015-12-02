
## AsianOption() currently has issue of Windows
.onWindows <- .Platform$OS.type == "windows"

## American option tests based on Haug's book
test.american.Haug <- function() {
    ## see QuantLib's test-suite/americanoption.cpp
    #/* The data below are from
    #   "Option pricing formulas", E.G. Haug, McGraw-Hill 1998
    #   pag 24
    #
    #   The following values were replicated only up to the second digit
    #   by the VB code provided by Haug, which was used as base for the
    #   C++ implementation
    #
    #*/

    AO <- AmericanOption                # shorthand
    checkEquals(AO(type="call", strike= 100.00, underl= 90.00,  div= 0.10, riskF=0.10,  mat=0.10,  vol= 0.15)$value,  0.0206, tol=1.0e-2)
    checkEquals(AO(type="call", strike= 100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.15)$value,  1.8771, tol=1.0e-4)
    checkEquals(AO(type="call", strike= 100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.15)$value, 10.0089, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl=  90.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.25)$value,  0.3159, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.25)$value,  3.1280, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.25)$value, 10.3919, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl=  90.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.35)$value,  0.9495, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.35)$value,  4.3777, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.35)$value, 11.1679, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl=  90.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.15)$value,  0.8208, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.15)$value,  4.0842, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.15)$value, 10.8087, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl=  90.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.25)$value,  2.7437, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.25)$value,  6.8015, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.25)$value, 13.0170, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl=  90.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.35)$value,  5.0063, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.35)$value,  9.5106, tol=1.0e-3)
    checkEquals(AO(type="call", strike= 100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.35)$value, 15.5689, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl=  90.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.15)$value, 10.0000, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.15)$value,  1.8770, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.15)$value,  0.0410, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl=  90.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.25)$value, 10.2533, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.25)$value,  3.1277, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.25)$value,  0.4562, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl=  90.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.35)$value, 10.8787, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.35)$value,  4.3777, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.10, vol= 0.35)$value,  1.2402, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl=  90.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.15)$value, 10.5595, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.15)$value,  4.0842, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.15)$value,  1.0822, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl=  90.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.25)$value, 12.4419, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.25)$value,  6.8014, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.25)$value,  3.3226, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl=  90.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.35)$value, 14.6945, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 100.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.35)$value,  9.5104, tol=1.0e-3)
    checkEquals(AO(type="put", strike=  100.00, underl= 110.00, div= 0.10, riskF= 0.10, mat= 0.50, vol= 0.35)$value,  5.8823, tol=1.0e-3)

}

## Asian option tests
test.asian <- function() {
    ## see QuantLib's test-suite/asianoptions.cpp

    AO <- AsianOption   	             # shorthand

    checkEquals(AO(averageType="geometric", typ="put", strike=85, underl=80, div=-0.03, riskF=0.05, mat=0.25, vol=0.20)$value, 4.6922, tol=1.0e-4)

    if (!.onWindows) {
        
        ## data from "Asian Option", Levy, 1997
        ## in "Exotic Options: The State of the Art",
        ## edited by Clewlow, Strickland
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=0.0, length=11.0/12.0, fixings=2, vol=0.13)$value, 1.3942835683, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=0.0, length=11.0/12.0, fixings=4, vol=0.13)$value, 1.5852442983, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=0.0, length=11.0/12.0, fixings=8, vol=0.13)$value, 1.66970673, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=0.0, length=11.0/12.0, fixings=12, vol=0.13)$value, 1.6980019214, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=0.0, length=11.0/12.0, fixings=26, vol=0.13)$value, 1.7255070456, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=0.0, length=11.0/12.0, fixings=52, vol=0.13)$value, 1.7401553533, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=0.0, length=11.0/12.0, fixings=100, vol=0.13)$value, 1.7478303712, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=0.0, length=11.0/12.0, fixings=250, vol=0.13)$value, 1.7490291943, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=0.0, length=11.0/12.0, fixings=500, vol=0.13)$value, 1.7515113291, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=0.0, length=11.0/12.0, fixings=1000, vol=0.13)$value, 1.7537344885, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=1.0/12.0, length=11.0/12.0, fixings=2, vol=0.13)$value, 1.8496053697, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=1.0/12.0, length=11.0/12.0, fixings=4, vol=0.13)$value, 2.0111495205, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=1.0/12.0, length=11.0/12.0, fixings=8, vol=0.13)$value, 2.0852138818, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=1.0/12.0, length=11.0/12.0, fixings=12, vol=0.13)$value, 2.1105094397, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=1.0/12.0, length=11.0/12.0, fixings=26, vol=0.13)$value, 2.1346526695, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=1.0/12.0, length=11.0/12.0, fixings=52, vol=0.13)$value, 2.147489651, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=1.0/12.0, length=11.0/12.0, fixings=100, vol=0.13)$value, 2.154728109, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=1.0/12.0, length=11.0/12.0, fixings=250, vol=0.13)$value, 2.1564276565, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=1.0/12.0, length=11.0/12.0, fixings=500, vol=0.13)$value, 2.1594238588, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=1.0/12.0, length=11.0/12.0, fixings=1000, vol=0.13)$value, 2.1595367326, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=3.0/12.0, length=11.0/12.0, fixings=2, vol=0.13)$value, 2.63315092584, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=3.0/12.0, length=11.0/12.0, fixings=4, vol=0.13)$value, 2.76723962361, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=3.0/12.0, length=11.0/12.0, fixings=8, vol=0.13)$value, 2.83124836881, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=3.0/12.0, length=11.0/12.0, fixings=12, vol=0.13)$value, 2.84290301412, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=3.0/12.0, length=11.0/12.0, fixings=26, vol=0.13)$value, 2.88179560417, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=3.0/12.0, length=11.0/12.0, fixings=52, vol=0.13)$value, 2.88447044543, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=3.0/12.0, length=11.0/12.0, fixings=100, vol=0.13)$value, 2.89985329603, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=3.0/12.0, length=11.0/12.0, fixings=250, vol=0.13)$value, 2.90047296063, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=3.0/12.0, length=11.0/12.0, fixings=500, vol=0.13)$value, 2.89813412160, tol=1.0e-2)
        checkEquals(AO("arithmetic", "put", underl=90.0, strike=87.0, div=0.06, riskF=0.025, first=3.0/12.0, length=11.0/12.0, fixings=1000, vol=0.13)$value, 2.8970336243, tol=1.0e-2)
    }
}

## Barrier Options
test.barrier <- function() {
    checkEquals(BarrierOption(barrType="downin", type="call", underl=100, strike=100, div=0.02, riskF=0.03, mat=0.5, vol=0.4, barrier=90)$value, 3.738254414)
    checkEquals(BarrierOption("downout", barrier=95, rebate=3, type="call", strike=90, underlying=100, div=0.04, riskF=0.08, mat=0.5, vol=0.25)$value, 9.024567695)
    checkEquals(BarrierOption("downin", barrier=95, rebate=3, type="call", strike=90, underlying=100, div=0.04, riskF=0.08, mat=0.5, vol=0.25)$value, 7.76267021)
}

test.barrier.Haug <- function() {
    ## see QuantLib's test-suite/barrieroption.cpp
    #    /* The data below are from
    #     * "Option pricing formulas", E.G. Haug, McGraw-Hill 1998 pag. 72
    #     */
    #
    BO <- BarrierOption                 # shorthand
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  9.0246, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  6.7924, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  4.8759, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  2.6789, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  2.3580, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  2.3453, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  7.7627, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  4.0109, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  2.0576, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value, 13.8333, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  7.8494, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  3.9795, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value, 14.1112, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  8.4482, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  4.5910, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  8.8334, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  7.0285, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  5.4137, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  2.6341, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  2.4389, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  2.4315, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  9.0093, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  5.1370, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  2.8517, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value, 14.8816, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  9.2045, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  5.3043, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="call", strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value, 15.2098, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="call", strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  9.7278, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="call", strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  5.8350, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  2.2798, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  2.2947, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  2.6252, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  3.7760, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  5.4932, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  7.5187, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  2.9586, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  6.5677, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value, 11.9752, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  2.2845, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  5.9085, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value, 11.6465, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  1.4653, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  3.3721, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.25)$value,  7.0846, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  2.4170, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  2.4258, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=95.0,  rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  2.6246, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="downout", barrier=100.0, rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  3.0000, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  4.2293, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  5.8032, tol=1.0e-4)
    checkEquals(BO(barrType="upout",   barrier=105.0, rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  7.5649, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  3.8769, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  7.7989, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=95.0,  rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value, 13.3078, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  3.3328, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  7.2636, tol=1.0e-4)
    checkEquals(BO(barrType="downin",  barrier=100.0, rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value, 12.9713, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="put",  strike=90,  underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  2.0658, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="put",  strike=100, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  4.4226, tol=1.0e-4)
    checkEquals(BO(barrType="upin",    barrier=105.0, rebate=3.0, type="put",  strike=110, underl=100.0, div=0.04, riskF=0.08, mat=0.50, vol=0.30)$value,  8.3686, tol=1.0e-4)
}

## Binary Options aka Digitals
test.binary <- function() {
    ## cash or nothing European
    rc <- BinaryOption(binType="cash", type="put", excType="european", strike=80, underl=100, div=0.06, r=0.06, mat=0.75, vol=0.35, cash=10)
    checkEquals(rc$value, 2.671045684)
    checkEquals(rc[1:7],
                list(value=2.671045, delta=-0.1060594, gamma=0.00310624, vega=8.153881, theta=-1.742309, rho=-9.9577, divRho=7.9544),
                tolerance=1.0e-5)
}

## European option tests based on Haug's book
test.european.Haug <- function() {
    ## see QuantLib's test-suite/europeanoption.cpp
    #/* The data below are from
    #   "Option pricing formulas", E.G. Haug, McGraw-Hill 1998
    #*/
    #  // pag 2-8, pg 24, p 27
    #
    Lines <- "type  strike  spot  q  r  t  v  value tol"
    EO <- EuropeanOption                # shorthand
    checkEquals(EO(type="call", strike=65.00,  underl=60.00,  div= 0.00, riskF=0.08, mat=0.25, vol=0.30)$value,  2.1334, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=95.00,  underl=100.00, div= 0.05, riskF=0.10, mat=0.50, vol=0.20)$value,  2.4648, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=19.00,  underl=19.00,  div= 0.10, riskF=0.10, mat=0.75, vol=0.28)$value,  1.7011, tol=1.0e-3)
    checkEquals(EO(type="call", strike=19.00,  underl=19.00,  div= 0.10, riskF=0.10, mat=0.75, vol=0.28)$value,  1.7011, tol=1.0e-3)
    checkEquals(EO(type="call", strike=1.60,   underl=1.56,   div= 0.08, riskF=0.06, mat=0.50, vol=0.12)$value,  0.0291, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=70.00,  underl=75.00,  div= 0.05, riskF=0.10, mat=0.50, vol=0.35)$value,  4.0870, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.10, vol=0.15)$value,  0.0205, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.15)$value,  1.8734, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.15)$value,  9.9413, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.10, vol=0.25)$value,  0.3150, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.25)$value,  3.1217, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.25)$value, 10.3556, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.10, vol=0.35)$value,  0.9474, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.35)$value,  4.3693, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.35)$value, 11.1381, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.50, vol=0.15)$value,  0.8069, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.15)$value,  4.0232, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.15)$value, 10.5769, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.50, vol=0.25)$value,  2.7026, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.25)$value,  6.6997, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.25)$value, 12.7857, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.50, vol=0.35)$value,  4.9329, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.35)$value,  9.3679, tol=1.0e-3)
    checkEquals(EO(type="call", strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.35)$value, 15.3086, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.10, vol=0.15)$value,  9.9210, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.15)$value,  1.8734, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.15)$value,  0.0408, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.10, vol=0.25)$value, 10.2155, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.25)$value,  3.1217, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.25)$value,  0.4551, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.10, vol=0.35)$value, 10.8479, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.35)$value,  4.3693, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.10, vol=0.35)$value,  1.2376, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.50, vol=0.15)$value, 10.3192, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.15)$value,  4.0232, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.15)$value,  1.0646, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.50, vol=0.25)$value, 12.2149, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.25)$value,  6.6997, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.25)$value,  3.2734, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=90.00,  div= 0.10, riskF=0.10, mat=0.50, vol=0.35)$value, 14.4452, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=100.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.35)$value,  9.3679, tol=1.0e-3)
    checkEquals(EO(type="put",  strike=100.00, underl=110.00, div= 0.10, riskF=0.10, mat=0.50, vol=0.35)$value,  5.7963, tol=1.0e-3)
    checkEquals(EO(type="call", strike=40.00,  underl=42.00,  div= 0.08, riskF=0.04, mat=0.75, vol=0.35)$value,  5.0975, tol=1.0e-3)
}
