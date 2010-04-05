#include "rquantlib.hpp";

using namespace boost;


RcppExport SEXP QL_HullWhiteCalibrationUsingCap(SEXP termStrcDateVec,
                                        SEXP termStrcZeroVec,
                                        SEXP capDataDF,
                                        SEXP iborDateVec,
                                        SEXP iborZeroVec,
                                        SEXP iborparams,
                                        SEXP evaluationDate){
    SEXP rl = R_NilValue; 	// Use this when there is nothing to be returned.
    char *exceptionMesg = NULL;

    try {
        RcppDateVector dv(evaluationDate);
        QuantLib::Date evalDate(dateFromR(dv(0)));
        Settings::instance().evaluationDate() = evalDate;
        
        Handle<YieldTermStructure> term(rebuildCurveFromZeroRates(termStrcDateVec,
                                                                  termStrcZeroVec));
        
        //set up ibor index
        Handle<YieldTermStructure> indexStrc(rebuildCurveFromZeroRates(iborDateVec,
                                                                       iborZeroVec));    
        RcppParams param(iborparams);
        std::string iborType = param.getStringValue("type");
        boost::shared_ptr<IborIndex> index = buildIborIndex(iborType,
                                                            indexStrc);
        //process capDataDF
        std::vector<boost::shared_ptr<CalibrationHelper>> caps;
        try {
            RcppFrame capDF(capDataDF);
            std::vector<std::vector<ColDatum> > table = capDF.getTableData();
            int nrow = table.size();
            for (int row=0; row<nrow;row++){
                Period p = periodByTimeUnit(table[row][0].getIntValue(),
                                            table[row][1].getStringValue());
                
                boost::shared_ptr<Quote> vol(new SimpleQuote(table[row][2].getDoubleValue()));
                DayCounter dc = getDayCounter(table[row][4].getIntValue());
                boost::shared_ptr<CalibrationHelper> 
                    helper(new CapHelper(p, Handle<Quote>(vol), index, 
                                         getFrequency(table[row][3].getIntValue()),
                                         dc,
                                         (table[row][5].getIntValue()==1)? true:false,
                                         term));
                boost::shared_ptr<BlackCapFloorEngine> engine(new BlackCapFloorEngine(term, table[row][2].getDoubleValue()));
                
                helper->setPricingEngine(engine);                
                caps.push_back(helper);
            }
        }
        catch(std::exception& ex){}
        
        
        //set up the HullWhite model
        boost::shared_ptr<HullWhite> model(new HullWhite(term));
        
        //calibrate the data
        LevenbergMarquardt optimizationMethod(1.0e-8,1.0e-8,1.0e-8);
        EndCriteria endCriteria(10000, 100, 1e-6, 1e-8, 1e-8);
        model->calibrate(caps, optimizationMethod, endCriteria);
        EndCriteria::Type ecType = model->endCriteria();
        //return the result
        Array xMinCalculated = model->params();

        RcppResultSet rs;
        rs.add("alpha", xMinCalculated[0]);
        rs.add("sigma", xMinCalculated[1]);
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


RcppExport SEXP QL_HullWhiteCalibrationUsingSwap(SEXP termStrcDateVec,
                                        SEXP termStrcZeroVec,
                                        SEXP swapDataDF,
                                        SEXP iborDateVec,
                                        SEXP iborZeroVec,
                                        SEXP iborparams,
                                        SEXP evaluationDate){
    SEXP rl = R_NilValue; 	// Use this when there is nothing to be returned.
    char *exceptionMesg = NULL;

    try {
        RcppDateVector dv(evaluationDate);
        QuantLib::Date evalDate(dateFromR(dv(0)));
        Settings::instance().evaluationDate() = evalDate;

        //set up the HullWhite model       
        Handle<YieldTermStructure> term(rebuildCurveFromZeroRates(termStrcDateVec,
                                                                  termStrcZeroVec));
        
        boost::shared_ptr<HullWhite> model(new HullWhite(term));        
        
        //set up ibor index
        Handle<YieldTermStructure> indexStrc(rebuildCurveFromZeroRates(iborDateVec,
                                                                       iborZeroVec));    
        RcppParams param(iborparams);
        std::string iborType = param.getStringValue("type");
        boost::shared_ptr<IborIndex> index = buildIborIndex(iborType,
                                                            indexStrc);
        //process capDataDF
        boost::shared_ptr<PricingEngine> engine(
                                                new JamshidianSwaptionEngine(model));
        std::vector<boost::shared_ptr<CalibrationHelper>> swaps;
        try {
            RcppFrame swapDF(swapDataDF);
            std::vector<std::vector<ColDatum> > table = swapDF.getTableData();
            int nrow = table.size();
            for (int row=0; row<nrow;row++){

                Period maturity = periodByTimeUnit(table[row][0].getIntValue(),
                                                   table[row][1].getStringValue());
                Period length = periodByTimeUnit(table[row][2].getIntValue(),
                                            table[row][3].getStringValue());                                
                boost::shared_ptr<Quote> vol(new SimpleQuote(table[row][4].getDoubleValue()));

                Period fixedLegTenor = periodByTimeUnit(table[row][5].getIntValue(),
                                            table[row][6].getStringValue());
                                
                DayCounter fixedLegDayCounter = getDayCounter(table[row][7].getIntValue());
                DayCounter floatingLegDayCounter = getDayCounter(table[row][8].getIntValue());


                boost::shared_ptr<CalibrationHelper> 
                    helper(new SwaptionHelper(maturity, length, 
                                              Handle<Quote>(vol), 
                                              index, 
                                              fixedLegTenor, 
                                              fixedLegDayCounter,
                                              floatingLegDayCounter,                                              
                                              term));
                helper->setPricingEngine(engine);                
                swaps.push_back(helper);
            }
        }
        catch(std::exception& ex){}
        
        
  
        
        //calibrate the data
        LevenbergMarquardt optimizationMethod(1.0e-8,1.0e-8,1.0e-8);
        EndCriteria endCriteria(10000, 100, 1e-6, 1e-8, 1e-8);
        model->calibrate(swaps, optimizationMethod, endCriteria);
        EndCriteria::Type ecType = model->endCriteria();
        //return the result
        Array xMinCalculated = model->params();

        RcppResultSet rs;
        rs.add("alpha", xMinCalculated[0]);
        rs.add("sigma", xMinCalculated[1]);
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
