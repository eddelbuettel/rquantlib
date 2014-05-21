// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2002 - 2014  Dirk Eddelbuettel 
//  Copyright (C) 2009 - 2012  Khanh Nguyen and Dirk Eddelbuettel
//  Copyright (C) 2013 - 2014  Dirk Eddelbuettel
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


//  [[Rcpp::interfaces(r, cpp)]]

#include <rquantlib.h>

// [[Rcpp::export]]
double zeroPriceByYieldEngine(double yield,
                              double faceAmount,
                              double dayCounter,
                              double frequency,
                              double businessDayConvention,
                              double compound,
                              QuantLib::Date maturityDate,
                              QuantLib::Date issueDate) {
    //setup bond
    QuantLib::Integer fixingDays = 2;
    QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    QuantLib::Date todaysDate = calendar.advance(issueDate, -fixingDays, QuantLib::Days);
    QuantLib::Settings::instance().evaluationDate() = todaysDate;
    QuantLib::Natural settlementDays = 1;
    
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
    double redemption = 100;
    QuantLib::ZeroCouponBond zbond(settlementDays, calendar, faceAmount, maturityDate, 
                                   bdc, redemption, issueDate);
       
    //return cleanPrice
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    QuantLib::Compounding cp = getCompounding(compound);
    QuantLib::Frequency freq = getFrequency(frequency);
    return zbond.cleanPrice(yield, dc, cp, freq);
}


// [[Rcpp::export]]
double zeroYieldByPriceEngine(double price,
                              double faceAmount,
                              double dayCounter,
                              double frequency,
                              double businessDayConvention,
                              double compound,
                              QuantLib::Date maturityDate,
                              QuantLib::Date issueDate) {

    //setup bond
    QuantLib::Integer fixingDays = 2;
    QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    QuantLib::Date todaysDate = calendar.advance(issueDate, -fixingDays, QuantLib::Days);
    QuantLib::Settings::instance().evaluationDate() = todaysDate;
    QuantLib::Natural settlementDays = 1;
       
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
    double redemption = 100;
    QuantLib::ZeroCouponBond zbond(settlementDays, calendar, faceAmount, maturityDate,
                                   bdc, redemption, issueDate);
       
    //return yield
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    QuantLib::Compounding cp = getCompounding(compound);
    QuantLib::Frequency freq = getFrequency(frequency);
    return zbond.yield(price, dc, cp, freq);
}


   
// [[Rcpp::export]]
double fixedRateBondYieldByPriceEngine(double settlementDays,
                                       double price,
                                       std::string cal,
                                       double faceAmount,
                                       double businessDayConvention,
                                       double compound,
                                       double redemption,
                                       double dayCounter,
                                       double frequency, 
                                       QuantLib::Date maturityDate,
                                       QuantLib::Date issueDate,
                                       QuantLib::Date effectiveDate,
                                       std::vector<double> rates) {

    //set up BusinessDayConvetion
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    QuantLib::Frequency freq = getFrequency(frequency);
    QuantLib::Compounding cp = getCompounding(compound);
    
    //set up calendar
    QuantLib::Calendar 
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    if (cal == "us") {
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    } else if (cal == "uk") {
        calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
    }
        
    //build the bond
    QuantLib::Schedule sch(effectiveDate, maturityDate, QuantLib::Period(freq), calendar,
                           bdc, bdc, QuantLib::DateGeneration::Backward, false);
        
    QuantLib::FixedRateBond bond(settlementDays, faceAmount, sch,
                                 rates, dc, bdc, redemption, issueDate);

    return bond.yield(price, dc, cp, freq);
}

// [[Rcpp::export]]
double fixedRateBondPriceByYieldEngine(double settlementDays,
                                       double yield,
                                       std::string cal,
                                       double faceAmount,
                                       double businessDayConvention,
                                       double compound,
                                       double redemption,
                                       double dayCounter,
                                       double frequency, 
                                       QuantLib::Date maturityDate,
                                       QuantLib::Date issueDate,
                                       QuantLib::Date effectiveDate,
                                       std::vector<double> rates) {
 
    //set up BusinessDayConvetion
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    QuantLib::Frequency freq = getFrequency(frequency);
    QuantLib::Compounding cp = getCompounding(compound);
 
    //set up calendar
    QuantLib::Calendar 
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    if (cal == "us") {
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    } else if (cal == "uk") {
        calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
    }
        
    //build the bond
    QuantLib::Schedule sch(effectiveDate, maturityDate, QuantLib::Period(freq), calendar,
                           bdc, bdc, QuantLib::DateGeneration::Backward, false);
        
    QuantLib::FixedRateBond bond(settlementDays, faceAmount, sch, rates, 
                                 dc, bdc, redemption, issueDate);
    
    return bond.cleanPrice(yield, dc, cp, freq);
}

// not exported to R but called below
Rcpp::List FloatingBond(Rcpp::List rparam, 
                        std::vector<double> gearings, 
                        std::vector<double> spreads,
                        std::vector<double> caps, 
                        std::vector<double> floors, 
                        QuantLib::Handle<QuantLib::YieldTermStructure> &index,
                        Rcpp::List iborparams,
                        QuantLib::Handle<QuantLib::YieldTermStructure> &discountCurve,
                        Rcpp::List datemisc) 
{
  
    double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
    QuantLib::Date maturityDate(Rcpp::as<QuantLib::Date>(rparam["maturityDate"]));
    QuantLib::Date issueDate(Rcpp::as<QuantLib::Date>(rparam["issueDate"]));
    QuantLib::Date effectiveDate(Rcpp::as<QuantLib::Date>(rparam["effectiveDate"]));
    double redemption = Rcpp::as<double>(rparam["redemption"]);

    double settlementDays = Rcpp::as<double>(datemisc["settlementDays"]);
    std::string cal = Rcpp::as<std::string>(datemisc["calendar"]);
    double dayCounter = Rcpp::as<double>(datemisc["dayCounter"]);
    double frequency = Rcpp::as<double>(datemisc["period"]);
    double businessDayConvention = Rcpp::as<double>(datemisc["businessDayConvention"]);
    double terminationDateConvention = Rcpp::as<double>(datemisc["terminationDateConvention"]);
    double dateGeneration = Rcpp::as<double>(datemisc["dateGeneration"]);
    double endOfMonthRule = Rcpp::as<double>(datemisc["endOfMonth"]);
    double fixingDays = Rcpp::as<double>(datemisc["fixingDays"]);

    //build schedule
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
    QuantLib::BusinessDayConvention tbdc = getBusinessDayConvention(terminationDateConvention);
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    QuantLib::Frequency freq = getFrequency(frequency);
    QuantLib::DateGeneration::Rule rule = getDateGenerationRule(dateGeneration);
    bool endOfMonth = (endOfMonthRule==1) ? true : false;

    //set up calendar
    QuantLib::Calendar 
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    if (cal == "us") {
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    } else if (cal == "uk") {
        calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
    }
    QuantLib::Schedule sch(effectiveDate, maturityDate, QuantLib::Period(freq), calendar,
                           bdc, tbdc, rule, endOfMonth);

    // //extract gearings, spreads, caps, and floors
    // std::vector<double> gearings = getDoubleVector(gearingsVec);
    // std::vector<double> spreads = getDoubleVector(spreadsVec);
    // std::vector<double> caps = getDoubleVector(capsVec);
    // std::vector<double> floors = getDoubleVector(floorsVec);

    std::string type = Rcpp::as<std::string>(iborparams["type"]);
    double length = Rcpp::as<double>(iborparams["length"]);
    std::string inTermOf = Rcpp::as<std::string>(iborparams["inTermOf"]);

    boost::shared_ptr<QuantLib::IborIndex> 
        iborindex(new QuantLib::USDLibor(6 * QuantLib::Months, index));
    if (type=="USDLibor") {
        if (inTermOf=="Months") {               
            boost::shared_ptr<QuantLib::IborIndex> 
                temp(new QuantLib::USDLibor(length * QuantLib::Months, index));
            iborindex = temp;
        } else if (inTermOf=="Years") {
            boost::shared_ptr<QuantLib::IborIndex> 
                temp(new QuantLib::USDLibor(length * QuantLib::Years, index));
            iborindex = temp;
        }
    }
    //build the bond
    QuantLib::FloatingRateBond bond(settlementDays, faceAmount, sch, iborindex, 
                                    dc, bdc, fixingDays, gearings, spreads, caps, 
                                    floors, false, redemption, issueDate);        
        
        
    //bond price
    boost::shared_ptr<QuantLib::PricingEngine> 
        bondEngine(new QuantLib::DiscountingBondEngine(discountCurve));
    bond.setPricingEngine(bondEngine);

        
    //cashflow
    boost::shared_ptr<QuantLib::IborCouponPricer> 
        pricer(new QuantLib::BlackIborCouponPricer(QuantLib::Handle<QuantLib::OptionletVolatilityStructure>()));
    setCouponPricer(bond.cashflows(),pricer);

    return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                              Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                              Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                              Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                              Rcpp::Named("yield") = bond.yield(QuantLib::Actual360(), 
                                                                QuantLib::Compounded, 
                                                                QuantLib::Annual),
                              Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

}

// [[Rcpp::export]]
Rcpp::List FloatBond1(SEXP bond, std::vector<double> gearings, std::vector<double> caps, 
                      std::vector<double> spreads, std::vector<double> floors, SEXP indexparams, SEXP index, 
                      Rcpp::List discountCurve, SEXP dateparams) {
    
    QuantLib::Handle<QuantLib::YieldTermStructure> discount_curve(getFlatCurve(discountCurve));
    QuantLib::Handle<QuantLib::YieldTermStructure> ibor_curve(getFlatCurve(index));
    return FloatingBond(bond, gearings, caps, spreads, floors, ibor_curve, indexparams, discount_curve, dateparams);       
}


// [[Rcpp::export]]
Rcpp::List FloatBond2(SEXP bond, std::vector<double> gearings, std::vector<double> caps, 
                      std::vector<double> spreads, std::vector<double> floors, 
                      SEXP indexparams, SEXP index_params, 
                      SEXP index_tsQuotes, SEXP index_times,
                      Rcpp::List discountCurve, SEXP dateparams) {
    
    QuantLib::Handle<QuantLib::YieldTermStructure> discount_curve(getFlatCurve(discountCurve));
    QuantLib::Handle<QuantLib::YieldTermStructure> ibor_curve(buildTermStructure(index_params, index_tsQuotes));
    return FloatingBond(bond, gearings, caps, spreads, floors, ibor_curve, indexparams, discount_curve, dateparams);       
}


// [[Rcpp::export]]
Rcpp::List FloatBond3(SEXP bond, std::vector<double> gearings, std::vector<double> caps,
                      std::vector<double> spreads, std::vector<double> floors, 
                      SEXP indexparams, Rcpp::List index, 
                      SEXP disc_params, SEXP disc_tsQuotes,
                      SEXP disc_times, SEXP dateparams) {

    QuantLib::Handle<QuantLib::YieldTermStructure> ibor_curve(getFlatCurve(index));
    QuantLib::Handle<QuantLib::YieldTermStructure> discount_curve(buildTermStructure(disc_params, disc_tsQuotes));
    return FloatingBond(bond, gearings, caps, spreads, floors, ibor_curve, indexparams, discount_curve, dateparams);  }


// [[Rcpp::export]]
Rcpp::List FloatBond4(SEXP bond, std::vector<double> gearings, std::vector<double> caps,
                      std::vector<double> spreads, std::vector<double> floors, 
                      SEXP indexparams, SEXP index_params, 
                      SEXP index_tsQuotes, SEXP index_times,
                      SEXP disc_params, SEXP disc_tsQuotes,
                      SEXP disc_times, SEXP dateparams) {
    QuantLib::Handle<QuantLib::YieldTermStructure> ibor_curve(buildTermStructure(index_params, index_tsQuotes));
    QuantLib::Handle<QuantLib::YieldTermStructure> discount_curve(buildTermStructure(disc_params, disc_tsQuotes));
    return FloatingBond(bond, gearings, caps, spreads, floors, ibor_curve, indexparams, discount_curve, dateparams);  }

// [[Rcpp::export]]
Rcpp::List floatingWithRebuiltCurveEngine(SEXP bondparams, std::vector<double> gearings,
                                          std::vector<double> spreads, std::vector<double> caps,
                                          std::vector<double> floors, SEXP indexparams,
                                          SEXP iborDateSexp, SEXP iborzeroSexp,
                                          SEXP dateSexp, SEXP zeroSexp,
                                          SEXP dateparams) {

    QuantLib::Handle<QuantLib::YieldTermStructure> 
        ibor_curve(rebuildCurveFromZeroRates(iborDateSexp, iborzeroSexp));       
    QuantLib::Handle<QuantLib::YieldTermStructure> 
        curve(rebuildCurveFromZeroRates(dateSexp, zeroSexp));       
        
    return FloatingBond(bondparams, gearings, caps, spreads,
                        floors, ibor_curve, indexparams,
                        curve, dateparams);
}

// TODO: R interface -- cannot use Attribute with converter for Handle<>
// currently NOT exported but called below
Rcpp::List fixedBondEngine(Rcpp::List rparam, std::vector<double> rates,
                           QuantLib::Handle<QuantLib::YieldTermStructure> &discountCurve,
                           Rcpp::List datemisc) {

    double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
    QuantLib::Date maturityDate(Rcpp::as<QuantLib::Date>(rparam["maturityDate"]));
    QuantLib::Date issueDate(Rcpp::as<QuantLib::Date>(rparam["issueDate"]));
    QuantLib::Date effectiveDate(Rcpp::as<QuantLib::Date>(rparam["effectiveDate"]));
    double redemption = Rcpp::as<double>(rparam["redemption"]);

    double settlementDays = Rcpp::as<double>(datemisc["settlementDays"]);
    std::string cal = Rcpp::as<std::string>(datemisc["calendar"]);
    double dayCounter = Rcpp::as<double>(datemisc["dayCounter"]);
    double frequency = Rcpp::as<double>(datemisc["period"]);
    double businessDayConvention = Rcpp::as<double>(datemisc["businessDayConvention"]);
    double terminationDateConvention = Rcpp::as<double>(datemisc["terminationDateConvention"]);
    double dateGeneration = Rcpp::as<double>(datemisc["dateGeneration"]);
    double endOfMonthRule = Rcpp::as<double>(datemisc["endOfMonth"]);

    

    //set up BusinessDayConvetion
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
    QuantLib::BusinessDayConvention tbdc = getBusinessDayConvention(terminationDateConvention);
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    QuantLib::Frequency freq = getFrequency(frequency);
    QuantLib::DateGeneration::Rule rule = getDateGenerationRule(dateGeneration);
    bool endOfMonth = (endOfMonthRule==1) ? true : false;
    //set up calendar
    QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    if (cal == "us") {
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    } else if (cal == "uk") {
        calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
    }

    //build the bond
    QuantLib::Schedule sch(effectiveDate, maturityDate, QuantLib::Period(freq), calendar,
                           bdc, tbdc, rule, endOfMonth);
        
    QuantLib::FixedRateBond bond(settlementDays, faceAmount, sch, rates, 
                                 dc, bdc, redemption, issueDate);
    
    //bond price
    boost::shared_ptr<QuantLib::PricingEngine> 
        bondEngine(new QuantLib::DiscountingBondEngine(discountCurve));
    bond.setPricingEngine(bondEngine);   
        
    return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                              Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                              Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                              Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                              Rcpp::Named("yield") = bond.yield(QuantLib::Actual360(), 
                                                                QuantLib::Compounded, QuantLib::Annual),
                              Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));
}

// [[Rcpp::export]]
Rcpp::List FixedRateWithRebuiltCurve(Rcpp::List bondparam, 
                                     std::vector<double> ratesVec,
                                     SEXP dateSexp, 
                                     SEXP zeroSexp,
                                     Rcpp::List dateparams) {
    QuantLib::Handle<QuantLib::YieldTermStructure> 
        curve(rebuildCurveFromZeroRates(dateSexp, zeroSexp));
    return fixedBondEngine(bondparam, ratesVec, curve, dateparams);
}


// TODO: R interface -- cannot use Attribute with converter for Handle<>
// currently NOT exported but called below
Rcpp::List zeroBondEngine(Rcpp::List rparam, 
                          QuantLib::Handle<QuantLib::YieldTermStructure> &discountCurve,
                          Rcpp::List datemisc) {
    
    double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
    QuantLib::Date maturityDate(Rcpp::as<QuantLib::Date>(rparam["maturityDate"]));
    QuantLib::Date issueDate(Rcpp::as<QuantLib::Date>(rparam["issueDate"]));
    double redemption = Rcpp::as<double>(rparam["redemption"]);

    double settlementDays = Rcpp::as<double>(datemisc["settlementDays"]);
    std::string cal = Rcpp::as<std::string>(datemisc["calendar"]);
    double businessDayConvention = Rcpp::as<double>(datemisc["businessDayConvention"]);
    QuantLib::Date refDate(Rcpp::as<QuantLib::Date>(datemisc["refDate"]));      
    QuantLib::Settings::instance().evaluationDate() = refDate;                               
        
    /*
      test-suite/bonds.cpp
    */      

    //set up QuantLib::BusinessDayConvetion
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        
    // set up calendar -- FIXME: use utils function getCalendar instead
    QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    if (cal == "us") {
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    } else if (cal == "uk") {
        calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
    }
        
    QuantLib::ZeroCouponBond bond(settlementDays, calendar, faceAmount,
                                  maturityDate, bdc, redemption, issueDate);
        
    boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountCurve));
    bond.setPricingEngine(bondEngine);

    return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                              Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                              Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                              Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                              Rcpp::Named("yield") = bond.yield(QuantLib::Actual360(), 
                                                                QuantLib::Compounded, QuantLib::Annual),
                              Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));
}

// [[Rcpp::export]]
Rcpp::List ZeroBondWithRebuiltCurve(SEXP bond,
                                    SEXP dateSexp, 
                                    SEXP zeroSexp,
                                    SEXP dateparams) {
    QuantLib::Handle<QuantLib::YieldTermStructure> 
        curve(rebuildCurveFromZeroRates(dateSexp, zeroSexp));
    return zeroBondEngine(bond, curve, dateparams);
}

// [[Rcpp::export]]
Rcpp::List convertibleZeroBondEngine(Rcpp::List rparam, 
                                     Rcpp::List processParam,
                                     SEXP dividendYieldDateSexp,
                                     SEXP dividendYieldZeroSexp,
                                     SEXP rffDateSexp, SEXP rffZeroSexp,
                                     Rcpp::DataFrame dividendScheduleFrame,
                                     Rcpp::DataFrame callabilityScheduleFrame,
                                     Rcpp::List datemisc) {

    QuantLib::DividendSchedule dividendSchedule = getDividendSchedule(dividendScheduleFrame);
    QuantLib::CallabilitySchedule 
        callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);

    //double faceAmount = Rcpp::as<double>(rparam["faceAmount");        
    QuantLib::Date maturityDate(Rcpp::as<QuantLib::Date>(rparam["maturityDate"]));
    QuantLib::Date issueDate(Rcpp::as<QuantLib::Date>(rparam["issueDate"]));
    double redemption = Rcpp::as<double>(rparam["redemption"]);
    std::string exercise = Rcpp::as<std::string>(rparam["exercise"]);
    double creditSpreadQuote = Rcpp::as<double>(rparam["creditSpread"]);
    double conversionRatio = Rcpp::as<double>(rparam["conversionRatio"]);

    double settlementDays = Rcpp::as<double>(datemisc["settlementDays"]);
    std::string cal = Rcpp::as<std::string>(datemisc["calendar"]);
    double dayCounter = Rcpp::as<double>(datemisc["dayCounter"]);
    double frequency = Rcpp::as<double>(datemisc["period"]);
    double businessDayConvention = Rcpp::as<double>(datemisc["businessDayConvention"]);
        
    QuantLib::Date todayDate = issueDate;
        
    QuantLib::Calendar calendar = 
        QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    if (cal == "us") {
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    } else if (cal == "uk") {
        calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
    }
 
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    QuantLib::Frequency freq = getFrequency(frequency);

    QuantLib::RelinkableHandle<QuantLib::Quote> underlying;
    QuantLib::RelinkableHandle<QuantLib::BlackVolTermStructure> volatility;
    boost::shared_ptr<QuantLib::BlackScholesMertonProcess> blackProcess;

    QuantLib::Handle<QuantLib::YieldTermStructure> 
        dividendYield(rebuildCurveFromZeroRates(dividendYieldDateSexp,
                                                dividendYieldZeroSexp));

    QuantLib::Handle<QuantLib::YieldTermStructure> 
        rff(rebuildCurveFromZeroRates(rffDateSexp, rffZeroSexp));

    double underlyingQuote = Rcpp::as<double>(processParam["underlying"]);
    double volatilityQuote = Rcpp::as<double>(processParam["volatility"]);
    underlying.linkTo(boost::shared_ptr<QuantLib::Quote>(new QuantLib::SimpleQuote(underlyingQuote)));
    boost::shared_ptr<QuantLib::SimpleQuote> vol(new QuantLib::SimpleQuote( volatilityQuote ));
    volatility.linkTo(flatVol(todayDate, vol, dc));

    blackProcess = 
        boost::shared_ptr<QuantLib::BlackScholesMertonProcess>(new QuantLib::BlackScholesMertonProcess(underlying, dividendYield,
                                                                                                       rff, volatility));

    QuantLib::RelinkableHandle<QuantLib::Quote> creditSpread;
    creditSpread.linkTo(boost::shared_ptr<QuantLib::Quote>(new QuantLib::SimpleQuote(creditSpreadQuote)));

    boost::shared_ptr<QuantLib::Exercise> euExercise(new QuantLib::EuropeanExercise(maturityDate));
    boost::shared_ptr<QuantLib::Exercise> amExercise(new QuantLib::AmericanExercise(issueDate, maturityDate));
    boost::shared_ptr<QuantLib::Exercise> ex = (exercise == "eu") ? euExercise : amExercise;
        
    QuantLib::Size timeSteps = 1001;
    boost::shared_ptr<QuantLib::PricingEngine> 
        engine(new QuantLib::BinomialConvertibleEngine<QuantLib::CoxRossRubinstein>(blackProcess, timeSteps));
        
    QuantLib::Handle<QuantLib::YieldTermStructure> 
        discountCurve(boost::shared_ptr<QuantLib::YieldTermStructure>(new QuantLib::ForwardSpreadedTermStructure(rff, creditSpread)));
        
    QuantLib::Schedule sch(issueDate, maturityDate, QuantLib::Period(freq), calendar,
                           bdc, bdc, QuantLib::DateGeneration::Backward, false);        
    QuantLib::ConvertibleZeroCouponBond bond(ex, conversionRatio,
                                             dividendSchedule, callabilitySchedule,
                                             creditSpread,
                                             issueDate, settlementDays,
                                             dc, sch,
                                             redemption);
    bond.setPricingEngine(engine);

    return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                              Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                              Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                              Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                              Rcpp::Named("yield") = bond.yield(QuantLib::Actual360(), 
                                                                QuantLib::Compounded, 
                                                                QuantLib::Annual),
                              Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));
}

// [[Rcpp::export]]
Rcpp::List convertibleFixedBondEngine(Rcpp::List rparam, 
                                      Rcpp::NumericVector rates, 
                                      Rcpp::List processParam,
                                      SEXP dividendYieldDateSexp,
                                      SEXP dividendYieldZeroSexp,
                                      SEXP rffDateSexp, SEXP rffZeroSexp,
                                      Rcpp::DataFrame dividendScheduleFrame,
                                      Rcpp::DataFrame callabilityScheduleFrame,
                                      Rcpp::List datemisc) {

    QuantLib::DividendSchedule dividendSchedule = getDividendSchedule(dividendScheduleFrame);
    QuantLib::CallabilitySchedule callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);

    //double faceAmount = Rcpp::as<double>(rparam["faceAmount");
    QuantLib::Date maturityDate(Rcpp::as<QuantLib::Date>(rparam["maturityDate"]));
    QuantLib::Date issueDate(Rcpp::as<QuantLib::Date>(rparam["issueDate"]));
    double redemption = Rcpp::as<double>(rparam["redemption"]);
    std::string exercise = Rcpp::as<std::string>(rparam["exercise"]);
    double creditSpreadQuote = Rcpp::as<double>(rparam["creditSpread"]);
    double conversionRatio = Rcpp::as<double>(rparam["conversionRatio"]);

    double settlementDays = Rcpp::as<double>(datemisc["settlementDays"]);
    std::string cal = Rcpp::as<std::string>(datemisc["calendar"]);
    double dayCounter = Rcpp::as<double>(datemisc["dayCounter"]);
    double frequency = Rcpp::as<double>(datemisc["period"]);
    double businessDayConvention = Rcpp::as<double>(datemisc["businessDayConvention"]);
       
    QuantLib::Date todayDate = issueDate;
    QuantLib::Settings::instance().evaluationDate() = todayDate;
    QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    if (cal == "us") {
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    } else if (cal == "uk") {
        calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
    }
 
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    QuantLib::Frequency freq = getFrequency(frequency);

    QuantLib::RelinkableHandle<QuantLib::Quote> underlying;
    QuantLib::RelinkableHandle<QuantLib::BlackVolTermStructure> volatility;
    
    QuantLib::Handle<QuantLib::YieldTermStructure> 
        dividendYield(rebuildCurveFromZeroRates(dividendYieldDateSexp, dividendYieldZeroSexp));

    QuantLib::Handle<QuantLib::YieldTermStructure> rff(rebuildCurveFromZeroRates(rffDateSexp, rffZeroSexp));

    double underlyingQuote = Rcpp::as<double>(processParam["underlying"]);
    double volatilityQuote = Rcpp::as<double>(processParam["volatility"]);
    underlying.linkTo(boost::shared_ptr<QuantLib::Quote>(new QuantLib::SimpleQuote(underlyingQuote)));
    boost::shared_ptr<QuantLib::SimpleQuote> vol(new QuantLib::SimpleQuote( volatilityQuote ));
    volatility.linkTo(flatVol(todayDate, vol, dc));

    boost::shared_ptr<QuantLib::BlackScholesMertonProcess> blackProcess;
    blackProcess = 
        boost::shared_ptr<QuantLib::BlackScholesMertonProcess>(new QuantLib::BlackScholesMertonProcess(underlying, dividendYield, rff, volatility));
        //	boost::shared_ptr<QuantLib::BlackScholesProcess> blackProcess;
        //ackProcess = boost::shared_ptr<QuantLib::BlackScholesProcess>(
        //					      new QuantLib::BlackScholesProcess(underlying, 
        //								      rff, volatility));

    QuantLib::RelinkableHandle<QuantLib::Quote> creditSpread;
    creditSpread.linkTo(boost::shared_ptr<QuantLib::Quote>(new QuantLib::SimpleQuote(creditSpreadQuote)));

    boost::shared_ptr<QuantLib::Exercise> euExercise(new QuantLib::EuropeanExercise(maturityDate));
    boost::shared_ptr<QuantLib::Exercise> amExercise(new QuantLib::AmericanExercise(issueDate, maturityDate));
    
    boost::shared_ptr<QuantLib::Exercise> ex = (exercise == "eu") ? euExercise : amExercise;
        
    QuantLib::Size timeSteps = 1001;
    boost::shared_ptr<QuantLib::PricingEngine> 
        engine(new QuantLib::BinomialConvertibleEngine<QuantLib::CoxRossRubinstein>(blackProcess, timeSteps));
        
    QuantLib::Handle<QuantLib::YieldTermStructure> 
        discountCurve(boost::shared_ptr<QuantLib::YieldTermStructure>(new QuantLib::ForwardSpreadedTermStructure(rff,
                                                                                                 creditSpread)));
        
    QuantLib::Schedule sch(issueDate, maturityDate,
                           QuantLib::Period(freq), calendar,
                           bdc, bdc,
                           QuantLib::DateGeneration::Backward, false);        
    QuantLib::ConvertibleFixedCouponBond bond(ex, conversionRatio,
                                              dividendSchedule, callabilitySchedule,
                                              creditSpread,issueDate, 
                                              settlementDays,
                                              Rcpp::as<std::vector <double> >(rates), 
                                              dc, sch, redemption);
    bond.setPricingEngine(engine);
        
    return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                              Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                              Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                              Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                              Rcpp::Named("yield") = bond.yield(QuantLib::Actual360(), 
                                                                QuantLib::Compounded, 
                                                                QuantLib::Annual),
                              Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

}


// [[Rcpp::export]]
Rcpp::List convertibleFloatingBondEngine(Rcpp::List rparam,  
                                         Rcpp::List processParam,
                                         SEXP dividendYieldDateSexp,
                                         SEXP dividendYieldZeroSexp,
                                         SEXP rffDateSexp, SEXP rffZeroSexp,
                                         SEXP iborIndexDateSexp, SEXP iborIndexZeroSexp,
                                         Rcpp::List iborparams, 
                                         std::vector<double> spreads,
                                         Rcpp::DataFrame dividendScheduleFrame,
                                         Rcpp::DataFrame callabilityScheduleFrame,
                                         Rcpp::List datemisc) {

    QuantLib::DividendSchedule dividendSchedule = getDividendSchedule(dividendScheduleFrame);
    QuantLib::CallabilitySchedule callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);
    
    //double faceAmount = Rcpp::as<double>(rparam["faceAmount");        
    QuantLib::Date maturityDate(Rcpp::as<QuantLib::Date>(rparam["maturityDate"]));
    QuantLib::Date issueDate(Rcpp::as<QuantLib::Date>(rparam["issueDate"]));
    double redemption = Rcpp::as<double>(rparam["redemption"]);
    std::string exercise = Rcpp::as<std::string>(rparam["exercise"]);
    double creditSpreadQuote = Rcpp::as<double>(rparam["creditSpread"]);
    double conversionRatio = Rcpp::as<double>(rparam["conversionRatio"]);

    //extract iborindex curve
    QuantLib::Handle<QuantLib::YieldTermStructure> index(rebuildCurveFromZeroRates(iborIndexDateSexp, iborIndexZeroSexp));

    std::string type = Rcpp::as<std::string>(iborparams["type"]);
    double length = Rcpp::as<double>(iborparams["length"]);
    std::string inTermOf = Rcpp::as<std::string>(iborparams["inTermOf"]);

    boost::shared_ptr<QuantLib::IborIndex> iborindex(new QuantLib::USDLibor(6 * QuantLib::Months, index));
    if (type=="USDLibor") {
        if (inTermOf=="Months") {               
            boost::shared_ptr<QuantLib::IborIndex> temp(new QuantLib::USDLibor(length * QuantLib::Months, index));
            iborindex = temp;
        } else if (inTermOf=="Years") {
            boost::shared_ptr<QuantLib::IborIndex> temp(new QuantLib::USDLibor(length * QuantLib::Years, index));
            iborindex = temp;
        }
    }

    double settlementDays = Rcpp::as<double>(datemisc["settlementDays"]);
    std::string cal = Rcpp::as<std::string>(datemisc["calendar"]);
    double dayCounter = Rcpp::as<double>(datemisc["dayCounter"]);
    double frequency = Rcpp::as<double>(datemisc["period"]);
    double businessDayConvention = Rcpp::as<double>(datemisc["businessDayConvention"]);

    QuantLib::Date todayDate = issueDate;
        
    QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    if (cal == "us") {
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    } else if (cal == "uk") {
        calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
    }
 
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    QuantLib::Frequency freq = getFrequency(frequency);
        
    QuantLib::RelinkableHandle<QuantLib::Quote> underlying;
    QuantLib::RelinkableHandle<QuantLib::BlackVolTermStructure> volatility;
    boost::shared_ptr<QuantLib::BlackScholesMertonProcess> blackProcess;

    QuantLib::Handle<QuantLib::YieldTermStructure> dividendYield(rebuildCurveFromZeroRates(dividendYieldDateSexp,
                                                                                           dividendYieldZeroSexp));

    QuantLib::Handle<QuantLib::YieldTermStructure> rff(rebuildCurveFromZeroRates(rffDateSexp,
                                                                                 rffZeroSexp));

    double underlyingQuote = Rcpp::as<double>(processParam["underlying"]);
    double volatilityQuote = Rcpp::as<double>(processParam["volatility"]);
    underlying.linkTo(boost::shared_ptr<QuantLib::Quote>(new QuantLib::SimpleQuote(underlyingQuote)));
    boost::shared_ptr<QuantLib::SimpleQuote> vol(new QuantLib::SimpleQuote( volatilityQuote ));
    volatility.linkTo(flatVol(todayDate, vol, dc));

    blackProcess = 
        boost::shared_ptr<QuantLib::BlackScholesMertonProcess>(new QuantLib::BlackScholesMertonProcess(underlying, dividendYield, rff, volatility));

    QuantLib::RelinkableHandle<QuantLib::Quote> creditSpread;
    creditSpread.linkTo(boost::shared_ptr<QuantLib::Quote>(new QuantLib::SimpleQuote(creditSpreadQuote)));
    
    boost::shared_ptr<QuantLib::Exercise> euExercise(new QuantLib::EuropeanExercise(maturityDate));
    boost::shared_ptr<QuantLib::Exercise> amExercise(new QuantLib::AmericanExercise(issueDate, maturityDate));
        
    boost::shared_ptr<QuantLib::Exercise> ex = (exercise == "eu") ? euExercise : amExercise;
        
        
    QuantLib::Size timeSteps = 1001;
    boost::shared_ptr<QuantLib::PricingEngine> engine(new QuantLib::BinomialConvertibleEngine<QuantLib::CoxRossRubinstein>(blackProcess, timeSteps));
        
    QuantLib::Handle<QuantLib::YieldTermStructure> 
        discountCurve(boost::shared_ptr<QuantLib::YieldTermStructure>(new QuantLib::ForwardSpreadedTermStructure(rff,
                                                                                                                 creditSpread)));
    QuantLib::Natural fixingDays = 2;
    QuantLib::Schedule sch(issueDate, maturityDate, QuantLib::Period(freq), 
                           calendar, bdc, bdc, QuantLib::DateGeneration::Backward, false);        
    QuantLib::ConvertibleFloatingRateBond bond(ex, conversionRatio, dividendSchedule, callabilitySchedule,
                                               creditSpread,issueDate, 
                                               settlementDays,iborindex,fixingDays, spreads,
                                               dc, sch, redemption);
    bond.setPricingEngine(engine);

    return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                              Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                              Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                              Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                              Rcpp::Named("yield") = bond.yield(QuantLib::Actual360(), QuantLib::Compounded, QuantLib::Annual),
                              Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));
}


// [[Rcpp::export]]
Rcpp::List callableBondEngine(Rcpp::List rparam, 
                              Rcpp::List hwparam, 
                              Rcpp::NumericVector coupon,
                              Rcpp::DataFrame callabilityScheduleFrame,
                              Rcpp::List datemisc) {

    QuantLib::CallabilitySchedule callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);

    double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);        
    QuantLib::Date maturityDate(Rcpp::as<QuantLib::Date>(rparam["maturityDate"]));
    QuantLib::Date issueDate(Rcpp::as<QuantLib::Date>(rparam["issueDate"]));
    double redemption = Rcpp::as<double>(rparam["redemption"]);

    double settlementDays = Rcpp::as<double>(datemisc["settlementDays"]);

    std::string cal = Rcpp::as<std::string>(datemisc["calendar"]);
    double dayCounter = Rcpp::as<double>(datemisc["dayCounter"]);
    double frequency = Rcpp::as<double>(datemisc["period"]);
    double businessDayConvention = Rcpp::as<double>(datemisc["businessDayConvention"]);
        
    QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    if (cal == "us") {
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    } else if (cal == "uk") {
        calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
    }

    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    QuantLib::Frequency freq = getFrequency(frequency);
        
    //extract coupon rates vector
    Rcpp::NumericVector rates(coupon); 
        
    double alpha = Rcpp::as<double>(hwparam["alpha"]);
    double sigma = Rcpp::as<double>(hwparam["sigma"]);
    double gridIntervals = Rcpp::as<double>(hwparam["gridIntervals"]);
    double rate = Rcpp::as<double>(hwparam["term"]);
        
    boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(rate));
    QuantLib::Handle<QuantLib::YieldTermStructure> termStructure(flatRate(issueDate,rRate,QuantLib::Actual360()));

    //QuantLib::Handle<QuantLib::YieldTermStructure> termStructure(rebuildCurveFromZeroRates(
    //                                                               hwTermDateSexp,
    //                                                               hwTermZeroSexp));
      
    boost::shared_ptr<QuantLib::ShortRateModel> 
        hw0(new QuantLib::HullWhite(termStructure,alpha,sigma));

    boost::shared_ptr<QuantLib::PricingEngine> 
        engine0(new QuantLib::TreeCallableFixedRateBondEngine(hw0,gridIntervals));

    QuantLib::Schedule sch(issueDate, maturityDate,
                           QuantLib::Period(freq), calendar, bdc, bdc,
                           QuantLib::DateGeneration::Backward, false);        

    //std::cout << "RQL SettleDate    : " << RQLContext::instance().settleDate << std::endl;
    //std::cout << "RQL calendar      : " << RQLContext::instance().calendar << std::endl;
    //std::cout << "RQL fixingDays    : " << RQLContext::instance().fixingDays << std::endl;

    QuantLib::CallableFixedRateBond bond(settlementDays, faceAmount, sch,
                                         Rcpp::as<std::vector <double> >(rates), 
                                         dc, bdc, redemption, issueDate, 
                                         callabilitySchedule);
    bond.setPricingEngine(engine0);

    return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                              Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                              Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                              Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                              Rcpp::Named("yield") = bond.yield(dc, QuantLib::Compounded, freq),
                              Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

    return R_NilValue;
}

// RcppExport SEXP CMSBond(SEXP bondparams, SEXP iborIndex, SEXP swapIndexParam, 
//                            SEXP capsVec, SEXP floorsVec, SEXP gearingsVec, 
//                            SEXP spreadsVec, SEXP swaptionVolSEXP, SEXP atmOptionTenorsSEXP,
//                            SEXP atmSwapTenorsSEXP, SEXP volMatrixSEXP, SEXP pricer,
//                            SEXP iborIndexDate, SEXP iborIndexRates)
// {
//     SEXP rl=R_NilValue;
//     char* exceptionMesg=NULL;
//     try {

//         std::vector<double> gearings = getDoubleVector(gearingsVec);
//         std::vector<double> spreads = getDoubleVector(spreadsVec);
//         std::vector<double> caps = getDoubleVector(capsVec);
//         std::vector<double> floors = getDoubleVector(floorsVec);

//         RcppStringVector strVec(atmOptionTenorsSEXP);
//         std::vector<string> atmOptionTenors(strVec.stlVector());
//         RcppStringVector strVec2(atmSwapTenorsSEXP);
//         std::vector<string> atmSwapTenors(strVec2.stlVector());

//         RcppMatrix<double> m(volMatrixSEXP);
        
//         QuantLib::Handle<QuantLib::YieldTermStructure> termStructure(rebuildCurveFromZeroRates(
//                                                                            iborIndexDate,iborIndexRates));
//         Rcppparams iborparams(iborIndex);
//         std::string ibortype = iborparams.getStringValue("type");
//         std::string iborlength = iborparams.getStringValue("length");
//         boost::shared_ptr<QuantLib::IborIndex> ibor = getIborIndex(termStructure, ibortype, 
//                                                          iborlength);
//         //fix tenor to make sure it is converted by matchparam
//         Rcppparams swapparams(swapIndexParam);
//         std::string familyName = swapparams.getStringValue("familyName");
//         std::double tenor = swapparams.getDoubleValue("tenor");
//         std::double settlementDays = swapparams.getDoubleValue("settlementDays");
//         std::string currency = swapparams.getStringValue("currency");
//         std::string fixedLegTenor = swapparams.getDoubleValue("fixedLegTenor");
//         std::string fixedLegConvention = swapparams.getDoubleValue("fixedLegConvention");
//         std::string fixedLegDayCounter = swapparams.getDoubleValue("fixedLegDayCounter");
//         std::string cal = swapparams.getStringValue("calendar");
//         QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
//         if (cal == "us"){
//             calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
//         }
//         else if (cal == "uk"){
//             calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
//         }
//         QuantLib::BusinessDayConvention fixedLegBDC = getBusinessDayConvention(fixedLegConvention);
//         QuantLib::DayCounter fixedLedDC = getDayCounter(fixedLegDayCounter);

//         boost::shared_ptr<QuantLib::SwapIndex> swapIndex(new QuantLib::SwapIndex(familiName, 
//                                                              getPeriodFromString(fixedLegTenor),
//                                                              settlemenDays, 
//                                                              currency, 
//                                                              calendar,
//                                                              getPeriodFromString(fixedLegTenor), 
//                                                              fixedLegBDC, 
//                                                              fixedLegDC, 
//                                                              ibor));

//         Rcppparams pricerparams(pricer);
//         std::string pricerType = pricerparams.getStringValue("type");
//         std::double zeroMeanRev = pricerparams.getDoubleValue("zeroMeanRev");
        
//         Rcppparams swaptionVolParams(swaptionVolSEXP);
//         std::string swaptionCal = swaptionVolParams.getStringValue("calendar");
//         std::double swaptionBDC = swaptionVolParams.getDoubleValue("businessDayConvention");
//         std::double swaptionDC = swaptionVolParams.getDoubleValue("dayCounter");
//         QuantLib::Handle<QuantLib::SwaptionVolatilityStructure> atmVol;
//         atmVol = QuantLib::Handle<QuantLib::SwaptionVolatilityStructure>(
//                                                      boost::shared_ptr<QuantLib::SwaptionVolatilityStructure>
//                                                      new QuantLib::SwaptionVolatilityMatrix(swapCal,
//                                                                                   swaptionBDC,
//                                                                                   atmOptionTenors,
//                                                                                   atmSwapTenors,
//                                                                                   m,
//                                                                                   swaptionDC));


//         boost::shared_ptr<QuantLib::CmsCouponPricer> pricer(new QuantLib::NumericHaganPricer(atmVol, yieldCurveModel,
//                                                                          zeroMeanRev));
        

//         Rcppparams rparams(bondparams);        
//         Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
//         Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
//         Rcpp::Date pDate = Rcpp::Date(Rcpp::as<int>(rparam["paymentDate"]));
//         QuantLib::Date maturityDate(dateFromR(mDate));
//         QuantLib::Date issueDate(dateFromR(iDate));
//         QuantLib::Date paymentDate(dateFromR(pDate));
//         std::double nomial = rparams.getDoubleValue("nomial");
        
//         CappedFlooredCmsCoupon coupon(paymentDate, nomial,
//                                       issueDate, maturityDate, 
//                                       settlementDays, swapIndex,
//                                       gearings, spreads, 
//                                       caps, floors,
//                                       issueDate, maturityDate,
//                                       dayCounter);

//         pricer->setSwaptionVolatility(atmVol);
//         coupon.setPricer(pricer);
        

        
//     } catch(std::exception& ex) {
//         exceptionMesg = copyMessageToR(ex.what());
//     } catch(...) {
//         exceptionMesg = copyMessageToR("unknown reason");
//     }
    
//     if(exceptionMesg != NULL)
//         Rf_error(exceptionMesg);
    
//     return rl;
// }

// [[Rcpp::export]]
Rcpp::List fittedBondCurveEngine(Rcpp::List curveparam, 
                                 Rcpp::NumericVector length,
                                 Rcpp::NumericVector coupons,
                                 Rcpp::NumericVector marketQuotes,
                                 Rcpp::List datemisc) {

    double settlementDays = Rcpp::as<double>(datemisc["settlementDays"]);
    double dayCounter = Rcpp::as<double>(datemisc["dayCounter"]);
    double frequency = Rcpp::as<double>(datemisc["period"]);
    double businessDayConvention = Rcpp::as<double>(datemisc["businessDayConvention"]);
    
    std::string method = Rcpp::as<std::string>(curveparam["method"]);
    QuantLib::Date origDate(Rcpp::as<QuantLib::Date>(curveparam["origDate"]));;
    QuantLib::Settings::instance().evaluationDate() = origDate;

    const QuantLib::Size numberOfBonds = length.size();

    std::vector< boost::shared_ptr<QuantLib::SimpleQuote> > quote;
    for (QuantLib::Size i=0; i<numberOfBonds; i++) {            
        boost::shared_ptr<QuantLib::SimpleQuote> cp(new QuantLib::SimpleQuote(marketQuotes[i]));
        quote.push_back(cp);
    }

    std::vector< QuantLib::RelinkableHandle<QuantLib::Quote> > quoteHandle(numberOfBonds);
    for (QuantLib::Size i=0; i<numberOfBonds; i++) {
        quoteHandle[i].linkTo(quote[i]);
    }
        
    QuantLib::Calendar calendar =  QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    QuantLib::Frequency freq = getFrequency(frequency);
    QuantLib::Real redemption = 100;
        
    std::vector<boost::shared_ptr<QuantLib::BondHelper> > instrumentsA;
        
    for (QuantLib::Size j=0; j < static_cast<QuantLib::Size>(length.size()); j++) {

        QuantLib::Date dated = origDate;
        QuantLib::Date issue = origDate;
        QuantLib::Date maturity = calendar.advance(issue, length[j], QuantLib::Years);
            
        QuantLib::Schedule schedule(dated, maturity, QuantLib::Period(freq), calendar,
                                    bdc, bdc,
                                    QuantLib::DateGeneration::Backward, false);

        boost::shared_ptr<QuantLib::FixedRateBond> 
            bond(new QuantLib::FixedRateBond(settlementDays, 100.0, schedule,
                                             std::vector<QuantLib::Rate>(1,coupons[j]),
                                             dc, bdc, redemption, issue, calendar));
        boost::shared_ptr<QuantLib::BondHelper> helperA(new QuantLib::BondHelper(quoteHandle[j], bond));
        instrumentsA.push_back(helperA);

    }

    bool constrainAtZero = true;
    QuantLib::Real tolerance = 1.0e-10;
    QuantLib::Size max = 5000;

    boost::shared_ptr<QuantLib::YieldTermStructure> curve;

       
    if (method=="ExponentialSplinesFitting") {
        QuantLib::ExponentialSplinesFitting exponentialSplines(constrainAtZero);

        boost::shared_ptr<QuantLib::FittedBondDiscountCurve> 
            ts1 (new QuantLib::FittedBondDiscountCurve(settlementDays, calendar, instrumentsA,
                                                       dc, exponentialSplines, tolerance, max));
        curve = ts1;

    } else if (method == "SimplePolynomialFitting") {
        double degree = Rcpp::as<double>(curveparam["degree"]);
        QuantLib::SimplePolynomialFitting simplePolynomial(degree, constrainAtZero);

        boost::shared_ptr<QuantLib::FittedBondDiscountCurve> 
            ts2 (new QuantLib::FittedBondDiscountCurve(settlementDays, calendar, instrumentsA, dc,
                                                       simplePolynomial, tolerance, max));
        curve = ts2;

    } else if (method == "NelsonSiegelFitting") {
        QuantLib::NelsonSiegelFitting nelsonSiegel;

        boost::shared_ptr<QuantLib::FittedBondDiscountCurve> 
            ts3 (new QuantLib::FittedBondDiscountCurve(settlementDays, calendar, instrumentsA, dc,
                                                       nelsonSiegel, tolerance, max));
        curve = ts3;
    }
        
    // Return discount, forward rate, and zero coupon curves
    // int numCol = 3;
    // std::vector<std::string> colNames(numCol);
    // colNames[0] = "date";
    // colNames[1] = "zeroRates";
    // colNames[2] = "discount";
    // RcppFrame frame(colNames);
    QuantLib::Date current = curve->referenceDate();;
    int n = curve->maxDate() - curve->referenceDate();
    //std::cout << curve->maxDate() << " " << curve->referenceDate() << " " << n << std::endl;

    Rcpp::DateVector dates(n);
    Rcpp::NumericVector zr(n);
    Rcpp::NumericVector di(n);

    for (int i = 0; i < n; i++) {
        QuantLib::Date d = current; 
        dates[i] =  Rcpp::Date(d.month(), d.dayOfMonth(), d.year());
        zr[i] = curve->zeroRate(current, QuantLib::ActualActual(), QuantLib::Continuous);
        di[i] = curve->discount(current);
        current++;
    }
    Rcpp::DataFrame frame = Rcpp::DataFrame::create(Rcpp::Named("date") = dates,
                                                    Rcpp::Named("zeroRates") = zr,
                                                    Rcpp::Named("discount") = di);
    return Rcpp::List::create(Rcpp::Named("table") = frame);

}
