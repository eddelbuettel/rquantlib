// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
//  RQuantLib function AffineSwaption
//
//  Copyright (C) 2005 - 2007 Dominick Samperi
//  Copyright (C) 2007 - 2014 Dirk Eddelbuettel
//  Copyright (C) 2016        Terry Leitch
//
//  This file is part of RQuantLib.
//
//  RQuantLib is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  RQuantLib is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with RQuantLib.  If not, see <http://www.gnu.org/licenses/>.

#include "rquantlib.h"


// [[Rcpp::export]]
Rcpp::List swapWithRebuiltCurveEngine(Rcpp::List rparam,
                                        Rcpp::List legparams,
                                        std::vector<QuantLib::Date> dateVec, 
                                        std::vector<double> zeroVec)
 {
    
    // std::vector<std::string> tsnames = tslist.names();

    double notional = 10000; // prices in basis points

    QuantLib::Date todaysDate(Rcpp::as<QuantLib::Date>(rparam["tradeDate"])); 
    QuantLib::Date settlementDate(Rcpp::as<QuantLib::Date>(rparam["settleDate"])); 
    QuantLib::Date startDate(Rcpp::as<QuantLib::Date>(rparam["startDate"])); 
    QuantLib::Date maturity(Rcpp::as<QuantLib::Date>(rparam["maturity"])); 
    bool payfix = Rcpp::as<bool>(rparam["payFixed"]);
    
    //cout << "TradeDate: " << todaysDate << endl << "Settle: " << settlementDate << endl;
    
    RQLContext::instance().settleDate = settlementDate;
    QuantLib::Settings::instance().evaluationDate() = todaysDate;
    
    // initialise from the singleton instance
    QuantLib::Calendar calendar = RQLContext::instance().calendar;
    //Integer fixingDays = RQLContext::instance().fixingDays;
    
    QuantLib::Rate fixedRate = Rcpp::as<double>(rparam["fixedRate"]);
    std::string method = Rcpp::as<std::string>(rparam["method"]);
    
    QuantLib::Handle<QuantLib::YieldTermStructure> 
        rhTermStructure(rebuildCurveFromZeroRates(dateVec, zeroVec));
    
    // Get swaption maturities
    //Rcpp::NumericVector swaptionMat(maturities);

    // Create dummy swap to get schedules.
    QuantLib::Frequency fixedLegFrequency = getFrequency(Rcpp::as<double>(legparams["fixFreq"]));
    QuantLib::BusinessDayConvention fixedLegConvention = QuantLib::Unadjusted;
    QuantLib::BusinessDayConvention floatingLegConvention = QuantLib::ModifiedFollowing;
    double fixDayCount = Rcpp::as<double>(legparams["dayCounter"]);
    QuantLib::DayCounter swFixedLegDayCounter = getDayCounter(Rcpp::as<double>(legparams["dayCounter"]));
    boost::shared_ptr<QuantLib::IborIndex> swFloatingLegIndex(new QuantLib::Euribor(QuantLib::Period(Rcpp::as<int>(legparams["floatFreq"]),QuantLib::Months),rhTermStructure));


    QuantLib::Rate dummyFixedRate = 0.03;

    QuantLib::Schedule fixedSchedule(startDate,maturity,
                                     QuantLib::Period(fixedLegFrequency),calendar,
                                     fixedLegConvention,fixedLegConvention,
                                     QuantLib::DateGeneration::Forward,false);
    QuantLib::Schedule floatSchedule(startDate,maturity,QuantLib::Period(Rcpp::as<int>(legparams["floatFreq"]),QuantLib::Months),
                                     calendar,
                                     floatingLegConvention,floatingLegConvention,
                                     QuantLib::DateGeneration::Forward,false);
    
    QuantLib::VanillaSwap::Type type;
    
    if(payfix){
        
        type = QuantLib::VanillaSwap::Payer;} 
    else{
        type = QuantLib::VanillaSwap::Receiver;    
    }
    boost::shared_ptr<QuantLib::VanillaSwap> 
        swap(new QuantLib::VanillaSwap(type, notional,
                                       fixedSchedule, dummyFixedRate, swFixedLegDayCounter,
                                       floatSchedule, swFloatingLegIndex, 0.0,
                                       swFloatingLegIndex->dayCounter()));
    swap->setPricingEngine(boost::shared_ptr<QuantLib::PricingEngine>(new QuantLib::DiscountingSwapEngine(rhTermStructure)));

    // Find the ATM or break-even rate
    QuantLib::Rate fixedATMRate = swap->fairRate();

    // The swap 
    boost::shared_ptr<QuantLib::VanillaSwap> 
        mySwap(new QuantLib::VanillaSwap(type, notional,
                                         fixedSchedule, fixedRate,swFixedLegDayCounter,
                                         floatSchedule, swFloatingLegIndex, 0.0,
                                         swFloatingLegIndex->dayCounter()));
    mySwap->setPricingEngine(boost::shared_ptr<QuantLib::PricingEngine>(new QuantLib::DiscountingSwapEngine(rhTermStructure)));
    return Rcpp::List::create(Rcpp::Named("NPV") = mySwap->NPV(),
                              Rcpp::Named("ATMrate") = fixedATMRate);
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//    
    
}
