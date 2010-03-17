#include "rquantlib.hpp"
using namespace boost;


RcppExport SEXP dayCount(SEXP startDates, SEXP endDates, SEXP dayCounter){
    SEXP rl = R_NilValue;
    char* exceptionMesg = NULL;
    try {
        
        RcppDateVector s = RcppDateVector(startDates);
        RcppDateVector e = RcppDateVector(endDates);
        
        RcppVector<double> rcppVec(dayCounter);
        std::vector<double> dc(rcppVec.stlVector());
        
        int n = dc.size();
        std::vector<double> result(n);
        for (int i=0; i< n; i++){
            QuantLib::Date d1( dateFromR(s(i)) );
            QuantLib::Date d2( dateFromR(e(i)) );

            DayCounter counter = getDayCounter(dc[i]);
            result[i] = (double)counter.dayCount(d1, d2);            
        }
        
        RcppResultSet rs;
        rs.add("",result);
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


RcppExport SEXP yearFraction(SEXP startDates, SEXP endDates, SEXP dayCounter){
    SEXP rl = R_NilValue;
    char* exceptionMesg = NULL;
    try {
        
        RcppDateVector s = RcppDateVector(startDates);
        RcppDateVector e = RcppDateVector(endDates);
        
        RcppVector<double> rcppVec(dayCounter);
        std::vector<double> dc(rcppVec.stlVector());
        
        int n = dc.size();
        std::vector<double> result(n);
        for (int i=0; i< n; i++){
            QuantLib::Date d1( dateFromR(s(i)) );
            QuantLib::Date d2( dateFromR(e(i)) );            
            
            
            DayCounter counter = getDayCounter(dc[i]);
            result[i] = (double)counter.yearFraction(d1, d2);            
        }        
        RcppResultSet rs;
        rs.add("",result);
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
