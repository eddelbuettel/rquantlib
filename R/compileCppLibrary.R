# # TODO: Add comment
# # 
# # Author: ut1tz2
# ###############################################################################
# 
# 
# sourceCpp("C:\\Program64\\FXRSHQT\\Dev\\sources\\workspace\\rquantlib\\rql.cpp")     # takes a few seconds
# timesTwo(21)
# 
# 
# 
# ####http://www.r-bloggers.com/build-rquantlib-on-32-bit-windows/ to create lib (.a and .la)
# ### Make a copy of Quantlib/ql and boost headers into src folder
# library(Rcpp)
# sourceCpp("C:\\Program64\\FXRSHQT\\Dev\\sources\\workspace\\rquantlib\\src\\dates.cpp")
# 
# sourceCpp("C:\\Program64\\FXRSHQT\\Dev\\sources\\workspace\\rquantlib\\src\\options.cpp",verbose=TRUE)
# 
# ### Load function from dll
# 
# #optionsDllInfo=dyn.load('C:\\Program64\\FXRSHQT\\Dev\\sources\\workspace\\CommonGit\\VolToolkit\\lib\\x64\\options.dll')
# source("C:\\Program64\\FXRSHQT\\Dev\\sources\\workspace\\CommonGit\\VolToolkit\\lib\\x64\\options.cpp.R")
# 
# doubleBarrier(barrierUp = 1.1,  barrierDown =   1.5,S =   1.13321,K1 = 1.13321,
#               0.0003541, 0.0033871,3/12,  0.10087,   0.08925, 0.08463,   0.11638)
# 
# S=1.39078
# barrierUp= 1.690215
# barrierDown= 1.091345
# volatility= 0.10765
# 
# callDKO=doubleKOVannaVolga("call",barrierUp =barrierUp,  barrierDown =   barrierDown,underlying =   S,strike = 1.091345,
#                            dividendYield=6e-04, riskFreeRate=0.01294,3/12,   0.111075,   0.10765, 0.109225,   0.10765)$calc
# putDKO=doubleKOVannaVolga("put",barrierUp = 1.5,  barrierDown =   1.1,underlying =   1.13321,strike = 1.5,0.0003541, 0.0033871,3/12,  
#                           0.10087,   0.08925, 0.08463,   0.08925)$calc
# library(fCertificates)
# 
# DoubleBarrierBinaryCall(S=1.13321, K=1.13321, L=1.1, U=1.5, T=3/12,
#                         r=0.0033871, r_d=0.0003541, sigma=0.11638)
# strike=100
# spot=95
# q=0.20
# r=0.10
# t=0.5  
# vol=0.2
# cashPayoff=15
# #       type, strike,   spot,    q,    r,   t,  vol,   value, tol
# #{ Option::Call, 100.00,  95.00, 0.20, 0.10, 0.5, 0.20,  8.9109, 1e-2 }
# 
# oneTouchMC("put",underlying = spot,strike = strike,cashPayoff,q, r,t,  vol,c(90,95,100),c(30,90),matrix(c(0.12,0.1,0.13,0.13,0.1,0.12),3,2))
# 
# BinaryBarrierOption(TypeFlag = "14", S =  spot, X =  0, H =  strike, K=cashPayoff,
#                     Time = t,
#                     r = (r-q), b = r-q, sigma = vol)
# #Using RQuantLib : prices are similar
# BinaryOption(binType="cash", type="call", excType="american",underlying=spot, strike=strike, dividendYield=(q),
#              riskFreeRate=(r), maturity=t, volatility=vol, cashPayoff=cashPayoff)
# 
# 
# 
