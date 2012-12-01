// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2002 - 2012 Dirk Eddelbuettel 
// Copyright (C) 2009 - 2012 Khanh Nguyen and Dirk Eddelbuettel
//
// $Id$
//
// This file is part of the RQuantLib library for GNU R.
// It is made available under the terms of the GNU General Public
// License, version 2, or at your option, any later version,
// incorporated herein by reference.
//
// This program is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA

#include <rquantlib.h>

RcppExport SEXP ZeroPriceByYield(SEXP optionParameters) {

    try {
        Rcpp::List rparam(optionParameters);
        double yield = Rcpp::as<double>(rparam["yield"]);
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
        double dayCounter = Rcpp::as<double>(rparam["dayCounter"]);
        double frequency = Rcpp::as<double>(rparam["frequency"]);
        double businessDayConvention = Rcpp::as<double>(rparam["businessDayConvention"]);
        double compound = Rcpp::as<double>(rparam["compound"]);
        QuantLib::Date maturityDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["maturityDate"])));
        QuantLib::Date issueDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["issueDate"])));
        //setup bond
        QuantLib::Integer fixingDays = 2;
        QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        QuantLib::Date todaysDate = calendar.advance(issueDate, -fixingDays, QuantLib::Days);
        QuantLib::Settings::instance().evaluationDate() = todaysDate;
        QuantLib::Natural settlementDays = 1;
       
        QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        double redemption = 100;
        QuantLib::ZeroCouponBond zbond(settlementDays, calendar,
                                       faceAmount, maturityDate,
                                       bdc, redemption, issueDate);
       
        //return cleanPrice
        QuantLib::DayCounter dc = getDayCounter(dayCounter);
        QuantLib::Compounding cp = getCompounding(compound);
        QuantLib::Frequency freq = getFrequency(frequency);
        return Rcpp::wrap(zbond.cleanPrice(yield, dc, cp, freq));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP ZeroYield(SEXP optionParameters) {

    try {
        Rcpp::List rparam(optionParameters);
        double price = Rcpp::as<double>(rparam["price"]);
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
        double dayCounter = Rcpp::as<double>(rparam["dayCounter"]);
        double frequency = Rcpp::as<double>(rparam["frequency"]);
        double businessDayConvention = Rcpp::as<double>(rparam["businessDayConvention"]);
        double compound = Rcpp::as<double>(rparam["compound"]);
        QuantLib::Date maturityDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["maturityDate"])));
        QuantLib::Date issueDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["issueDate"])));
        //setup bond
        QuantLib::Integer fixingDays = 2;
        QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        QuantLib::Date todaysDate = calendar.advance(issueDate, -fixingDays, QuantLib::Days);
        QuantLib::Settings::instance().evaluationDate() = todaysDate;
        QuantLib::Natural settlementDays = 1;
       
        QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        double redemption = 100;
        QuantLib::ZeroCouponBond zbond(settlementDays, calendar,
                                       faceAmount, maturityDate,
                                       bdc, redemption, issueDate);
       
        //return yield
        QuantLib::DayCounter dc = getDayCounter(dayCounter);
        QuantLib::Compounding cp = getCompounding(compound);
        QuantLib::Frequency freq = getFrequency(frequency);
        return Rcpp::wrap(zbond.yield(price, dc, cp, freq));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }
    
    return R_NilValue;
}


RcppExport SEXP ZeroBond(SEXP bondparam, 
                         QuantLib::Handle<QuantLib::YieldTermStructure> &discountCurve,
                         SEXP dateparams) {

    try {
        Rcpp::List rparam(bondparam);
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
        QuantLib::Date maturityDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["maturityDate"])));
        QuantLib::Date issueDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["issueDate"])));
        double redemption = Rcpp::as<double>(rparam["redemption"]);

        Rcpp::List misc(dateparams);
        double settlementDays = Rcpp::as<double>(misc["settlementDays"]);
        std::string cal = Rcpp::as<std::string>(misc["calendar"]);
        double businessDayConvention = Rcpp::as<double>(misc["businessDayConvention"]);
        QuantLib::Date refDate(dateFromR(Rcpp::as<Rcpp::Date>(misc["refDate"])));      
        QuantLib::Settings::instance().evaluationDate() = refDate;                               
        
        /*
          test-suite/bonds.cpp
        */      

        //set up QuantLib::BusinessDayConvetion
        QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        
        // set up calendar -- FIXME: use utils function getCalendar instead
        QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
        }
        
        QuantLib::ZeroCouponBond bond(settlementDays,
                                      calendar,
                                      faceAmount,
                                      maturityDate,
                                      bdc,
                                      redemption, issueDate);
        
        boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountCurve));
        bond.setPricingEngine(bondEngine);

        return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                                  Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                                  Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                                  Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                                  Rcpp::Named("yield") = bond.yield(QuantLib::Actual360(), QuantLib::Compounded, QuantLib::Annual),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP FixedBond(SEXP bondparam, SEXP ratesVec,
                          QuantLib::Handle<QuantLib::YieldTermStructure> &discountCurve,
                          SEXP dateparams){

    try {
        Rcpp::List rparam(bondparam);
        
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
        QuantLib::Date maturityDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["maturityDate"])));
        QuantLib::Date issueDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["issueDate"])));
        QuantLib::Date effectiveDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["effectiveDate"])));
        double redemption = Rcpp::as<double>(rparam["redemption"]);

        Rcpp::List misc(dateparams);      
        double settlementDays = Rcpp::as<double>(misc["settlementDays"]);
        std::string cal = Rcpp::as<std::string>(misc["calendar"]);
        double dayCounter = Rcpp::as<double>(misc["dayCounter"]);
        double frequency = Rcpp::as<double>(misc["period"]);
        double businessDayConvention = Rcpp::as<double>(misc["businessDayConvention"]);
        double terminationDateConvention = Rcpp::as<double>(misc["terminationDateConvention"]);
        double dateGeneration = Rcpp::as<double>(misc["dateGeneration"]);
        double endOfMonthRule = Rcpp::as<double>(misc["endOfMonth"]);

        //extract coupon rates vector
        Rcpp::NumericVector rates(ratesVec); 

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
        } else if (cal == "uk"){
            calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
        }

        //build the bond
        QuantLib::Schedule sch(effectiveDate, maturityDate, QuantLib::Period(freq), calendar,
                               bdc, tbdc, rule, endOfMonth);
        
        QuantLib::FixedRateBond bond(settlementDays, faceAmount, sch,
                                     Rcpp::as<std::vector <double> >(rates), 
                                     dc, bdc, redemption, issueDate);

        //bond price
        boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountCurve));
        bond.setPricingEngine(bondEngine);   
        
        return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                                  Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                                  Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                                  Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                                  Rcpp::Named("yield") = bond.yield(QuantLib::Actual360(), QuantLib::Compounded, QuantLib::Annual),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}
   

RcppExport SEXP FixedRateBondYield(SEXP optionParameters, SEXP ratesVec) {
  
    try {
        Rcpp::List rparam(optionParameters);
        double settlementDays = Rcpp::as<double>(rparam["settlementDays"]);
        std::string cal = Rcpp::as<std::string>(rparam["calendar"]);
        double price = Rcpp::as<double>(rparam["price"]);
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
        double businessDayConvention = Rcpp::as<double>(rparam["businessDayConvention"]);
        double compound = Rcpp::as<double>(rparam["compound"]);
        double redemption = Rcpp::as<double>(rparam["redemption"]);
        double dayCounter = Rcpp::as<double>(rparam["dayCounter"]);
        double frequency = Rcpp::as<double>(rparam["period"]);
        
        QuantLib::Date maturityDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["maturityDate"])));
        QuantLib::Date issueDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["issueDate"])));
        QuantLib::Date effectiveDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["effectiveDate"])));
        
        //extract coupon rates vector
        Rcpp::NumericVector rates(ratesVec); 
        
        //set up BusinessDayConvetion
        QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        QuantLib::DayCounter dc = getDayCounter(dayCounter);
        QuantLib::Frequency freq = getFrequency(frequency);
        QuantLib::Compounding cp = getCompounding(compound);
 
        //set up calendar
        QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
        }
        
        //build the bond
        QuantLib::Schedule sch(effectiveDate, maturityDate, QuantLib::Period(freq), calendar,
                               bdc, bdc, QuantLib::DateGeneration::Backward, false);
        
        QuantLib::FixedRateBond bond(settlementDays, faceAmount, sch,
                                     Rcpp::as<std::vector <double> >(rates), 
                                     dc, bdc, redemption, issueDate);

        return Rcpp::wrap(bond.yield(price, dc, cp, freq));
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

 
RcppExport SEXP FixedRateBondPriceByYield(SEXP optionParameters, SEXP ratesVec) {
  
    try {
        Rcpp::List rparam(optionParameters);
        double settlementDays = Rcpp::as<double>(rparam["settlementDays"]);
        std::string cal = Rcpp::as<std::string>(rparam["calendar"]);
        double yield = Rcpp::as<double>(rparam["yield"]);
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
        double businessDayConvention = Rcpp::as<double>(rparam["businessDayConvention"]);
        double compound = Rcpp::as<double>(rparam["compound"]);
        double redemption = Rcpp::as<double>(rparam["redemption"]);
        double dayCounter = Rcpp::as<double>(rparam["dayCounter"]);
        double frequency = Rcpp::as<double>(rparam["period"]);
        
        QuantLib::Date maturityDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["maturityDate"])));
        QuantLib::Date issueDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["issueDate"])));
        QuantLib::Date effectiveDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["effectiveDate"])));
        
        //extract coupon rates vector
        Rcpp::NumericVector rates(ratesVec); 
        
        //set up BusinessDayConvetion
        QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        QuantLib::DayCounter dc = getDayCounter(dayCounter);
        QuantLib::Frequency freq = getFrequency(frequency);
        QuantLib::Compounding cp = getCompounding(compound);
 
        //set up calendar
        QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
        }
        
        //build the bond
        QuantLib::Schedule sch(effectiveDate, maturityDate, QuantLib::Period(freq), calendar,
                               bdc, bdc, QuantLib::DateGeneration::Backward, false);
        
        QuantLib::FixedRateBond bond(settlementDays, faceAmount, sch,
                                     Rcpp::as<std::vector <double> >(rates), 
                                     dc, bdc, redemption, issueDate);
        
        return Rcpp::wrap(bond.cleanPrice(yield, dc, cp, freq));
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }
    
    return R_NilValue;
}


SEXP FloatingBond(SEXP bondparam, SEXP gearingsVec, SEXP spreadsVec,
                  SEXP capsVec, SEXP floorsVec, 
                  QuantLib::Handle<QuantLib::YieldTermStructure> &index,
                  SEXP indexparams,
                  QuantLib::Handle<QuantLib::YieldTermStructure> &discountCurve,
                  SEXP dateparams) 
{
  
    try {
        Rcpp::List rparam(bondparam);        
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
        QuantLib::Date maturityDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["maturityDate"])));
        QuantLib::Date issueDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["issueDate"])));
        QuantLib::Date effectiveDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["effectiveDate"])));
        double redemption = Rcpp::as<double>(rparam["redemption"]);

        Rcpp::List misc(dateparams);      
        double settlementDays = Rcpp::as<double>(misc["settlementDays"]);
        std::string cal = Rcpp::as<std::string>(misc["calendar"]);
        double dayCounter = Rcpp::as<double>(misc["dayCounter"]);
        double frequency = Rcpp::as<double>(misc["period"]);
        double businessDayConvention = Rcpp::as<double>(misc["businessDayConvention"]);
        double terminationDateConvention = Rcpp::as<double>(misc["terminationDateConvention"]);
        double dateGeneration = Rcpp::as<double>(misc["dateGeneration"]);
        double endOfMonthRule = Rcpp::as<double>(misc["endOfMonth"]);
        double fixingDays = Rcpp::as<double>(misc["fixingDays"]);

        //build schedule
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
        } else if (cal == "uk"){
            calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
        }
        QuantLib::Schedule sch(effectiveDate, maturityDate, QuantLib::Period(freq), calendar,
                               bdc, tbdc, rule, endOfMonth);

        //extract gearings, spreads, caps, and floors
        std::vector<double> gearings = getDoubleVector(gearingsVec);
        std::vector<double> spreads = getDoubleVector(spreadsVec);
        std::vector<double> caps = getDoubleVector(capsVec);
        std::vector<double> floors = getDoubleVector(floorsVec);

        Rcpp::List iborparams(indexparams);      
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
        //build the bond
        QuantLib::FloatingRateBond bond(settlementDays, faceAmount, sch, iborindex, dc, bdc, fixingDays,
                                        gearings, spreads, caps, floors, false, redemption, issueDate);        
        
        
        //bond price
        boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountCurve));
        bond.setPricingEngine(bondEngine);

        
        //cashflow
        boost::shared_ptr<QuantLib::IborCouponPricer> pricer(new QuantLib::BlackIborCouponPricer(QuantLib::Handle<QuantLib::OptionletVolatilityStructure>()));
        setCouponPricer(bond.cashflows(),pricer);

        return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                                  Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                                  Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                                  Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                                  Rcpp::Named("yield") = bond.yield(QuantLib::Actual360(), QuantLib::Compounded, QuantLib::Annual),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP FloatBond1(SEXP bond, SEXP gearings, SEXP caps, SEXP spreads,
                           SEXP floors, SEXP indexparams, SEXP index, 
                           SEXP discountCurve, SEXP dateparams)
{
    
    try{

        QuantLib::Handle<QuantLib::YieldTermStructure> discount_curve(getFlatCurve(discountCurve));
        QuantLib::Handle<QuantLib::YieldTermStructure> ibor_curve(getFlatCurve(index));
        return Rcpp::wrap(FloatingBond(bond, gearings, caps, spreads,
                                       floors, ibor_curve, indexparams,
                                       discount_curve, dateparams));       
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP FloatBond2(SEXP bond, SEXP gearings, SEXP caps, SEXP spreads,
                           SEXP floors, SEXP indexparams, SEXP index_params, 
                           SEXP index_tsQuotes, SEXP index_times,
                           SEXP discountCurve, SEXP dateparams)
{
    
    try{

        QuantLib::Handle<QuantLib::YieldTermStructure> discount_curve(getFlatCurve(discountCurve));
        QuantLib::Handle<QuantLib::YieldTermStructure> ibor_curve(buildTermStructure(index_params,
                                                                 index_tsQuotes,
                                                                 index_times));
        return Rcpp::wrap(FloatingBond(bond, gearings, caps, spreads,
                                       floors, ibor_curve, indexparams,
                                       discount_curve, dateparams));       
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP FloatBond3(SEXP bond, SEXP gearings, SEXP caps,
                           SEXP spreads, SEXP floors, 
                           SEXP indexparams, SEXP index, 
                           SEXP discount_params, SEXP discount_tsQuotes,
                           SEXP discount_times, SEXP dateparams)
{
    
    try {

        QuantLib::Handle<QuantLib::YieldTermStructure> ibor_curve(getFlatCurve(index));
        QuantLib::Handle<QuantLib::YieldTermStructure> discount_curve(buildTermStructure(discount_params,
                                                                     discount_tsQuotes,
                                                                     discount_times));
        return Rcpp::wrap(FloatingBond(bond, gearings, caps, spreads,
                                       floors, ibor_curve, indexparams,
                                       discount_curve, dateparams));       
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP FloatBond4(SEXP bond, SEXP gearings, SEXP caps,
                           SEXP spreads, SEXP floors, 
                           SEXP indexparams, SEXP index_params, 
                           SEXP index_tsQuotes, SEXP index_times,
                           SEXP discount_params, SEXP discount_tsQuotes,
                           SEXP discount_times, SEXP dateparams)
{
    
    try {

        QuantLib::Handle<QuantLib::YieldTermStructure> ibor_curve(buildTermStructure(index_params,
                                                                 index_tsQuotes,
                                                                 index_times));
      
        QuantLib::Handle<QuantLib::YieldTermStructure> discount_curve(buildTermStructure(discount_params,
                                                                 discount_tsQuotes,
                                                                 discount_times));
        return Rcpp::wrap(FloatingBond(bond, gearings, caps, spreads,
                                       floors, ibor_curve, indexparams,
                                       discount_curve, dateparams));       
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }
    
    return R_NilValue;
}

RcppExport SEXP FloatingWithRebuiltCurve(SEXP bondparams, SEXP gearings,
                                         SEXP spreads, SEXP caps,
                                         SEXP floors, SEXP indexparams,
                                         SEXP iborDateSexp, SEXP iborzeroSexp,
                                         SEXP dateSexp, SEXP zeroSexp,
                                         SEXP dateparams) {

    try {
        QuantLib::Handle<QuantLib::YieldTermStructure> ibor_curve(rebuildCurveFromZeroRates(iborDateSexp, iborzeroSexp));       
        QuantLib::Handle<QuantLib::YieldTermStructure> curve(rebuildCurveFromZeroRates(dateSexp, zeroSexp));       
        
        SEXP flrtbond = FloatingBond(bondparams, gearings, caps, spreads,
                                     floors, ibor_curve, indexparams,
                                     curve, dateparams);
        return flrtbond;

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP FixedRateWithRebuiltCurve(SEXP bondparam, SEXP ratesVec,
                                          SEXP dateSexp, SEXP zeroSexp,
                                          SEXP dateparams){
    try {
        QuantLib::Handle<QuantLib::YieldTermStructure> curve(rebuildCurveFromZeroRates(dateSexp, zeroSexp));
        return Rcpp::wrap(FixedBond(bondparam, ratesVec, curve, dateparams));
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP ZeroBondWithRebuiltCurve(SEXP bond,
                                         SEXP dateSexp, SEXP zeroSexp,
                                         SEXP dateparams){
    try {

        QuantLib::Handle<QuantLib::YieldTermStructure> curve(rebuildCurveFromZeroRates(dateSexp, zeroSexp));
        return Rcpp::wrap(ZeroBond(bond, curve, dateparams));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP ConvertibleZeroBond(SEXP bondparams, SEXP process,
                                    SEXP dividendYieldDateSexp,
                                    SEXP dividendYieldZeroSexp,
                                    SEXP rffDateSexp, SEXP rffZeroSexp,
                                    SEXP dividendScheduleFrame,
                                    SEXP callabilityScheduleFrame,
                                    SEXP dateparams) 
{

    try {

        QuantLib::DividendSchedule dividendSchedule = getDividendSchedule(dividendScheduleFrame);
        QuantLib::CallabilitySchedule callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);

        Rcpp::List rparam(bondparams);
        
        //double faceAmount = Rcpp::as<double>(rparam["faceAmount");        
        QuantLib::Date maturityDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["maturityDate"])));
        QuantLib::Date issueDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["issueDate"])));
        double redemption = Rcpp::as<double>(rparam["redemption"]);
        std::string exercise = Rcpp::as<std::string>(rparam["exercise"]);
        double creditSpreadQuote = Rcpp::as<double>(rparam["creditSpread"]);
        double conversionRatio = Rcpp::as<double>(rparam["conversionRatio"]);

                                                  
        Rcpp::List misc(dateparams);      
        double settlementDays = Rcpp::as<double>(misc["settlementDays"]);
        std::string cal = Rcpp::as<std::string>(misc["calendar"]);
        double dayCounter = Rcpp::as<double>(misc["dayCounter"]);
        double frequency = Rcpp::as<double>(misc["period"]);
        double businessDayConvention = Rcpp::as<double>(misc["businessDayConvention"]);
        
        QuantLib::Date todayDate = issueDate;
        
        QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        if (cal == "us") {
            calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
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

        Rcpp::List processParam(process);
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
            discountCurve(boost::shared_ptr<QuantLib::YieldTermStructure>(new QuantLib::ForwardSpreadedTermStructure(rff,
                                                                                                 creditSpread)));
        
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
                                  Rcpp::Named("yield") = bond.yield(QuantLib::Actual360(), QuantLib::Compounded, QuantLib::Annual),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP ConvertibleFixedBond(SEXP bondparams, SEXP coupon, SEXP process,
                                     SEXP dividendYieldDateSexp,
                                     SEXP dividendYieldZeroSexp,
                                     SEXP rffDateSexp, SEXP rffZeroSexp,
                                     SEXP dividendScheduleFrame,
                                     SEXP callabilityScheduleFrame,
                                     SEXP dateparams) {

    try {

        QuantLib::DividendSchedule dividendSchedule = getDividendSchedule(dividendScheduleFrame);
        QuantLib::CallabilitySchedule callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);

        Rcpp::List rparam(bondparams);
        //double faceAmount = Rcpp::as<double>(rparam["faceAmount");
        QuantLib::Date maturityDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["maturityDate"])));
        QuantLib::Date issueDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["issueDate"])));
        double redemption = Rcpp::as<double>(rparam["redemption"]);
        std::string exercise = Rcpp::as<std::string>(rparam["exercise"]);
        double creditSpreadQuote = Rcpp::as<double>(rparam["creditSpread"]);
        double conversionRatio = Rcpp::as<double>(rparam["conversionRatio"]);

        //extract coupon rates vector
        Rcpp::NumericVector rates(coupon); 

        Rcpp::List misc(dateparams);      
        double settlementDays = Rcpp::as<double>(misc["settlementDays"]);
        std::string cal = Rcpp::as<std::string>(misc["calendar"]);
        double dayCounter = Rcpp::as<double>(misc["dayCounter"]);
        double frequency = Rcpp::as<double>(misc["period"]);
        double businessDayConvention = Rcpp::as<double>(misc["businessDayConvention"]);
       
        QuantLib::Date todayDate = issueDate;
        QuantLib::Settings::instance().evaluationDate() = todayDate;
        QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
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

        Rcpp::List processParam(process);
        double underlyingQuote = Rcpp::as<double>(processParam["underlying"]);
        double volatilityQuote = Rcpp::as<double>(processParam["volatility"]);
        underlying.linkTo(boost::shared_ptr<QuantLib::Quote>(new QuantLib::SimpleQuote(underlyingQuote)));
        boost::shared_ptr<QuantLib::SimpleQuote> vol(new QuantLib::SimpleQuote( volatilityQuote ));
        volatility.linkTo(flatVol(todayDate, vol, dc));

        boost::shared_ptr<QuantLib::BlackScholesMertonProcess> blackProcess;
        blackProcess = 
            boost::shared_ptr<QuantLib::BlackScholesMertonProcess>(new QuantLib::BlackScholesMertonProcess(underlying, dividendYield,
                                                                                       rff, volatility));
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
                                  Rcpp::Named("yield") = bond.yield(QuantLib::Actual360(), QuantLib::Compounded, QuantLib::Annual),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP ConvertibleFloatingBond(SEXP bondparams,  SEXP process,
                                        SEXP dividendYieldDateSexp,
                                        SEXP dividendYieldZeroSexp,
                                        SEXP rffDateSexp, SEXP rffZeroSexp,
                                        SEXP iborIndexDateSexp, SEXP iborIndexZeroSexp,
                                        SEXP indexparams, SEXP spreadVec,
                                        SEXP dividendScheduleFrame,
                                        SEXP callabilityScheduleFrame,
                                        SEXP dateparams) {

    try {

        QuantLib::DividendSchedule dividendSchedule = getDividendSchedule(dividendScheduleFrame);
        QuantLib::CallabilitySchedule callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);

        Rcpp::List rparam(bondparams);
        
        //double faceAmount = Rcpp::as<double>(rparam["faceAmount");        
        QuantLib::Date maturityDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["maturityDate"])));
        QuantLib::Date issueDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["issueDate"])));
        double redemption = Rcpp::as<double>(rparam["redemption"]);
        std::string exercise = Rcpp::as<std::string>(rparam["exercise"]);
        double creditSpreadQuote = Rcpp::as<double>(rparam["creditSpread"]);
        double conversionRatio = Rcpp::as<double>(rparam["conversionRatio"]);

        std::vector<double> spreads = getDoubleVector(spreadVec);

        //extract iborindex curve
        QuantLib::Handle<QuantLib::YieldTermStructure> index(rebuildCurveFromZeroRates(iborIndexDateSexp, iborIndexZeroSexp));

        Rcpp::List iborparams(indexparams);      
        std::string type = Rcpp::as<std::string>(iborparams["type"]);
        double length = Rcpp::as<double>(iborparams["length"]);
        std::string inTermOf = Rcpp::as<std::string>(iborparams["inTermOf"]);

        boost::shared_ptr<QuantLib::IborIndex> iborindex(new QuantLib::USDLibor(6 * QuantLib::Months, index));
        if (type=="USDLibor"){
            if (inTermOf=="Months"){               
                boost::shared_ptr<QuantLib::IborIndex> temp(new QuantLib::USDLibor(length * QuantLib::Months, index));
                iborindex = temp;
            } else if (inTermOf=="Years") {
                boost::shared_ptr<QuantLib::IborIndex> temp(new QuantLib::USDLibor(length * QuantLib::Years, index));
                iborindex = temp;
            }
        }

        Rcpp::List misc(dateparams);      
        double settlementDays = Rcpp::as<double>(misc["settlementDays"]);
        std::string cal = Rcpp::as<std::string>(misc["calendar"]);
        double dayCounter = Rcpp::as<double>(misc["dayCounter"]);
        double frequency = Rcpp::as<double>(misc["period"]);
        double businessDayConvention = Rcpp::as<double>(misc["businessDayConvention"]);

        QuantLib::Date todayDate = issueDate;
        
        QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
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

        Rcpp::List processParam(process);
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
        boost::shared_ptr<QuantLib::PricingEngine> engine(new QuantLib::BinomialConvertibleEngine<QuantLib::CoxRossRubinstein>(blackProcess,
                                                                                                 timeSteps));
        
        QuantLib::Handle<QuantLib::YieldTermStructure> 
            discountCurve(boost::shared_ptr<QuantLib::YieldTermStructure>(new QuantLib::ForwardSpreadedTermStructure(rff,
                                                                                                 creditSpread)));
        QuantLib::Natural fixingDays = 2;
        QuantLib::Schedule sch(issueDate, maturityDate,
                               QuantLib::Period(freq), calendar,
                               bdc, bdc,
                               QuantLib::DateGeneration::Backward, false);        
        QuantLib::ConvertibleFloatingRateBond bond(ex, conversionRatio,
                                                   dividendSchedule, callabilitySchedule,
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
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP CallableBond(SEXP bondparams, SEXP hw, SEXP coupon,
                             SEXP callabilityScheduleFrame,
                             SEXP dateparams) {

    try {

        QuantLib::CallabilitySchedule callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);

        Rcpp::List rparam(bondparams);
        
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);        
        QuantLib::Date maturityDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["maturityDate"])));
        QuantLib::Date issueDate(dateFromR(Rcpp::as<Rcpp::Date>(rparam["issueDate"])));
        double redemption = Rcpp::as<double>(rparam["redemption"]);

        Rcpp::List misc(dateparams);      
        double settlementDays = Rcpp::as<double>(misc["settlementDays"]);

        std::string cal = Rcpp::as<std::string>(misc["calendar"]);
        double dayCounter = Rcpp::as<double>(misc["dayCounter"]);
        double frequency = Rcpp::as<double>(misc["period"]);
        double businessDayConvention = Rcpp::as<double>(misc["businessDayConvention"]);
        
        QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
        }

        QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        QuantLib::DayCounter dc = getDayCounter(dayCounter);
        QuantLib::Frequency freq = getFrequency(frequency);
        
        //extract coupon rates vector
        Rcpp::NumericVector rates(coupon); 
        
        Rcpp::List hwparam(hw);
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

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

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

RcppExport SEXP FittedBondCurve(SEXP curveparams, SEXP lengthVec,
                                SEXP couponVec,SEXP marketVec,
                                SEXP dateparams){
    try {

        //extract length, coupon and market prices vector
        Rcpp::NumericVector length(lengthVec); 
        Rcpp::NumericVector coupons(couponVec); 
        Rcpp::NumericVector marketQuotes(marketVec); 

        Rcpp::List misc(dateparams);      
        double settlementDays = Rcpp::as<double>(misc["settlementDays"]);
        double dayCounter = Rcpp::as<double>(misc["dayCounter"]);
        double frequency = Rcpp::as<double>(misc["period"]);
        double businessDayConvention = Rcpp::as<double>(misc["businessDayConvention"]);
    
        Rcpp::List curvepam(curveparams);
        std::string method = Rcpp::as<std::string>(curvepam["method"]);
        Rcpp::Date oDate = Rcpp::as<Rcpp::Date>(curvepam["origDate"]);
        QuantLib::Date origDate(dateFromR(oDate));
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

        std::vector<boost::shared_ptr<QuantLib::FixedRateBondHelper> > instrumentsA;
        
        for (QuantLib::Size j=0; j < static_cast<QuantLib::Size>(length.size()); j++) {

            QuantLib::Date dated = origDate;
            QuantLib::Date issue = origDate;
            QuantLib::Date maturity = calendar.advance(issue, length[j], QuantLib::Years);
            
            QuantLib::Schedule schedule(dated, maturity, QuantLib::Period(freq), calendar,
                                        bdc, bdc,
                                        QuantLib::DateGeneration::Backward, false);

            boost::shared_ptr<QuantLib::FixedRateBondHelper> helperA(
                     new QuantLib::FixedRateBondHelper(quoteHandle[j], settlementDays, 100.0, schedule,
                                                       std::vector<QuantLib::Rate>(1,coupons[j]),
                                                       dc, bdc, redemption, issue));
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

        } else if (method == "SimplePolynomialFitting"){
            double degree = Rcpp::as<double>(curvepam["degree"]);
            QuantLib::SimplePolynomialFitting simplePolynomial(degree, constrainAtZero);

            boost::shared_ptr<QuantLib::FittedBondDiscountCurve> 
                ts2 (new QuantLib::FittedBondDiscountCurve(settlementDays, calendar, instrumentsA, dc,
                                                           simplePolynomial, tolerance, max));
            curve = ts2;

        } else if (method == "NelsonSiegelFitting"){
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

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}
