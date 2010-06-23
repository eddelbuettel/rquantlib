// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2002 - 2009 Dirk Eddelbuettel 
// Copyright (C) 2009 - 2010 Khanh Nguyen and Dirk Eddelbuettel
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

#include <rquantlib.hpp>

using namespace boost;


RcppExport SEXP QL_ZeroPriceByYield(SEXP optionParameters) {

    try {
        Rcpp::List rparam(optionParameters);
        double yield = Rcpp::as<double>(rparam["yield"]);
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
        double dayCounter = Rcpp::as<double>(rparam["dayCounter"]);
        double frequency = Rcpp::as<double>(rparam["frequency"]);
        double businessDayConvention = Rcpp::as<double>(rparam["businessDayConvention"]);
        double compound = Rcpp::as<double>(rparam["compound"]);
        Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
        Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        //setup bond
        QuantLib::Integer fixingDays = 2;
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        Date todaysDate = calendar.advance(issueDate, -fixingDays, Days);
        Settings::instance().evaluationDate() = todaysDate;
        Natural settlementDays = 1;
       
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        double redemption = 100;
        ZeroCouponBond zbond(settlementDays, calendar,
                             faceAmount, maturityDate,
                             bdc, redemption, issueDate);
       
        //return cleanPrice
        DayCounter dc = getDayCounter(dayCounter);
        Compounding cp = getCompounding(compound);
        Frequency freq = getFrequency(frequency);
        return Rcpp::wrap(zbond.cleanPrice(yield, dc, cp, freq));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP QL_ZeroYield(SEXP optionParameters) {

    try {
        Rcpp::List rparam(optionParameters);
        double price = Rcpp::as<double>(rparam["price"]);
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
        double dayCounter = Rcpp::as<double>(rparam["dayCounter"]);
        double frequency = Rcpp::as<double>(rparam["frequency"]);
        double businessDayConvention = Rcpp::as<double>(rparam["businessDayConvention"]);
        double compound = Rcpp::as<double>(rparam["compound"]);
        Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
        Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        //setup bond
        QuantLib::Integer fixingDays = 2;
        Calendar calendar=UnitedStates(UnitedStates::GovernmentBond);
        Date todaysDate = calendar.advance(issueDate, -fixingDays, Days);
        Settings::instance().evaluationDate() = todaysDate;
        Natural settlementDays = 1;
       
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        double redemption = 100;
        ZeroCouponBond zbond(settlementDays, calendar,
                             faceAmount, maturityDate,
                             bdc, redemption, issueDate);
       
        //return yield
        DayCounter dc = getDayCounter(dayCounter);
        Compounding cp = getCompounding(compound);
        Frequency freq = getFrequency(frequency);
        return Rcpp::wrap(zbond.yield(price, dc, cp, freq));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }
    
    return R_NilValue;
}


RcppExport SEXP QL_ZeroBond(SEXP bondparam, 
                            Handle<YieldTermStructure> &discountCurve,
                            SEXP dateparams) {

    try {
        Rcpp::List rparam(bondparam);
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
        Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
        Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        double redemption = Rcpp::as<double>(rparam["redemption"]);

        Rcpp::List misc(dateparams);
        double settlementDays = Rcpp::as<double>(misc["settlementDays"]);
        std::string cal = Rcpp::as<std::string>(misc["calendar"]);
        double businessDayConvention = Rcpp::as<double>(misc["businessDayConvention"]);
        QuantLib::Date refDate(dateFromR(Rcpp::Date(Rcpp::as<int>(misc["refDate"]))));      
        Settings::instance().evaluationDate() = refDate;                               
        
        /*
          test-suite/bonds.cpp
        */      

        //set up BusinessDayConvetion
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        
        // set up calendar -- FIXME: use utils function getCalendar instead
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
        
        ZeroCouponBond bond(settlementDays,
                            calendar,
                            faceAmount,
                            maturityDate,
                            bdc,
                            redemption, issueDate);
        
        boost::shared_ptr<PricingEngine> bondEngine(new DiscountingBondEngine(discountCurve));
        bond.setPricingEngine(bondEngine);

        return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                                  Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                                  Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                                  Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                                  Rcpp::Named("yield") = bond.yield(Actual360(), Compounded, Annual),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP QL_FixedBond(SEXP bondparam, SEXP ratesVec,
                             Handle<YieldTermStructure> &discountCurve,
                             SEXP dateparams){

    try {
        Rcpp::List rparam(bondparam);
        
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);
        Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
        Rcpp::Date eDate = Rcpp::Date(Rcpp::as<int>(rparam["effectiveDate"]));
        Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date effectiveDate(dateFromR(eDate));
        QuantLib::Date issueDate(dateFromR(iDate));
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
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        BusinessDayConvention tbdc = getBusinessDayConvention(terminationDateConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        DateGeneration::Rule rule = getDateGenerationRule(dateGeneration);
        bool endOfMonth = (endOfMonthRule==1) ? true : false;
        //set up calendar
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us") {
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }

        //build the bond
        Schedule sch(effectiveDate, maturityDate, Period(freq), calendar,
                     bdc, tbdc, rule, endOfMonth);
        
        FixedRateBond bond(settlementDays, faceAmount, sch,
                           Rcpp::as<std::vector <double> >(rates), 
                           dc, bdc, redemption, issueDate);

        //bond price
        boost::shared_ptr<PricingEngine> bondEngine(new DiscountingBondEngine(discountCurve));
        bond.setPricingEngine(bondEngine);   
        
        return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                                  Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                                  Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                                  Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                                  Rcpp::Named("yield") = bond.yield(Actual360(), Compounded, Annual),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}
   

RcppExport  SEXP QL_FixedRateBondYield(SEXP optionParameters, SEXP ratesVec) {
  
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
        
        Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
        Rcpp::Date eDate = Rcpp::Date(Rcpp::as<int>(rparam["effectiveDate"]));
        Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date effectiveDate(dateFromR(eDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        
        //extract coupon rates vector
        Rcpp::NumericVector rates(ratesVec); 
        
        //set up BusinessDayConvetion
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        Compounding cp = getCompounding(compound);
 
        //set up calendar
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
        
        //build the bond
        Schedule sch(effectiveDate, maturityDate, Period(freq), calendar,
                     bdc, bdc, DateGeneration::Backward, false);
        
        FixedRateBond bond(settlementDays, faceAmount, sch,
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

 
RcppExport SEXP QL_FixedRateBondPriceByYield(SEXP optionParameters, SEXP ratesVec) {
  
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
        
        Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
        Rcpp::Date eDate = Rcpp::Date(Rcpp::as<int>(rparam["effectiveDate"]));
        Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date effectiveDate(dateFromR(eDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        
        //extract coupon rates vector
        Rcpp::NumericVector rates(ratesVec); 
        
        //set up BusinessDayConvetion
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        Compounding cp = getCompounding(compound);
 
        //set up calendar
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
        
        //build the bond
        Schedule sch(effectiveDate, maturityDate, Period(freq), calendar,
                     bdc, bdc, DateGeneration::Backward, false);
        
        FixedRateBond bond(settlementDays, faceAmount, sch,
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


SEXP QL_FloatingBond(SEXP bondparam, SEXP gearingsVec, SEXP spreadsVec,
                     SEXP capsVec, SEXP floorsVec, 
                     Handle<YieldTermStructure> &index,
                     SEXP indexparams,
                     Handle<YieldTermStructure> &discountCurve,
                     SEXP dateparams) 
{
  
    try {
        Rcpp::List rparam(bondparam);        
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);     
        Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
        Rcpp::Date eDate = Rcpp::Date(Rcpp::as<int>(rparam["effectiveDate"]));
        Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date effectiveDate(dateFromR(eDate));
        QuantLib::Date issueDate(dateFromR(iDate));
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
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        BusinessDayConvention tbdc = getBusinessDayConvention(terminationDateConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        DateGeneration::Rule rule = getDateGenerationRule(dateGeneration);
        bool endOfMonth = (endOfMonthRule==1) ? true : false;

        //set up calendar
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us") {
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
        Schedule sch(effectiveDate, maturityDate, Period(freq), calendar,
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

        boost::shared_ptr<IborIndex> iborindex(new USDLibor(6 * Months, index));
        if (type=="USDLibor") {
            if (inTermOf=="Months") {               
                boost::shared_ptr<IborIndex> temp(new USDLibor(length * Months, index));
                iborindex = temp;
            } else if (inTermOf=="Years") {
                boost::shared_ptr<IborIndex> temp(new USDLibor(length * Years, index));
                iborindex = temp;
            }
        }
        //build the bond
        FloatingRateBond bond(settlementDays, faceAmount, sch, iborindex, dc, bdc, fixingDays,
                              gearings, spreads, caps, floors, false, redemption, issueDate);        
        
        
        //bond price
        boost::shared_ptr<PricingEngine> bondEngine(new DiscountingBondEngine(discountCurve));
        bond.setPricingEngine(bondEngine);

        
        //cashflow
        boost::shared_ptr<IborCouponPricer> pricer(new BlackIborCouponPricer(Handle<OptionletVolatilityStructure>()));
        setCouponPricer(bond.cashflows(),pricer);

        return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                                  Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                                  Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                                  Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                                  Rcpp::Named("yield") = bond.yield(Actual360(), Compounded, Annual),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP QL_FloatBond1(SEXP bond, SEXP gearings, SEXP caps,
                              SEXP spreads,
                              SEXP floors, SEXP indexparams, SEXP index, 
                              SEXP discountCurve, SEXP dateparams)
{
    
    try{

        Handle<YieldTermStructure> discount_curve(getFlatCurve(discountCurve));
        Handle<YieldTermStructure> ibor_curve(getFlatCurve(index));
        return Rcpp::wrap(QL_FloatingBond(bond, gearings, caps, spreads,
                                          floors, ibor_curve, indexparams,
                                          discount_curve, dateparams));       
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP QL_FloatBond2(SEXP bond, SEXP gearings, SEXP caps,
                              SEXP spreads,
                              SEXP floors, SEXP indexparams, SEXP index_params, 
                              SEXP index_tsQuotes, SEXP index_times,
                              SEXP discountCurve, SEXP dateparams)
{
    
    try{

        Handle<YieldTermStructure> discount_curve(getFlatCurve(discountCurve));
        Handle<YieldTermStructure> ibor_curve(buildTermStructure(index_params,
                                                                 index_tsQuotes,
                                                                 index_times));
        return Rcpp::wrap(QL_FloatingBond(bond, gearings, caps, spreads,
                                          floors, ibor_curve, indexparams,
                                          discount_curve, dateparams));       
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP QL_FloatBond3(SEXP bond, SEXP gearings, SEXP caps,
                              SEXP spreads, SEXP floors, 
                              SEXP indexparams, SEXP index, 
                              SEXP discount_params, SEXP discount_tsQuotes,
                              SEXP discount_times, SEXP dateparams)
{
    
    try {

        Handle<YieldTermStructure> ibor_curve(getFlatCurve(index));
        Handle<YieldTermStructure> discount_curve(buildTermStructure(discount_params,
                                                                     discount_tsQuotes,
                                                                     discount_times));
        return Rcpp::wrap(QL_FloatingBond(bond, gearings, caps, spreads,
                                          floors, ibor_curve, indexparams,
                                          discount_curve, dateparams));       
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP QL_FloatBond4(SEXP bond, SEXP gearings, SEXP caps,
                              SEXP spreads, SEXP floors, 
                              SEXP indexparams, SEXP index_params, 
                              SEXP index_tsQuotes, SEXP index_times,
                              SEXP discount_params, SEXP discount_tsQuotes,
                              SEXP discount_times, SEXP dateparams)
{
    
    try {

        Handle<YieldTermStructure> ibor_curve(buildTermStructure(index_params,
                                                                 index_tsQuotes,
                                                                 index_times));
      
        Handle<YieldTermStructure> discount_curve(buildTermStructure(discount_params,
                                                                 discount_tsQuotes,
                                                                 discount_times));
        return Rcpp::wrap(QL_FloatingBond(bond, gearings, caps, spreads,
                                          floors, ibor_curve, indexparams,
                                          discount_curve, dateparams));       
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }
    
    return R_NilValue;
}

RcppExport SEXP QL_FloatingWithRebuiltCurve(SEXP bondparams, SEXP gearings,
                                            SEXP spreads, SEXP caps,
                                            SEXP floors, SEXP indexparams,
                                            SEXP iborDateSexp, SEXP iborzeroSexp,
                                            SEXP dateSexp, SEXP zeroSexp,
                                            SEXP dateparams) {

    try {
        Handle<YieldTermStructure> ibor_curve(rebuildCurveFromZeroRates(iborDateSexp, iborzeroSexp));       
        Handle<YieldTermStructure> curve(rebuildCurveFromZeroRates(dateSexp, zeroSexp));       
        
        SEXP flrtbond = QL_FloatingBond(bondparams, gearings, caps, spreads,
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


RcppExport SEXP QL_FixedRateWithRebuiltCurve(SEXP bondparam, SEXP ratesVec,
                                             SEXP dateSexp, SEXP zeroSexp,
                                             SEXP dateparams){
    try {
        Handle<YieldTermStructure> curve(rebuildCurveFromZeroRates(dateSexp, zeroSexp));
        return Rcpp::wrap(QL_FixedBond(bondparam, ratesVec, curve, dateparams));
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP QL_ZeroBondWithRebuiltCurve(SEXP bond,
                                            SEXP dateSexp, SEXP zeroSexp,
                                            SEXP dateparams){
    try {

        Handle<YieldTermStructure> curve(rebuildCurveFromZeroRates(dateSexp, zeroSexp));
        return Rcpp::wrap(QL_ZeroBond(bond, curve, dateparams));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP QL_ConvertibleZeroBond(SEXP bondparams, SEXP process,
                                       SEXP dividendYieldDateSexp,
                                       SEXP dividendYieldZeroSexp,
                                       SEXP rffDateSexp, SEXP rffZeroSexp,
                                       SEXP dividendScheduleFrame,
                                       SEXP callabilityScheduleFrame,
                                       SEXP dateparams) 
{

    try {

        DividendSchedule dividendSchedule = getDividendSchedule(dividendScheduleFrame);
        CallabilitySchedule callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);

        Rcpp::List rparam(bondparams);
        
        //double faceAmount = Rcpp::as<double>(rparam["faceAmount");        
        Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
        Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
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
        
        QuantLib::Date todayDate(dateFromR(iDate));
        
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us") {
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
 
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);

        RelinkableHandle<Quote> underlying;
        RelinkableHandle<BlackVolTermStructure> volatility;
        boost::shared_ptr<BlackScholesMertonProcess> blackProcess;

        Handle<YieldTermStructure> dividendYield(rebuildCurveFromZeroRates(dividendYieldDateSexp,
                                                                           dividendYieldZeroSexp));

        Handle<YieldTermStructure> rff(rebuildCurveFromZeroRates(rffDateSexp,
                                                                 rffZeroSexp));

        Rcpp::List processParam(process);
        double underlyingQuote = Rcpp::as<double>(processParam["underlying"]);
        double volatilityQuote = Rcpp::as<double>(processParam["volatility"]);
        underlying.linkTo(boost::shared_ptr<Quote>(new SimpleQuote(underlyingQuote)));
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote( volatilityQuote ));
        volatility.linkTo(flatVol(todayDate, vol, dc));

        blackProcess = 
            boost::shared_ptr<BlackScholesMertonProcess>(new BlackScholesMertonProcess(underlying, dividendYield,
                                                                                       rff, volatility));

        RelinkableHandle<Quote> creditSpread;
        creditSpread.linkTo(boost::shared_ptr<Quote>(new SimpleQuote(creditSpreadQuote)));

        boost::shared_ptr<Exercise> euExercise(new EuropeanExercise(maturityDate));
        boost::shared_ptr<Exercise> amExercise(new AmericanExercise(issueDate, maturityDate));
        boost::shared_ptr<Exercise> ex = (exercise == "eu") ? euExercise : amExercise;
        
        Size timeSteps = 1001;
        boost::shared_ptr<PricingEngine> 
            engine(new BinomialConvertibleEngine<CoxRossRubinstein>(blackProcess, timeSteps));
        
        Handle<YieldTermStructure> 
            discountCurve(boost::shared_ptr<YieldTermStructure>(new ForwardSpreadedTermStructure(rff,
                                                                                                 creditSpread)));
        
        Schedule sch(issueDate, maturityDate, Period(freq), calendar,
                     bdc, bdc, DateGeneration::Backward, false);        
        ConvertibleZeroCouponBond bond(ex, conversionRatio,
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
                                  Rcpp::Named("yield") = bond.yield(Actual360(), Compounded, Annual),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP QL_ConvertibleFixedBond(SEXP bondparams, SEXP coupon, SEXP process,
                                        SEXP dividendYieldDateSexp,
                                        SEXP dividendYieldZeroSexp,
                                        SEXP rffDateSexp, SEXP rffZeroSexp,
                                        SEXP dividendScheduleFrame,
                                        SEXP callabilityScheduleFrame,
                                        SEXP dateparams) {

    try {

        DividendSchedule dividendSchedule = getDividendSchedule(dividendScheduleFrame);
        CallabilitySchedule callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);

        Rcpp::List rparam(bondparams);
        //double faceAmount = Rcpp::as<double>(rparam["faceAmount");        
        Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
        Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
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
       
        QuantLib::Date todayDate(dateFromR(iDate));
        Settings::instance().evaluationDate() = todayDate;
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
 
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);

        RelinkableHandle<Quote> underlying;
        RelinkableHandle<BlackVolTermStructure> volatility;
        
        Handle<YieldTermStructure> 
            dividendYield(rebuildCurveFromZeroRates(dividendYieldDateSexp, dividendYieldZeroSexp));

        Handle<YieldTermStructure> rff(rebuildCurveFromZeroRates(rffDateSexp, rffZeroSexp));

        Rcpp::List processParam(process);
        double underlyingQuote = Rcpp::as<double>(processParam["underlying"]);
        double volatilityQuote = Rcpp::as<double>(processParam["volatility"]);
        underlying.linkTo(boost::shared_ptr<Quote>(new SimpleQuote(underlyingQuote)));
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote( volatilityQuote ));
        volatility.linkTo(flatVol(todayDate, vol, dc));

        boost::shared_ptr<BlackScholesMertonProcess> blackProcess;
        blackProcess = 
            boost::shared_ptr<BlackScholesMertonProcess>(new BlackScholesMertonProcess(underlying, dividendYield,
                                                                                       rff, volatility));
        //	boost::shared_ptr<BlackScholesProcess> blackProcess;
        //ackProcess = boost::shared_ptr<BlackScholesProcess>(
        //					      new BlackScholesProcess(underlying, 
        //								      rff, volatility));

        RelinkableHandle<Quote> creditSpread;
        creditSpread.linkTo(boost::shared_ptr<Quote>(new SimpleQuote(creditSpreadQuote)));

        boost::shared_ptr<Exercise> euExercise(new EuropeanExercise(maturityDate));
        boost::shared_ptr<Exercise> amExercise(new AmericanExercise(issueDate, maturityDate));
        
        boost::shared_ptr<Exercise> ex = (exercise == "eu") ? euExercise : amExercise;
        
        Size timeSteps = 1001;
        boost::shared_ptr<PricingEngine> 
            engine(new BinomialConvertibleEngine<CoxRossRubinstein>(blackProcess, timeSteps));
        
        Handle<YieldTermStructure> 
            discountCurve(boost::shared_ptr<YieldTermStructure>(new ForwardSpreadedTermStructure(rff,
                                                                                                 creditSpread)));
        
        Schedule sch(issueDate, maturityDate,
                     Period(freq), calendar,
                     bdc, bdc,
                     DateGeneration::Backward, false);        
        ConvertibleFixedCouponBond bond(ex, conversionRatio,
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
                                  Rcpp::Named("yield") = bond.yield(Actual360(), Compounded, Annual),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP QL_ConvertibleFloatingBond(SEXP bondparams,  SEXP process,
                                           SEXP dividendYieldDateSexp,
                                           SEXP dividendYieldZeroSexp,
                                           SEXP rffDateSexp, SEXP rffZeroSexp,
                                           SEXP iborIndexDateSexp, SEXP iborIndexZeroSexp,
                                           SEXP indexparams, SEXP spreadVec,
                                           SEXP dividendScheduleFrame,
                                           SEXP callabilityScheduleFrame,
                                           SEXP dateparams) {

    try {

        DividendSchedule dividendSchedule = getDividendSchedule(dividendScheduleFrame);
        CallabilitySchedule callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);

        Rcpp::List rparam(bondparams);
        
        //double faceAmount = Rcpp::as<double>(rparam["faceAmount");        
        Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
        Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        double redemption = Rcpp::as<double>(rparam["redemption"]);
        std::string exercise = Rcpp::as<std::string>(rparam["exercise"]);
        double creditSpreadQuote = Rcpp::as<double>(rparam["creditSpread"]);
        double conversionRatio = Rcpp::as<double>(rparam["conversionRatio"]);

        std::vector<double> spreads = getDoubleVector(spreadVec);

        //extract iborindex curve
        Handle<YieldTermStructure> index(rebuildCurveFromZeroRates(iborIndexDateSexp, iborIndexZeroSexp));

        Rcpp::List iborparams(indexparams);      
        std::string type = Rcpp::as<std::string>(iborparams["type"]);
        double length = Rcpp::as<double>(iborparams["length"]);
        std::string inTermOf = Rcpp::as<std::string>(iborparams["inTermOf"]);

        boost::shared_ptr<IborIndex> iborindex(new USDLibor(6 * Months, index));
        if (type=="USDLibor"){
            if (inTermOf=="Months"){               
                boost::shared_ptr<IborIndex> temp(new USDLibor(length * Months, index));
                iborindex = temp;
            } else if (inTermOf=="Years") {
                boost::shared_ptr<IborIndex> temp(new USDLibor(length * Years, index));
                iborindex = temp;
            }
        }

        Rcpp::List misc(dateparams);      
        double settlementDays = Rcpp::as<double>(misc["settlementDays"]);
        std::string cal = Rcpp::as<std::string>(misc["calendar"]);
        double dayCounter = Rcpp::as<double>(misc["dayCounter"]);
        double frequency = Rcpp::as<double>(misc["period"]);
        double businessDayConvention = Rcpp::as<double>(misc["businessDayConvention"]);

        QuantLib::Date todayDate(dateFromR(iDate));
        
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
 
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        
        RelinkableHandle<Quote> underlying;
        RelinkableHandle<BlackVolTermStructure> volatility;
        boost::shared_ptr<BlackScholesMertonProcess> blackProcess;

        Handle<YieldTermStructure> dividendYield(rebuildCurveFromZeroRates(dividendYieldDateSexp,
                                                                           dividendYieldZeroSexp));

        Handle<YieldTermStructure> rff(rebuildCurveFromZeroRates(rffDateSexp,
                                                                 rffZeroSexp));

        Rcpp::List processParam(process);
        double underlyingQuote = Rcpp::as<double>(processParam["underlying"]);
        double volatilityQuote = Rcpp::as<double>(processParam["volatility"]);
        underlying.linkTo(boost::shared_ptr<Quote>(new SimpleQuote(underlyingQuote)));
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote( volatilityQuote ));
        volatility.linkTo(flatVol(todayDate, vol, dc));

        blackProcess = 
            boost::shared_ptr<BlackScholesMertonProcess>(new BlackScholesMertonProcess(underlying, dividendYield,
                                                                                       rff, volatility));

        RelinkableHandle<Quote> creditSpread;
        creditSpread.linkTo(boost::shared_ptr<Quote>(new SimpleQuote(creditSpreadQuote)));

        boost::shared_ptr<Exercise> euExercise(new EuropeanExercise(maturityDate));
        boost::shared_ptr<Exercise> amExercise(new AmericanExercise(issueDate, maturityDate));
        
        boost::shared_ptr<Exercise> ex = (exercise == "eu") ? euExercise : amExercise;
        
        
        Size timeSteps = 1001;
        boost::shared_ptr<PricingEngine> engine(new BinomialConvertibleEngine<CoxRossRubinstein>(blackProcess,
                                                                                                 timeSteps));
        
        Handle<YieldTermStructure> 
            discountCurve(boost::shared_ptr<YieldTermStructure>(new ForwardSpreadedTermStructure(rff,
                                                                                                 creditSpread)));
        Natural fixingDays = 2;
        Schedule sch(issueDate, maturityDate,
                     Period(freq), calendar,
                     bdc, bdc,
                     DateGeneration::Backward, false);        
        ConvertibleFloatingRateBond bond(ex, conversionRatio,
                                         dividendSchedule, callabilitySchedule,
                                         creditSpread,issueDate, 
                                         settlementDays,iborindex,fixingDays, spreads,
                                         dc, sch, redemption);
        bond.setPricingEngine(engine);

        return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                                  Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                                  Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                                  Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                                  Rcpp::Named("yield") = bond.yield(Actual360(), Compounded, Annual),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP QL_CallableBond(SEXP bondparams, SEXP hw, SEXP coupon,
                                SEXP callabilityScheduleFrame,
                                SEXP dateparams) {

    try {

        CallabilitySchedule callabilitySchedule = getCallabilitySchedule(callabilityScheduleFrame);

        Rcpp::List rparam(bondparams);
        
        double faceAmount = Rcpp::as<double>(rparam["faceAmount"]);        
        Rcpp::Date mDate = Rcpp::Date(Rcpp::as<int>(rparam["maturityDate"]));
        Rcpp::Date iDate = Rcpp::Date(Rcpp::as<int>(rparam["issueDate"]));
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        double redemption = Rcpp::as<double>(rparam["redemption"]);

        Rcpp::List misc(dateparams);      
        double settlementDays = Rcpp::as<double>(misc["settlementDays"]);
        std::string cal = Rcpp::as<std::string>(misc["calendar"]);
        double dayCounter = Rcpp::as<double>(misc["dayCounter"]);
        double frequency = Rcpp::as<double>(misc["period"]);
        double businessDayConvention = Rcpp::as<double>(misc["businessDayConvention"]);
        
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        } else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
 
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        
        //extract coupon rates vector
        Rcpp::NumericVector rates(coupon); 
        
        Rcpp::List hwparam(hw);
        double alpha = Rcpp::as<double>(hwparam["alpha"]);
        double sigma = Rcpp::as<double>(hwparam["sigma"]);
        double gridIntervals = Rcpp::as<double>(hwparam["gridIntervals"]);
        double rate = Rcpp::as<double>(hwparam["term"]);
        
        boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(rate));
        Handle<YieldTermStructure> termStructure(flatRate(issueDate,rRate,Actual360()));

        //Handle<YieldTermStructure> termStructure(rebuildCurveFromZeroRates(
        //                                                               hwTermDateSexp,
        //                                                               hwTermZeroSexp));
      
        boost::shared_ptr<ShortRateModel> 
            hw0(new HullWhite(termStructure,alpha,sigma));

        boost::shared_ptr<PricingEngine> 
            engine0(new TreeCallableFixedRateBondEngine(hw0,gridIntervals));

        Schedule sch(issueDate, maturityDate,
                     Period(freq), calendar, bdc, bdc,
                     DateGeneration::Backward, false);        

        CallableFixedRateBond bond(settlementDays, faceAmount, sch,
                                   Rcpp::as<std::vector <double> >(rates), 
                                   dc, bdc, redemption, issueDate, 
                                   callabilitySchedule);
        bond.setPricingEngine(engine0);

        return Rcpp::List::create(Rcpp::Named("NPV") = bond.NPV(),
                                  Rcpp::Named("cleanPrice") = bond.cleanPrice(),
                                  Rcpp::Named("dirtyPrice") = bond.dirtyPrice(),
                                  Rcpp::Named("accruedCoupon") = bond.accruedAmount(),
                                  Rcpp::Named("yield") = bond.yield(dc, Compounded, freq),
                                  Rcpp::Named("cashFlow") = getCashFlowDataFrame(bond.cashflows()));

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

// RcppExport SEXP QL_CMSBond(SEXP bondparams, SEXP iborIndex, SEXP swapIndexParam, 
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
        
//         Handle<YieldTermStructure> termStructure(rebuildCurveFromZeroRates(
//                                                                            iborIndexDate,iborIndexRates));
//         Rcppparams iborparams(iborIndex);
//         std::string ibortype = iborparams.getStringValue("type");
//         std::string iborlength = iborparams.getStringValue("length");
//         boost::shared_ptr<IborIndex> ibor = getIborIndex(termStructure, ibortype, 
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
//         Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
//         if (cal == "us"){
//             calendar = UnitedStates(UnitedStates::GovernmentBond);
//         }
//         else if (cal == "uk"){
//             calendar = UnitedKingdom(UnitedKingdom::Exchange);
//         }
//         BusinessDayConvention fixedLegBDC = getBusinessDayConvention(fixedLegConvention);
//         DayCounter fixedLedDC = getDayCounter(fixedLegDayCounter);

//         boost::shared_ptr<SwapIndex> swapIndex(new SwapIndex(familiName, 
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
//         Handle<SwaptionVolatilityStructure> atmVol;
//         atmVol = Handle<SwaptionVolatilityStructure>(
//                                                      boost::shared_ptr<SwaptionVolatilityStructure>
//                                                      new SwaptionVolatilityMatrix(swapCal,
//                                                                                   swaptionBDC,
//                                                                                   atmOptionTenors,
//                                                                                   atmSwapTenors,
//                                                                                   m,
//                                                                                   swaptionDC));


//         boost::shared_ptr<CmsCouponPricer> pricer(new NumericHaganPricer(atmVol, yieldCurveModel,
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

RcppExport SEXP QL_FittedBondCurve(SEXP curveparams, SEXP lengthVec,
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
        Rcpp::Date oDate = Rcpp::Date(Rcpp::as<int>(curvepam["origDate"]));
        QuantLib::Date origDate(dateFromR(oDate));
        Settings::instance().evaluationDate() = origDate;

        const Size numberOfBonds = length.size();

        std::vector< boost::shared_ptr<SimpleQuote> > quote;
        for (Size i=0; i<numberOfBonds; i++) {            
            boost::shared_ptr<SimpleQuote> cp(new SimpleQuote(marketQuotes[i]));
            quote.push_back(cp);
        }

        RelinkableHandle<Quote> quoteHandle[numberOfBonds];
        for (Size i=0; i<numberOfBonds; i++) {
            quoteHandle[i].linkTo(quote[i]);
        }

        Calendar calendar =  UnitedStates(UnitedStates::GovernmentBond);
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        Real redemption = 100;

        std::vector<boost::shared_ptr<FixedRateBondHelper> > instrumentsA;
        
        for (Size j=0; j < static_cast<Size>(length.size()); j++) {

            Date dated = origDate;
            Date issue = origDate;
            Date maturity = calendar.advance(issue, length[j], Years);
            
            Schedule schedule(dated, maturity, Period(freq), calendar,
                              bdc, bdc,
                              DateGeneration::Backward, false);

            boost::shared_ptr<FixedRateBondHelper> helperA(
                     new FixedRateBondHelper(quoteHandle[j], settlementDays, 100.0, schedule,
                                             std::vector<Rate>(1,coupons[j]),
                                             dc, bdc, redemption, issue));
            instrumentsA.push_back(helperA);

        }

        bool constrainAtZero = true;
        Real tolerance = 1.0e-10;
        Size max = 5000;

        boost::shared_ptr<YieldTermStructure> curve;

        if (method=="ExponentialSplinesFitting") {
            ExponentialSplinesFitting exponentialSplines(constrainAtZero);

            boost::shared_ptr<FittedBondDiscountCurve> 
                ts1 (new FittedBondDiscountCurve(settlementDays, calendar, instrumentsA,
                                                 dc, exponentialSplines, tolerance, max));
            curve = ts1;

        } else if (method == "SimplePolynomialFitting"){
            double degree = Rcpp::as<double>(curvepam["degree"]);
            SimplePolynomialFitting simplePolynomial(degree, constrainAtZero);

            boost::shared_ptr<FittedBondDiscountCurve> 
                ts2 (new FittedBondDiscountCurve(settlementDays, calendar, instrumentsA, dc,
                                                simplePolynomial, tolerance, max));
            curve = ts2;

        } else if (method == "NelsonSiegelFitting"){
            NelsonSiegelFitting nelsonSiegel;

            boost::shared_ptr<FittedBondDiscountCurve> 
                ts3 (new FittedBondDiscountCurve(settlementDays, calendar, instrumentsA, dc,
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
        Date current = curve->referenceDate();;
        int n = curve->maxDate() - curve->referenceDate();
        std::cout << curve->maxDate() << " " << curve->referenceDate() << " " << n << std::endl;

        Rcpp::DateVector dates(n);
        Rcpp::NumericVector zr(n);
        Rcpp::NumericVector di(n);

        for (int i = 0; i < n; i++) {
            Date d = current; 
            dates[i] =  Rcpp::Date(d.month(), d.dayOfMonth(), d.year());
            zr[i] = curve->zeroRate(current, ActualActual(), Continuous);
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
