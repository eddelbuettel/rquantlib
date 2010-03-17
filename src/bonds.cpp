/* RQuantLib -- R interface to the QuantLib libraries
##
## Copyright (C) 2002 - 2009 Dirk Eddelbuettel <edd@debian.org>
## Copyright (C) 2009        Khanh Nguyen <knguyen@cs.umb.edu>
##
## $Id$
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
*/

#include "rquantlib.hpp"
using namespace boost;


RcppExport  SEXP QL_ZeroPriceByYield(SEXP optionParameters) {
    SEXP rl = R_NilValue;
    char* exceptionMesg=NULL;
    try{
       RcppParams rparam(optionParameters);
       double yield = rparam.getDoubleValue("yield");
       double faceAmount = rparam.getDoubleValue("faceAmount");
       double dayCounter = rparam.getDoubleValue("dayCounter");
       double frequency = rparam.getDoubleValue("frequency");
       double businessDayConvention = rparam.getDoubleValue("businessDayConvention");
       double compound = rparam.getDoubleValue("compound");
       RcppDate mDate = rparam.getDateValue("maturityDate");
       RcppDate iDate = rparam.getDateValue("issueDate");
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
       
       //return cleanPrice
       RcppResultSet rs;
       DayCounter dc = getDayCounter(dayCounter);
       Compounding cp = getCompounding(compound);
       Frequency freq = getFrequency(frequency);
       rs.add("cleanPrice", zbond.cleanPrice(yield, dc, cp, freq));
       rl = rs.getReturnList();
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}

RcppExport  SEXP QL_ZeroYield(SEXP optionParameters) {
    SEXP rl = R_NilValue;
    char* exceptionMesg=NULL;
    try{
       RcppParams rparam(optionParameters);
       double price = rparam.getDoubleValue("price");
       double faceAmount = rparam.getDoubleValue("faceAmount");
       double dayCounter = rparam.getDoubleValue("dayCounter");
       double frequency = rparam.getDoubleValue("frequency");
       double businessDayConvention = rparam.getDoubleValue("businessDayConvention");
       double compound = rparam.getDoubleValue("compound");
       RcppDate mDate = rparam.getDateValue("maturityDate");
       RcppDate iDate = rparam.getDateValue("issueDate");
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
       RcppResultSet rs;
       DayCounter dc = getDayCounter(dayCounter);
       Compounding cp = getCompounding(compound);
       Frequency freq = getFrequency(frequency);
       rs.add("yield", zbond.yield(price, dc, cp, freq));
       rl = rs.getReturnList();
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}


SEXP ZeroBond(SEXP bondparam, 
              Handle<YieldTermStructure> &discountCurve,
              SEXP dateparams) {

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{
        RcppParams rparam(bondparam);
        double faceAmount = rparam.getDoubleValue("faceAmount");
        RcppDate mDate = rparam.getDateValue("maturityDate");
        RcppDate iDate = rparam.getDateValue("issueDate");
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        double redemption = rparam.getDoubleValue("redemption");

        RcppParams misc(dateparams);
        double settlementDays = misc.getDoubleValue("settlementDays");
        std::string cal = misc.getStringValue("calendar");
        double businessDayConvention = misc.getDoubleValue("businessDayConvention");
        QuantLib::Date refDate(dateFromR(misc.getDateValue("refDate")));      
        Settings::instance().evaluationDate() = refDate;                               
        
        /*
          test-suite/bonds.cpp
        */      

        //set up BusinessDayConvetion
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        
        //set up calendar
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        }
        else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
        
        
        ZeroCouponBond bond(settlementDays,
                            calendar,
                            faceAmount,
                            maturityDate,
                            bdc,
                            redemption, issueDate);
        
        boost::shared_ptr<PricingEngine> bondEngine(
                                                    new DiscountingBondEngine(discountCurve));
        bond.setPricingEngine(bondEngine);

        //cashflow
        int numCol = 2;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "Date";
        colNames[1] = "Amount";
        RcppFrame frame(colNames);
        
        Leg bondCashFlow = bond.cashflows();
        for (unsigned int i = 0; i< bondCashFlow.size(); i++){
            std::vector<ColDatum> row(numCol);
            Date d = bondCashFlow[i]->date();
            row[0].setDateValue(RcppDate(d.month(), d.dayOfMonth(), d.year()));
            row[1].setDoubleValue(bondCashFlow[i]->amount());
            frame.addRow(row);
        }

        
        RcppResultSet rs;
        rs.add("NPV", bond.NPV());
        rs.add("cleanPrice", bond.cleanPrice());
        rs.add("dirtyPrice", bond.dirtyPrice());
        rs.add("accruedCoupon", bond.accruedAmount());
        rs.add("yield", bond.yield(Actual360(), Compounded, Annual));
        rs.add("cashFlow", frame);
        rl = rs.getReturnList();
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}






SEXP FixedBond(SEXP bondparam, SEXP ratesVec,
                   Handle<YieldTermStructure> &discountCurve,
                   SEXP dateparams){
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{
        RcppParams rparam(bondparam);
        
        double faceAmount = rparam.getDoubleValue("faceAmount");
        
        RcppDate mDate = rparam.getDateValue("maturityDate");
        RcppDate eDate = rparam.getDateValue("effectiveDate");
        RcppDate iDate = rparam.getDateValue("issueDate");
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date effectiveDate(dateFromR(eDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        double redemption = rparam.getDoubleValue("redemption");

        RcppParams misc(dateparams);      
        double settlementDays = misc.getDoubleValue("settlementDays");
        std::string cal = misc.getStringValue("calendar");
        double dayCounter = misc.getDoubleValue("dayCounter");
        double frequency = misc.getDoubleValue("period");
        double businessDayConvention = misc.getDoubleValue("businessDayConvention");
        double terminationDateConvention = misc.getDoubleValue("terminationDateConvention");
        double dateGeneration = misc.getDoubleValue("dateGeneration");
        double endOfMonthRule = misc.getDoubleValue("endOfMonth");



        //extract coupon rates vector
        RcppVector<double> RcppVec(ratesVec); 
        std::vector<double> rates(RcppVec.stlVector());

        //set up BusinessDayConvetion
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        BusinessDayConvention tbdc = getBusinessDayConvention(terminationDateConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        DateGeneration::Rule rule = getDateGenerationRule(dateGeneration);
        bool endOfMonth = (endOfMonthRule==1) ? true : false;
        //set up calendar
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        }
        else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }

        //build the bond
        Schedule sch(effectiveDate, maturityDate,
                     Period(freq), calendar,
                     bdc, tbdc, rule, endOfMonth);
        
        FixedRateBond bond(settlementDays, faceAmount, sch,
                           rates,dc, bdc, redemption, issueDate);

        //bond price
        boost::shared_ptr<PricingEngine> bondEngine(
                                                    new DiscountingBondEngine(discountCurve));
        bond.setPricingEngine(bondEngine);   

        //cashflow
        int numCol = 2;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "Date";
        colNames[1] = "Amount";
        RcppFrame frame(colNames);
        
        Leg bondCashFlow = bond.cashflows();
        for (unsigned int i = 0; i< bondCashFlow.size(); i++){
            std::vector<ColDatum> row(numCol);
            Date d = bondCashFlow[i]->date();
            row[0].setDateValue(RcppDate(d.month(), d.dayOfMonth(), d.year()));
            row[1].setDoubleValue(bondCashFlow[i]->amount());
            frame.addRow(row);
        }
        
        
        RcppResultSet rs;

        rs.add("NPV", bond.NPV());
        rs.add("cleanPrice", bond.cleanPrice());
        rs.add("dirtyPrice", bond.dirtyPrice());
        rs.add("accruedCoupon", bond.accruedAmount());
        rs.add("yield", bond.yield(Actual360(), Compounded, Annual));
        rs.add("cashFlow", frame);
        rl = rs.getReturnList();

    }
    catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    return rl;
}




    

RcppExport  SEXP QL_FixedRateBondYield(SEXP optionParameters, SEXP ratesVec) {
  
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{
        RcppParams rparam(optionParameters);
        double settlementDays = rparam.getDoubleValue("settlementDays");
        std::string cal = rparam.getStringValue("calendar");
        double price = rparam.getDoubleValue("price");
        double faceAmount = rparam.getDoubleValue("faceAmount");
        double businessDayConvention = rparam.getDoubleValue("businessDayConvention");
        double compound = rparam.getDoubleValue("compound");
        double redemption = rparam.getDoubleValue("redemption");
        double dayCounter = rparam.getDoubleValue("dayCounter");
        double frequency = rparam.getDoubleValue("period");
        
        RcppDate mDate = rparam.getDateValue("maturityDate");
        RcppDate eDate = rparam.getDateValue("effectiveDate");
        RcppDate iDate = rparam.getDateValue("issueDate");
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date effectiveDate(dateFromR(eDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        
        //extract coupon rates vector
        RcppVector<double> RcppVec(ratesVec); 
        std::vector<double> rates(RcppVec.stlVector());
        
        //set up BusinessDayConvetion
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        Compounding cp = getCompounding(compound);
 
        //set up calendar
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        }
        else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
        
        //build the bond
        Schedule sch(effectiveDate, maturityDate,
                     Period(freq), calendar,
                     bdc, bdc, DateGeneration::Backward, false);
        
        FixedRateBond bond(settlementDays, faceAmount, sch,
                           rates,dc, bdc, redemption, issueDate);
        
        
        
        RcppResultSet rs;
        rs.add("yield", bond.yield(price, dc, cp, freq));
        rl = rs.getReturnList();
        
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}
 
RcppExport  SEXP QL_FixedRateBondPriceByYield(SEXP optionParameters, SEXP ratesVec) {
  
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{
        RcppParams rparam(optionParameters);
        double settlementDays = rparam.getDoubleValue("settlementDays");
        std::string cal = rparam.getStringValue("calendar");
        double yield = rparam.getDoubleValue("yield");
        double faceAmount = rparam.getDoubleValue("faceAmount");
        double businessDayConvention = rparam.getDoubleValue("businessDayConvention");
        double compound = rparam.getDoubleValue("compound");
        double redemption = rparam.getDoubleValue("redemption");
        double dayCounter = rparam.getDoubleValue("dayCounter");
        double frequency = rparam.getDoubleValue("period");
        
        RcppDate mDate = rparam.getDateValue("maturityDate");
        RcppDate eDate = rparam.getDateValue("effectiveDate");
        RcppDate iDate = rparam.getDateValue("issueDate");
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date effectiveDate(dateFromR(eDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        
        //extract coupon rates vector
        RcppVector<double> RcppVec(ratesVec); 
        std::vector<double> rates(RcppVec.stlVector());
        
        //set up BusinessDayConvetion
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        Compounding cp = getCompounding(compound);
 
        //set up calendar
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        }
        else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
        
        //build the bond
        Schedule sch(effectiveDate, maturityDate,
                     Period(freq), calendar,
                     bdc, bdc, DateGeneration::Backward, false);
        
        FixedRateBond bond(settlementDays, faceAmount, sch,
                           rates,dc, bdc, redemption, issueDate);
        
        
        
        RcppResultSet rs;
        rs.add("cleanPrice", bond.cleanPrice(yield, dc, cp, freq));
        rl = rs.getReturnList();
        
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}



SEXP FloatingBond(SEXP bondparam, SEXP gearingsVec, SEXP spreadsVec,
                  SEXP capsVec, SEXP floorsVec, 
                  Handle<YieldTermStructure> &index,
                  SEXP indexparams,
                  Handle<YieldTermStructure> &discountCurve,
                  SEXP dateparams) 
{
  
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{
        RcppParams rparam(bondparam);        
        double faceAmount = rparam.getDoubleValue("faceAmount");     
        RcppDate mDate = rparam.getDateValue("maturityDate");
        RcppDate eDate = rparam.getDateValue("effectiveDate");
        RcppDate iDate = rparam.getDateValue("issueDate");
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date effectiveDate(dateFromR(eDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        double redemption = rparam.getDoubleValue("redemption");

        RcppParams misc(dateparams);      
        double settlementDays = misc.getDoubleValue("settlementDays");
        std::string cal = misc.getStringValue("calendar");
        double dayCounter = misc.getDoubleValue("dayCounter");
        double frequency = misc.getDoubleValue("period");
        double businessDayConvention = misc.getDoubleValue("businessDayConvention");
        double terminationDateConvention = misc.getDoubleValue("terminationDateConvention");
        double dateGeneration = misc.getDoubleValue("dateGeneration");
        double endOfMonthRule = misc.getDoubleValue("endOfMonth");
        double fixingDays = misc.getDoubleValue("fixingDays");

        

        //build schedule
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        BusinessDayConvention tbdc = getBusinessDayConvention(terminationDateConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        DateGeneration::Rule rule = getDateGenerationRule(dateGeneration);
        bool endOfMonth = (endOfMonthRule==1) ? true : false;

        //set up calendar
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        }
        else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
        Schedule sch(effectiveDate, maturityDate,
                     Period(freq), calendar,
                     bdc, tbdc, rule, endOfMonth);

        //extract gearings, spreads, caps, and floors
        std::vector<double> gearings = getDoubleVector(gearingsVec);
        std::vector<double> spreads = getDoubleVector(spreadsVec);
        std::vector<double> caps = getDoubleVector(capsVec);
        std::vector<double> floors = getDoubleVector(floorsVec);

        RcppParams iborparams(indexparams);      
        std::string type = iborparams.getStringValue("type");
        double length = iborparams.getDoubleValue("length");
        std::string inTermOf = iborparams.getStringValue("inTermOf");


        boost::shared_ptr<IborIndex> iborindex(new USDLibor(6 * Months, index));
        if (type=="USDLibor"){
            if (inTermOf=="Months"){               
                boost::shared_ptr<IborIndex> temp(new USDLibor(length * Months, index));
                iborindex = temp;
            }
            else if (inTermOf=="Years") {
                boost::shared_ptr<IborIndex> temp(new USDLibor(length * Years, index));
                iborindex = temp;
            }
        }
        //build the bond
        FloatingRateBond bond(settlementDays, faceAmount, sch,
                              iborindex, dc, bdc, fixingDays,
                              gearings, spreads, caps, floors, false,
                              redemption, issueDate);        
        
        
        //bond price
        boost::shared_ptr<PricingEngine> bondEngine(
                                                    new DiscountingBondEngine(discountCurve));
        bond.setPricingEngine(bondEngine);

        
        //cashflow
        boost::shared_ptr<IborCouponPricer> pricer(new
                                                   BlackIborCouponPricer(Handle<OptionletVolatilityStructure>()));
        setCouponPricer(bond.cashflows(),pricer);

        int numCol = 2;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "Date";
        colNames[1] = "Amount";
        RcppFrame frame(colNames);
        
        Leg bondCashFlow = bond.cashflows();
        for (unsigned int i = 0; i< bondCashFlow.size(); i++){
            std::vector<ColDatum> row(numCol);
            Date d = bondCashFlow[i]->date();
            row[0].setDateValue(RcppDate(d.month(), d.dayOfMonth(), d.year()));
            row[1].setDoubleValue(bondCashFlow[i]->amount());
            frame.addRow(row);
        }
        
        
        RcppResultSet rs;

        rs.add("NPV", bond.NPV());
        rs.add("cleanPrice", bond.cleanPrice());
        rs.add("dirtyPrice", bond.dirtyPrice());
        rs.add("accruedCoupon", bond.accruedAmount());
        rs.add("yield", bond.yield(Actual360(), Compounded, Annual));
        rs.add("cashFlow", frame);
        rl = rs.getReturnList();
        
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}

RcppExport SEXP QL_FloatBond1(SEXP bond, SEXP gearings, SEXP caps,
                              SEXP spreads,
                              SEXP floors, SEXP indexparams, SEXP index, 
                              SEXP discountCurve, SEXP dateparams)
{
    
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{

        Handle<YieldTermStructure> discount_curve(getFlatCurve(discountCurve));
        Handle<YieldTermStructure> ibor_curve(getFlatCurve(index));
        rl = FloatingBond(bond, gearings, caps, spreads,
                          floors, ibor_curve, indexparams,
                          discount_curve, dateparams);       
        
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}

RcppExport SEXP QL_FloatBond2(SEXP bond, SEXP gearings, SEXP caps,
                              SEXP spreads,
                              SEXP floors, SEXP indexparams, SEXP index_params, 
                              SEXP index_tsQuotes, SEXP index_times,
                              SEXP discountCurve, SEXP dateparams)
{
    
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{

        Handle<YieldTermStructure> discount_curve(getFlatCurve(discountCurve));
        Handle<YieldTermStructure> ibor_curve(
                                              buildTermStructure(index_params,
                                                                 index_tsQuotes,
                                                                 index_times));
        rl = FloatingBond(bond, gearings, caps, spreads,
                          floors, ibor_curve, indexparams,
                          discount_curve, dateparams);       
        
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}
RcppExport SEXP QL_FloatBond3(SEXP bond, SEXP gearings, SEXP caps,
                              SEXP spreads, SEXP floors, 
                              SEXP indexparams, SEXP index, 
                              SEXP discount_params, SEXP discount_tsQuotes,
                              SEXP discount_times, SEXP dateparams)
{
    
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{

        Handle<YieldTermStructure> ibor_curve(getFlatCurve(index));
        Handle<YieldTermStructure> discount_curve(
                                                  buildTermStructure(discount_params,
                                                                 discount_tsQuotes,
                                                                 discount_times));
        rl = FloatingBond(bond, gearings, caps, spreads,
                          floors, ibor_curve, indexparams,
                          discount_curve, dateparams);       
        
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}
RcppExport SEXP QL_FloatBond4(SEXP bond, SEXP gearings, SEXP caps,
                              SEXP spreads, SEXP floors, 
                              SEXP indexparams, SEXP index_params, 
                              SEXP index_tsQuotes, SEXP index_times,
                              SEXP discount_params, SEXP discount_tsQuotes,
                              SEXP discount_times, SEXP dateparams)
{
    
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{

        Handle<YieldTermStructure> ibor_curve(
                                              buildTermStructure(index_params,
                                                                 index_tsQuotes,
                                                                 index_times));
      
        Handle<YieldTermStructure> discount_curve(
                                                  buildTermStructure(discount_params,
                                                                 discount_tsQuotes,
                                                                 discount_times));
        rl = FloatingBond(bond, gearings, caps, spreads,
                          floors, ibor_curve, indexparams,
                          discount_curve, dateparams);       
        
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}

RcppExport SEXP QL_FloatingWithRebuiltCurve(SEXP bond, SEXP gearings,
                                            SEXP spreads, SEXP caps,
                                            SEXP floors, SEXP indexparams,
                                            SEXP iborDateSexp, SEXP iborzeroSexp,
                                            SEXP dateSexp, SEXP zeroSexp,
                                            SEXP dateparams){
   SEXP rl=R_NilValue;
   char* exceptionMesg=NULL;
   try {

               
       Handle<YieldTermStructure> ibor_curve(rebuildCurveFromZeroRates(iborDateSexp,
                                                                   iborzeroSexp));       
       Handle<YieldTermStructure> curve(rebuildCurveFromZeroRates(dateSexp,
                                                                   zeroSexp));       


       rl = FloatingBond(bond, gearings, caps, spreads,
                         floors, ibor_curve, indexparams,
                         curve, dateparams);
   } catch(std::exception& ex) {
       exceptionMesg = copyMessageToR(ex.what());
   } catch(...) {
       exceptionMesg = copyMessageToR("unknown reason");
   }
   
   if(exceptionMesg != NULL)
       Rf_error(exceptionMesg);
    
   return rl;
}

RcppExport SEXP QL_FixedRateWithRebuiltCurve(SEXP bondparam, SEXP ratesVec,
                                             SEXP dateSexp, SEXP zeroSexp,
                                             SEXP dateparams){
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try {
        Handle<YieldTermStructure> curve(rebuildCurveFromZeroRates(dateSexp,
                                                                   zeroSexp));
        rl = FixedBond(bondparam, ratesVec, curve, dateparams);

    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}

RcppExport SEXP QL_ZeroBondWithRebuiltCurve(SEXP bond,
                                            SEXP dateSexp, SEXP zeroSexp,
                                            SEXP dateparams){
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{

        Handle<YieldTermStructure> curve(rebuildCurveFromZeroRates(dateSexp,
                                                                   zeroSexp));


        rl = ZeroBond(bond, curve, dateparams);
        
        
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}


RcppExport SEXP QL_ConvertibleZeroBond(SEXP bondparams, SEXP process,
                                       SEXP dividendYieldDateSexp,
                                       SEXP dividendYieldZeroSexp,
                                       SEXP rffDateSexp, SEXP rffZeroSexp,
                                       SEXP dividendScheduleFrame,
                                       SEXP callabilityScheduleFrame,
                                       SEXP dateparams) {

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{


 


        DividendSchedule dividendSchedule;
        CallabilitySchedule callabilitySchedule;

        try {
            RcppFrame rcppDividendSchedule(dividendScheduleFrame);        
            std::vector<std::vector<ColDatum> > table = rcppDividendSchedule.getTableData();
            int nrow = table.size();
            //int ncol = table[0].size();
            for (int row=0;row<nrow;row++){          
                int type = (table[row][0].getStringValue()=="Fixed") ? 1 : 0;
                double amount = table[row][1].getDoubleValue();
                double rate = table[row][2].getDoubleValue();
                QuantLib::Date d(dateFromR(table[row][3].getDateValue()));            
                if (type==1){
                    dividendSchedule.push_back(
                                               boost::shared_ptr<Dividend>(new FixedDividend(amount, d)));
                }
                else {
                    dividendSchedule.push_back(
                                               boost::shared_ptr<Dividend>(new FractionalDividend(rate, amount, d)));                                           
                }
            }
        }
        catch (std::exception& ex){}

        try {
            RcppFrame rcppCallabilitySchedule(callabilityScheduleFrame);
            std::vector<std::vector<ColDatum> > table = rcppCallabilitySchedule.getTableData();
            int nrow = table.size();
            for (int row=0;row<nrow;row++){
                double price = table[row][0].getDoubleValue();
                int type = (table[row][1].getStringValue()=="P") ? 1 : 0;
                QuantLib::Date d(dateFromR(table[row][2].getDateValue()));            
                
                if (type==1){
                    callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                                  (new Callability(Callability::Price(price, 
                                                                                      Callability::Price::Clean),
                                                                   Callability::Put,d )));
            }
                else {
                    callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                                  (new Callability(Callability::Price(price, 
                                                                                      Callability::Price::Clean),
                                                                   Callability::Call,d )));
                }            
            }
        }
        catch (std::exception& ex){}

        RcppParams rparam(bondparams);
        
        //double faceAmount = rparam.getDoubleValue("faceAmount");        
        RcppDate mDate = rparam.getDateValue("maturityDate");
        RcppDate iDate = rparam.getDateValue("issueDate");
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        double redemption = rparam.getDoubleValue("redemption");
        std::string exercise = rparam.getStringValue("exercise");
        double creditSpreadQuote = rparam.getDoubleValue("creditSpread");
        double conversionRatio = rparam.getDoubleValue("conversionRatio");


        RcppParams misc(dateparams);      
        double settlementDays = misc.getDoubleValue("settlementDays");
        std::string cal = misc.getStringValue("calendar");
        double dayCounter = misc.getDoubleValue("dayCounter");
        double frequency = misc.getDoubleValue("period");
        double businessDayConvention = misc.getDoubleValue("businessDayConvention");

        QuantLib::Date todayDate(dateFromR(iDate));
        
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        }
        else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
 
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        

        RelinkableHandle<Quote> underlying;
        RelinkableHandle<BlackVolTermStructure> volatility;
        boost::shared_ptr<BlackScholesMertonProcess> blackProcess;

        Handle<YieldTermStructure> dividendYield(rebuildCurveFromZeroRates(
                                                                           dividendYieldDateSexp,
                                                                           dividendYieldZeroSexp));

        Handle<YieldTermStructure> rff(rebuildCurveFromZeroRates(rffDateSexp,
                                                                 rffZeroSexp));

        RcppParams processParam(process);
        double underlyingQuote = processParam.getDoubleValue("underlying");
        double volatilityQuote = processParam.getDoubleValue("volatility");
        underlying.linkTo(boost::shared_ptr<Quote>(new SimpleQuote(underlyingQuote)));
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote( volatilityQuote ));
        volatility.linkTo(flatVol(todayDate, vol, dc));

        blackProcess = boost::shared_ptr<BlackScholesMertonProcess>(
                    new BlackScholesMertonProcess(underlying, dividendYield,
                                                  rff, volatility));

        RelinkableHandle<Quote> creditSpread;
        creditSpread.linkTo(
                            boost::shared_ptr<Quote>(new SimpleQuote(creditSpreadQuote)));

        boost::shared_ptr<Exercise> euExercise(
                                               new EuropeanExercise(maturityDate));
        boost::shared_ptr<Exercise> amExercise(
                                               new AmericanExercise(issueDate,
                                                                    maturityDate));
        
        boost::shared_ptr<Exercise> ex = (exercise == "eu") ? euExercise : amExercise;
        
        
        Size timeSteps = 1001;
        boost::shared_ptr<PricingEngine> engine(
                                                new BinomialConvertibleEngine<CoxRossRubinstein>(blackProcess,
                                                                                                 timeSteps));
        
        Handle<YieldTermStructure> discountCurve(
                                                 boost::shared_ptr<YieldTermStructure>(
                                                                                       new ForwardSpreadedTermStructure(rff,
                                                                                                                        creditSpread)));
        
        Schedule sch(issueDate, maturityDate,
                     Period(freq), calendar,
                     bdc, bdc,
                     DateGeneration::Backward, false);        
        ConvertibleZeroCouponBond bond(ex, conversionRatio,
                                     dividendSchedule, callabilitySchedule,
                                     creditSpread,
                                     issueDate, settlementDays,
                                     dc, sch,
                                     redemption);
        bond.setPricingEngine(engine);


        //cashflow
        int numCol = 2;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "Date";
        colNames[1] = "Amount";
        RcppFrame frame(colNames);
        
        Leg bondCashFlow = bond.cashflows();
        for (unsigned int i = 0; i< bondCashFlow.size(); i++){
            std::vector<ColDatum> row(numCol);
            Date d = bondCashFlow[i]->date();
            row[0].setDateValue(RcppDate(d.month(), d.dayOfMonth(), d.year()));
            row[1].setDoubleValue(bondCashFlow[i]->amount());
            frame.addRow(row);
        }
        
        
        RcppResultSet rs;

        rs.add("NPV", bond.NPV());
        rs.add("cleanPrice", bond.cleanPrice());
        rs.add("dirtyPrice", bond.dirtyPrice());
        rs.add("accruedCoupon", bond.accruedAmount());
        rs.add("yield", bond.yield(Actual360(), Compounded, Annual));
        rs.add("cashFlow", frame);
        rl = rs.getReturnList();

    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}

RcppExport SEXP QL_ConvertibleFixedBond(SEXP bondparams, SEXP coupon, SEXP process,
                                       SEXP dividendYieldDateSexp,
                                       SEXP dividendYieldZeroSexp,
                                       SEXP rffDateSexp, SEXP rffZeroSexp,
                                       SEXP dividendScheduleFrame,
                                       SEXP callabilityScheduleFrame,
                                       SEXP dateparams) {

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{

        DividendSchedule dividendSchedule;
        CallabilitySchedule callabilitySchedule;

        try {
            RcppFrame rcppDividendSchedule(dividendScheduleFrame);        
            std::vector<std::vector<ColDatum> > table = rcppDividendSchedule.getTableData();
            int nrow = table.size();
            //int ncol = table[0].size();
            for (int row=0;row<nrow;row++){          
                int type = (table[row][0].getStringValue()=="Fixed") ? 1 : 0;
                double amount = table[row][1].getDoubleValue();
                double rate = table[row][2].getDoubleValue();
                QuantLib::Date d(dateFromR(table[row][3].getDateValue()));            
                if (type==1){
                    dividendSchedule.push_back(
                                               boost::shared_ptr<Dividend>(new FixedDividend(amount, d)));
                }
                else {
                    dividendSchedule.push_back(
                                               boost::shared_ptr<Dividend>(new FractionalDividend(rate, amount, d)));                                           
                }
            }
        }
        catch (std::exception& ex){}

        try {
            RcppFrame rcppCallabilitySchedule(callabilityScheduleFrame);
            std::vector<std::vector<ColDatum> > table = rcppCallabilitySchedule.getTableData();
            int nrow = table.size();
            for (int row=0;row<nrow;row++){
                double price = table[row][0].getDoubleValue();
                int type = (table[row][1].getStringValue()=="P") ? 1 : 0;
                QuantLib::Date d(dateFromR(table[row][2].getDateValue()));            
                
                if (type==1){
                    callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                                  (new Callability(Callability::Price(price, 
                                                                                      Callability::Price::Clean),
                                                                   Callability::Put,d )));
            }
                else {
                    callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                                  (new Callability(Callability::Price(price, 
                                                                                      Callability::Price::Clean),
                                                                   Callability::Call,d )));
                }            
            }
        }
        catch (std::exception& ex){}

        RcppParams rparam(bondparams);
        
        //double faceAmount = rparam.getDoubleValue("faceAmount");        
        RcppDate mDate = rparam.getDateValue("maturityDate");
        RcppDate iDate = rparam.getDateValue("issueDate");
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        double redemption = rparam.getDoubleValue("redemption");
        std::string exercise = rparam.getStringValue("exercise");
        double creditSpreadQuote = rparam.getDoubleValue("creditSpread");
        double conversionRatio = rparam.getDoubleValue("conversionRatio");


        //extract coupon rates vector
        RcppVector<double> RcppVec(coupon); 
        std::vector<double> rates(RcppVec.stlVector());

        RcppParams misc(dateparams);      
        double settlementDays = misc.getDoubleValue("settlementDays");
        std::string cal = misc.getStringValue("calendar");
        double dayCounter = misc.getDoubleValue("dayCounter");
        double frequency = misc.getDoubleValue("period");
        double businessDayConvention = misc.getDoubleValue("businessDayConvention");
       
        QuantLib::Date todayDate(dateFromR(iDate));
	Settings::instance().evaluationDate() = todayDate;
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        }
        else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
 
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        

        RelinkableHandle<Quote> underlying;
        RelinkableHandle<BlackVolTermStructure> volatility;

        Handle<YieldTermStructure> dividendYield(rebuildCurveFromZeroRates(
                                                                           dividendYieldDateSexp,
                                                                           dividendYieldZeroSexp));

        Handle<YieldTermStructure> rff(rebuildCurveFromZeroRates(rffDateSexp,
                                                                 rffZeroSexp));

        RcppParams processParam(process);
        double underlyingQuote = processParam.getDoubleValue("underlying");
        double volatilityQuote = processParam.getDoubleValue("volatility");
        underlying.linkTo(boost::shared_ptr<Quote>(new SimpleQuote(underlyingQuote)));
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote( volatilityQuote ));
        volatility.linkTo(flatVol(todayDate, vol, dc));

	boost::shared_ptr<BlackScholesMertonProcess> blackProcess;
        blackProcess = boost::shared_ptr<BlackScholesMertonProcess>(
								    new BlackScholesMertonProcess(underlying, dividendYield,
												  rff, volatility));
	//	boost::shared_ptr<BlackScholesProcess> blackProcess;
        //ackProcess = boost::shared_ptr<BlackScholesProcess>(
	//					      new BlackScholesProcess(underlying, 
	//								      rff, volatility));


        RelinkableHandle<Quote> creditSpread;
        creditSpread.linkTo(
                            boost::shared_ptr<Quote>(new SimpleQuote(creditSpreadQuote)));

        boost::shared_ptr<Exercise> euExercise(
                                               new EuropeanExercise(maturityDate));
        boost::shared_ptr<Exercise> amExercise(
                                               new AmericanExercise(issueDate,
                                                                    maturityDate));
        
        boost::shared_ptr<Exercise> ex = (exercise == "eu") ? euExercise : amExercise;
        
        
        Size timeSteps = 1001;
        boost::shared_ptr<PricingEngine> engine(
                                                new BinomialConvertibleEngine<CoxRossRubinstein>(blackProcess,
                                                                                                 timeSteps));
        
        Handle<YieldTermStructure> discountCurve(
                                                 boost::shared_ptr<YieldTermStructure>(
                                                                                       new ForwardSpreadedTermStructure(rff,
                                                                                                                        creditSpread)));
        
        Schedule sch(issueDate, maturityDate,
                     Period(freq), calendar,
                     bdc, bdc,
                     DateGeneration::Backward, false);        
        ConvertibleFixedCouponBond bond(ex, conversionRatio,
                                        dividendSchedule, callabilitySchedule,
                                        creditSpread,issueDate, 
                                        settlementDays,rates,
                                        dc, sch, redemption);
        bond.setPricingEngine(engine);


        //cashflow
        int numCol = 2;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "Date";
        colNames[1] = "Amount";
        RcppFrame frame(colNames);
        
        Leg bondCashFlow = bond.cashflows();
        for (unsigned int i = 0; i< bondCashFlow.size(); i++){
            std::vector<ColDatum> row(numCol);
            Date d = bondCashFlow[i]->date();
            row[0].setDateValue(RcppDate(d.month(), d.dayOfMonth(), d.year()));
            row[1].setDoubleValue(bondCashFlow[i]->amount());
            frame.addRow(row);
        }
        
        
        RcppResultSet rs;

        rs.add("NPV", bond.NPV());
        rs.add("cleanPrice", bond.cleanPrice());
        rs.add("dirtyPrice", bond.dirtyPrice());
        rs.add("accruedCoupon", bond.accruedAmount());
        rs.add("yield", bond.yield(Actual360(), Compounded, Annual));
        rs.add("cashFlow", frame);
        rl = rs.getReturnList();

    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
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

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{

        DividendSchedule dividendSchedule;
        CallabilitySchedule callabilitySchedule;

        try {
            RcppFrame rcppDividendSchedule(dividendScheduleFrame);        
            std::vector<std::vector<ColDatum> > table = rcppDividendSchedule.getTableData();
            int nrow = table.size();
            //int ncol = table[0].size();
            for (int row=0;row<nrow;row++){          
                int type = (table[row][0].getStringValue()=="Fixed") ? 1 : 0;
                double amount = table[row][1].getDoubleValue();
                double rate = table[row][2].getDoubleValue();
                QuantLib::Date d(dateFromR(table[row][3].getDateValue()));            
                if (type==1){
                    dividendSchedule.push_back(
                                               boost::shared_ptr<Dividend>(new FixedDividend(amount, d)));
                }
                else {
                    dividendSchedule.push_back(
                                               boost::shared_ptr<Dividend>(new FractionalDividend(rate, amount, d)));                                           
                }
            }
        }
        catch (std::exception& ex){}

        try {
            RcppFrame rcppCallabilitySchedule(callabilityScheduleFrame);
            std::vector<std::vector<ColDatum> > table = rcppCallabilitySchedule.getTableData();
            int nrow = table.size();
            for (int row=0;row<nrow;row++){
                double price = table[row][0].getDoubleValue();
                int type = (table[row][1].getStringValue()=="P") ? 1 : 0;
                QuantLib::Date d(dateFromR(table[row][2].getDateValue()));            
                
                if (type==1){
                    callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                                  (new Callability(Callability::Price(price, 
                                                                                      Callability::Price::Clean),
                                                                   Callability::Put,d )));
            }
                else {
                    callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                                  (new Callability(Callability::Price(price, 
                                                                                      Callability::Price::Clean),
                                                                   Callability::Call,d )));
                }            
            }
        }
        catch (std::exception& ex){}

        RcppParams rparam(bondparams);
        
        //double faceAmount = rparam.getDoubleValue("faceAmount");        
        RcppDate mDate = rparam.getDateValue("maturityDate");
        RcppDate iDate = rparam.getDateValue("issueDate");
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        double redemption = rparam.getDoubleValue("redemption");
        std::string exercise = rparam.getStringValue("exercise");
        double creditSpreadQuote = rparam.getDoubleValue("creditSpread");
        double conversionRatio = rparam.getDoubleValue("conversionRatio");


        std::vector<double> spreads = getDoubleVector(spreadVec);

        //extract iborindex curve
        Handle<YieldTermStructure> index(rebuildCurveFromZeroRates(iborIndexDateSexp,
                                                                 iborIndexZeroSexp));

        RcppParams iborparams(indexparams);      
        std::string type = iborparams.getStringValue("type");
        double length = iborparams.getDoubleValue("length");
        std::string inTermOf = iborparams.getStringValue("inTermOf");

        boost::shared_ptr<IborIndex> iborindex(new USDLibor(6 * Months, index));
        if (type=="USDLibor"){
            if (inTermOf=="Months"){               
                boost::shared_ptr<IborIndex> temp(new USDLibor(length * Months, index));
                iborindex = temp;
            }
            else if (inTermOf=="Years") {
                boost::shared_ptr<IborIndex> temp(new USDLibor(length * Years, index));
                iborindex = temp;
            }
        }


        RcppParams misc(dateparams);      
        double settlementDays = misc.getDoubleValue("settlementDays");
        std::string cal = misc.getStringValue("calendar");
        double dayCounter = misc.getDoubleValue("dayCounter");
        double frequency = misc.getDoubleValue("period");
        double businessDayConvention = misc.getDoubleValue("businessDayConvention");

        QuantLib::Date todayDate(dateFromR(iDate));
        
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        }
        else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
 
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        

        RelinkableHandle<Quote> underlying;
        RelinkableHandle<BlackVolTermStructure> volatility;
        boost::shared_ptr<BlackScholesMertonProcess> blackProcess;

        Handle<YieldTermStructure> dividendYield(rebuildCurveFromZeroRates(
                                                                           dividendYieldDateSexp,
                                                                           dividendYieldZeroSexp));

        Handle<YieldTermStructure> rff(rebuildCurveFromZeroRates(rffDateSexp,
                                                                 rffZeroSexp));

        RcppParams processParam(process);
        double underlyingQuote = processParam.getDoubleValue("underlying");
        double volatilityQuote = processParam.getDoubleValue("volatility");
        underlying.linkTo(boost::shared_ptr<Quote>(new SimpleQuote(underlyingQuote)));
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote( volatilityQuote ));
        volatility.linkTo(flatVol(todayDate, vol, dc));

        blackProcess = boost::shared_ptr<BlackScholesMertonProcess>(
                    new BlackScholesMertonProcess(underlying, dividendYield,
                                                  rff, volatility));

        RelinkableHandle<Quote> creditSpread;
        creditSpread.linkTo(
                            boost::shared_ptr<Quote>(new SimpleQuote(creditSpreadQuote)));

        boost::shared_ptr<Exercise> euExercise(
                                               new EuropeanExercise(maturityDate));
        boost::shared_ptr<Exercise> amExercise(
                                               new AmericanExercise(issueDate,
                                                                    maturityDate));
        
        boost::shared_ptr<Exercise> ex = (exercise == "eu") ? euExercise : amExercise;
        
        
        Size timeSteps = 1001;
        boost::shared_ptr<PricingEngine> engine(
                                                new BinomialConvertibleEngine<CoxRossRubinstein>(blackProcess,
                                                                                                 timeSteps));
        
        Handle<YieldTermStructure> discountCurve(
                                                 boost::shared_ptr<YieldTermStructure>(
                                                                                       new ForwardSpreadedTermStructure(rff,
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


        //cashflow
        int numCol = 2;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "Date";
        colNames[1] = "Amount";
        RcppFrame frame(colNames);
        
        Leg bondCashFlow = bond.cashflows();
        for (unsigned int i = 0; i< bondCashFlow.size(); i++){
            std::vector<ColDatum> row(numCol);
            Date d = bondCashFlow[i]->date();
            row[0].setDateValue(RcppDate(d.month(), d.dayOfMonth(), d.year()));
            row[1].setDoubleValue(bondCashFlow[i]->amount());
            frame.addRow(row);
        }
        
        
        RcppResultSet rs;

        rs.add("NPV", bond.NPV());
        rs.add("cleanPrice", bond.cleanPrice());
        rs.add("dirtyPrice", bond.dirtyPrice());
        rs.add("accruedCoupon", bond.accruedAmount());
        rs.add("yield", bond.yield(Actual360(), Compounded, Annual));
        rs.add("cashFlow", frame);
        rl = rs.getReturnList();

    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}
RcppExport SEXP QL_CallableBond(SEXP bondparams, SEXP hw, SEXP coupon,
                                
                                
                                SEXP callabilityScheduleFrame,
                                SEXP dateparams) {

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{

        CallabilitySchedule callabilitySchedule;
        try {
            RcppFrame rcppCallabilitySchedule(callabilityScheduleFrame);
            std::vector<std::vector<ColDatum> > table = rcppCallabilitySchedule.getTableData();
            int nrow = table.size();
            for (int row=0;row<nrow;row++){
                double price = table[row][0].getDoubleValue();
                int type = (table[row][1].getStringValue()=="P") ? 1 : 0;
                QuantLib::Date d(dateFromR(table[row][2].getDateValue()));            

                if (type==1){
                    callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                                  (new Callability(Callability::Price(price, 
                                                                                      Callability::Price::Clean),
                                                                   Callability::Put,d )));
            }
                else {
                    callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                                  (new Callability(Callability::Price(price, 
                                                                                      Callability::Price::Clean),
                                                                   Callability::Call,d )));
                }            
            }
        }
        catch (std::exception& ex){                  
        }

        RcppParams rparam(bondparams);
        
        double faceAmount = rparam.getDoubleValue("faceAmount");        
        RcppDate mDate = rparam.getDateValue("maturityDate");
        RcppDate iDate = rparam.getDateValue("issueDate");
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        double redemption = rparam.getDoubleValue("redemption");



        RcppParams misc(dateparams);      
        double settlementDays = misc.getDoubleValue("settlementDays");
        std::string cal = misc.getStringValue("calendar");
        double dayCounter = misc.getDoubleValue("dayCounter");
        double frequency = misc.getDoubleValue("period");
        double businessDayConvention = misc.getDoubleValue("businessDayConvention");
        
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        }
        else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
 
        BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);
        DayCounter dc = getDayCounter(dayCounter);
        Frequency freq = getFrequency(frequency);
        
        //extract coupon rates vector
        RcppVector<double> RcppVec(coupon); 
        std::vector<double> rates(RcppVec.stlVector());

        RcppParams hwparam(hw);
        double alpha = hwparam.getDoubleValue("alpha");
        double sigma = hwparam.getDoubleValue("sigma");
        double gridIntervals = hwparam.getDoubleValue("gridIntervals");
        double rate = hwparam.getDoubleValue("term");
        
        boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(rate));
        Handle<YieldTermStructure> termStructure(flatRate(issueDate,rRate,Actual360()));

        //Handle<YieldTermStructure> termStructure(rebuildCurveFromZeroRates(
        //                                                               hwTermDateSexp,
        //                                                               hwTermZeroSexp));
        
      

        boost::shared_ptr<ShortRateModel> hw0(
                       new HullWhite(termStructure,alpha,sigma));

        boost::shared_ptr<PricingEngine> engine0(
                      new TreeCallableFixedRateBondEngine(hw0,gridIntervals));



        Schedule sch(issueDate, maturityDate,
                     Period(freq), calendar,
                     bdc, bdc,
                     DateGeneration::Backward, false);        

        CallableFixedRateBond bond(settlementDays, faceAmount, 
                                   sch,rates,
                                   dc, bdc,
                                   redemption, issueDate, 
                                   callabilitySchedule);
        bond.setPricingEngine(engine0);

        //cashflow
        int numCol = 2;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "Date";
        colNames[1] = "Amount";
        RcppFrame frame(colNames);

        Leg bondCashFlow = bond.cashflows();
        for (unsigned int i = 0; i< bondCashFlow.size(); i++){
            std::vector<ColDatum> row(numCol);
            Date d = bondCashFlow[i]->date();
            row[0].setDateValue(RcppDate(d.month(), d.dayOfMonth(), d.year()));
            row[1].setDoubleValue(bondCashFlow[i]->amount());
            frame.addRow(row);
        }

        
        RcppResultSet rs;

        rs.add("NPV", bond.NPV());
        rs.add("cleanPrice", bond.cleanPrice());
        rs.add("dirtyPrice", bond.dirtyPrice());
        rs.add("accruedCoupon", bond.accruedAmount());
        rs.add("yield", bond.yield(dc, Compounded, freq));
        rs.add("cashFlow", frame);
        rl = rs.getReturnList();

    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}
/*
RcppExport SEXP QL_CMSBond(SEXP bondparams, SEXP iborIndex, SEXP swapIndexParam, 
                           SEXP capsVec, SEXP floorsVec, SEXP gearingsVec, 
                           SEXP spreadsVec, SEXP swaptionVolSEXP, SEXP atmOptionTenorsSEXP,
                           SEXP atmSwapTenorsSEXP, SEXP volMatrixSEXP, SEXP pricer,
                           SEXP iborIndexDate, SEXP iborIndexRates)
{
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try {

        std::vector<double> gearings = getDoubleVector(gearingsVec);
        std::vector<double> spreads = getDoubleVector(spreadsVec);
        std::vector<double> caps = getDoubleVector(capsVec);
        std::vector<double> floors = getDoubleVector(floorsVec);

        RcppStringVector strVec(atmOptionTenorsSEXP);
        std::vector<string> atmOptionTenors(strVec.stlVector());
        RcppStringVector strVec2(atmSwapTenorsSEXP);
        std::vector<string> atmSwapTenors(strVec2.stlVector());

        RcppMatrix<double> m(volMatrixSEXP);
        
        Handle<YieldTermStructure> termStructure(rebuildCurveFromZeroRates(
                                                                           iborIndexDate,iborIndexRates));
        Rcppparams iborparams(iborIndex);
        std::string ibortype = iborparams.getStringValue("type");
        std::string iborlength = iborparams.getStringValue("length");
        boost::shared_ptr<IborIndex> ibor = getIborIndex(termStructure, ibortype, 
                                                         iborlength);
        //fix tenor to make sure it is converted by matchparam
        Rcppparams swapparams(swapIndexParam);
        std::string familyName = swapparams.getStringValue("familyName");
        std::double tenor = swapparams.getDoubleValue("tenor");
        std::double settlementDays = swapparams.getDoubleValue("settlementDays");
        std::string currency = swapparams.getStringValue("currency");
        std::string fixedLegTenor = swapparams.getDoubleValue("fixedLegTenor");
        std::string fixedLegConvention = swapparams.getDoubleValue("fixedLegConvention");
        std::string fixedLegDayCounter = swapparams.getDoubleValue("fixedLegDayCounter");
        std::string cal = swapparams.getStringValue("calendar");
        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        if (cal == "us"){
            calendar = UnitedStates(UnitedStates::GovernmentBond);
        }
        else if (cal == "uk"){
            calendar = UnitedKingdom(UnitedKingdom::Exchange);
        }
        BusinessDayConvention fixedLegBDC = getBusinessDayConvention(fixedLegConvention);
        DayCounter fixedLedDC = getDayCounter(fixedLegDayCounter);

        boost::shared_ptr<SwapIndex> swapIndex(new SwapIndex(familiName, 
                                                             getPeriodFromString(fixedLegTenor),
                                                             settlemenDays, 
                                                             currency, 
                                                             calendar,
                                                             getPeriodFromString(fixedLegTenor), 
                                                             fixedLegBDC, 
                                                             fixedLegDC, 
                                                             ibor));

        Rcppparams pricerparams(pricer);
        std::string pricerType = pricerparams.getStringValue("type");
        std::double zeroMeanRev = pricerparams.getDoubleValue("zeroMeanRev");
        
        Rcppparams swaptionVolParams(swaptionVolSEXP);
        std::string swaptionCal = swaptionVolParams.getStringValue("calendar");
        std::double swaptionBDC = swaptionVolParams.getDoubleValue("businessDayConvention");
        std::double swaptionDC = swaptionVolParams.getDoubleValue("dayCounter");
        Handle<SwaptionVolatilityStructure> atmVol;
        atmVol = Handle<SwaptionVolatilityStructure>(
                                                     boost::shared_ptr<SwaptionVolatilityStructure>
                                                     new SwaptionVolatilityMatrix(swapCal,
                                                                                  swaptionBDC,
                                                                                  atmOptionTenors,
                                                                                  atmSwapTenors,
                                                                                  m,
                                                                                  swaptionDC));


        boost::shared_ptr<CmsCouponPricer> pricer(new NumericHaganPricer(atmVol, yieldCurveModel,
                                                                         zeroMeanRev));
        

        Rcppparams rparams(bondparams);        
        RcppDate mDate = rparam.getDateValue("maturityDate");
        RcppDate iDate = rparam.getDateValue("issueDate");
        RcppDate pDate = rparam.getDateValue("paymentDate");
        QuantLib::Date maturityDate(dateFromR(mDate));
        QuantLib::Date issueDate(dateFromR(iDate));
        QuantLib::Date paymentDate(dateFromR(pDate));
        std::double nomial = rparams.getDoubleValue("nomial");
        
        CappedFlooredCmsCoupon coupon(paymentDate, nomial,
                                      issueDate, maturityDate, 
                                      settlementDays, swapIndex,
                                      gearings, spreads, 
                                      caps, floors,
                                      issueDate, maturityDate,
                                      dayCounter);

        pricer->setSwaptionVolatility(atmVol);
        coupon.setPricer(pricer);
        

        
    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}
*/
RcppExport SEXP QL_FittedBondCurve(SEXP curveparams, SEXP lengthVec,
                                   SEXP couponVec,SEXP marketVec,
                                   SEXP dateparams){
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try {


        //extract length, coupon and market prices vector
        RcppVector<int> RcppVec(lengthVec); 
        std::vector<int> lengths(RcppVec.stlVector());
        RcppVector<double> RcppVec1(couponVec);
        std::vector<double> coupons(RcppVec1.stlVector());
        RcppVector<double> RcppVec2(marketVec);
        std::vector<double> marketQuotes(RcppVec2.stlVector());


        RcppParams misc(dateparams);      
        double settlementDays = misc.getDoubleValue("settlementDays");
        double dayCounter = misc.getDoubleValue("dayCounter");
        double frequency = misc.getDoubleValue("period");
        double businessDayConvention = misc.getDoubleValue("businessDayConvention");
    
        RcppParams curvepam(curveparams);
        std::string method = curvepam.getStringValue("method");
        RcppDate oDate = curvepam.getDateValue("origDate");
        QuantLib::Date origDate(dateFromR(oDate));
        Settings::instance().evaluationDate() = origDate;

        const Size numberOfBonds = lengths.size();

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
        
        for (Size j=0; j<lengths.size(); j++) {

            Date dated = origDate;
            Date issue = origDate;
            Date maturity = calendar.advance(issue, lengths[j], Years);
            
            Schedule schedule(dated, maturity, Period(freq), calendar,
                              bdc, bdc,
                              DateGeneration::Backward, false);

            boost::shared_ptr<FixedRateBondHelper> helperA(
                     new FixedRateBondHelper(quoteHandle[j],
                                             settlementDays,
                                             100.0,
                                             schedule,
                                             std::vector<Rate>(1,coupons[j]),
                                             dc,
                                             bdc,
                                             redemption,
                                             issue));
            instrumentsA.push_back(helperA);

        }

        bool constrainAtZero = true;
        Real tolerance = 1.0e-10;
        Size max = 5000;

	boost::shared_ptr<YieldTermStructure> curve;

        if (method=="ExponentialSplinesFitting"){
            ExponentialSplinesFitting exponentialSplines(constrainAtZero);

            boost::shared_ptr<FittedBondDiscountCurve> ts1 (
                                                            new FittedBondDiscountCurve(settlementDays,
                                                                                        calendar,
                                                                                        instrumentsA,
                                                                                        dc,
                                                                                        exponentialSplines,
                                                                                        tolerance,
                                                                                        max));
            curve = ts1;
        }
        else if (method == "SimplePolynomialFitting"){
            double degree = curvepam.getDoubleValue("degree");
            SimplePolynomialFitting simplePolynomial(degree, constrainAtZero);

            boost::shared_ptr<FittedBondDiscountCurve> ts2 (
                    new FittedBondDiscountCurve(settlementDays,
                                                calendar,
                                                instrumentsA,
                                                dc,
                                                simplePolynomial,
                                                tolerance,
                                                max));
            curve = ts2;
        }
        else if (method == "NelsonSiegelFitting"){
            NelsonSiegelFitting nelsonSiegel;

            boost::shared_ptr<FittedBondDiscountCurve> ts3 (
                        new FittedBondDiscountCurve(settlementDays,
                                                    calendar,
                                                    instrumentsA,
                                                    dc,
                                                    nelsonSiegel,
                                                    tolerance,
                                                    max));
            curve = ts3;
        }
        
        

	// Return discount, forward rate, and zero coupon curves
        int numCol = 3;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "date";
        colNames[1] = "zeroRates";
        colNames[2] = "discount";


        RcppFrame frame(colNames);
        Date current = curve->referenceDate();;
        int n = curve->maxDate() - curve->referenceDate();
        std::cout << n << std::endl;
        for (int i = 0; i<n;i++){
            std::vector<ColDatum> row(numCol);
            Date d = current; 
            row[0].setDateValue(RcppDate(d.month(),
                                         d.dayOfMonth(),
                                         d.year()));
            
            double zrate = curve->zeroRate(current, 
                                           ActualActual(),
                                           Continuous);
            row[1].setDoubleValue(zrate);                        
            
            double discount = curve->discount(current);
            row[2].setDoubleValue(discount);

            frame.addRow(row);
            current++;
        }

	RcppResultSet rs;
        rs.add("table", frame);
	rl = rs.getReturnList();

    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
    
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}
