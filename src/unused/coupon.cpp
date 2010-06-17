#include "rquantlib.hpp"


RcppExport SEXP cfamounts(SEXP params){
       
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{
        RcppParams rparam(params); 

        QuantLib::Date maturity(dateFromR(rparam.getDateValue("Maturity")));
        QuantLib::Date settle(dateFromR(rparam.getDateValue("Settle")));
        QuantLib::Date issue(dateFromR(rparam.getDateValue("IssueDate")));

        double rate = rparam.getDoubleValue("CouponRate");
        std::vector<double> rateVec(1, rate);
        double faceAmount = rparam.getDoubleValue("Face");
        double period = rparam.getDoubleValue("Period");
        double basis = rparam.getDoubleValue("Basis");
        DayCounter dayCounter = getDayCounter(basis);
        Frequency freq = getFrequency(period);
        Period p(freq);
        double EMR = rparam.getDoubleValue("EMR");
        Calendar calendar=UnitedStates(UnitedStates::GovernmentBond);
        
        
        Schedule sch(settle, maturity, p, calendar, 
                     Unadjusted, Unadjusted, DateGeneration::Backward, 
                     (EMR == 1)? true : false);

        FixedRateBond bond(1, faceAmount, sch, rateVec, dayCounter, Following,
                           100, issue);

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


RcppExport SEXP cfdates(SEXP params){
    SEXP rl = R_NilValue;
    char* exceptionMesg = NULL;
    try {
        RcppParams rparam(params);
        
        double basis = rparam.getDoubleValue("dayCounter");
        DayCounter dayCounter = getDayCounter(basis);
        double p = rparam.getDoubleValue("period");        
        Frequency freq = getFrequency(p);
        Period period(freq);
        double emr = rparam.getDoubleValue("emr");

        bool endOfMonth = false;
        if (emr == 1) endOfMonth = true;

        QuantLib::Date d1(dateFromR(rparam.getDateValue("settle")));        
        QuantLib::Date d2(dateFromR(rparam.getDateValue("maturity")));
        Calendar calendar=UnitedStates(UnitedStates::GovernmentBond); 
        
        Schedule sch(d1, d2, period, calendar, Unadjusted,
                     Unadjusted, DateGeneration::Backward, endOfMonth);

        //cfdates
        int numCol = 1;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "Date";        
        RcppFrame frame(colNames);
        
        std::vector<QuantLib::Date> dates = sch.dates();
        for (unsigned int i = 0; i< dates.size(); i++){
            std::vector<ColDatum> row(numCol);
            Date d = dates[i];
            row[0].setDateValue(RcppDate(d.month(), d.dayOfMonth(), d.year()));           
            frame.addRow(row);
        }
        RcppResultSet rs;
        rs.add("", frame);
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



