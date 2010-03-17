#include "rquantlib.hpp"

using namespace boost;
RcppExport SEXP cbprice(SEXP params, SEXP dividendFrame, SEXP callFrame){
    
    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
    try{
        RcppParams rparams(params);

        double rff = rparams.getDoubleValue("rff");
        double spread = rparams.getDoubleValue("spread");
        double sigma = rparams.getDoubleValue("sigma");
        double price = rparams.getDoubleValue("price");
        double convRatio = rparams.getDoubleValue("convRatio");
        double numSteps = rparams.getDoubleValue("numSteps");
        QuantLib::Date maturity(dateFromR(rparams.getDateValue("maturity")));
        QuantLib::Date settle(dateFromR(rparams.getDateValue("settle")));
        QuantLib::Date issue(dateFromR(rparams.getDateValue("issue")));
        double coupon = rparams.getDoubleValue("couponRate");
        double p = rparams.getDoubleValue("period");
        double basis = rparams.getDoubleValue("basis");
        DayCounter dayCounter = getDayCounter(basis);
        Frequency freq = getFrequency(p);
        Period period(freq);
        double emr = rparams.getDoubleValue("emr");
        double callType = rparams.getDoubleValue("calltype");
        double dividendType = rparams.getDoubleValue("dividendtype");
        double treeType = rparams.getDoubleValue("treeType");        


        DividendSchedule dividendSchedule;
        try {
            RcppFrame frame(dividendFrame);
            std::vector<std::vector<ColDatum> >  table = frame.getTableData();
            int nrow = table.size();

            for (int row = 0; row < nrow; row++){
                QuantLib::Date d(dateFromR(table[row][0].getDateValue()));            
                double value = table[row][1].getDoubleValue();
                if (dividendType == 0) {
                    dividendSchedule.push_back(
                                               boost::shared_ptr<Dividend>(
                                                                           new FixedDividend(value, d)));                    
                }
                else {
                    dividendSchedule.push_back(
                                               boost::shared_ptr<Dividend>(new FractionalDividend(value, d)));                    
                }
            }
        }
        catch (std::exception& ex){}

        CallabilitySchedule callabilitySchedule;
        try {
            RcppFrame frame(callFrame);
            std::vector<std::vector<ColDatum> >  table = frame.getTableData();
            int nrow = table.size();

            for (int row = 0; row < nrow; row++){
                QuantLib::Date d(dateFromR(table[row][0].getDateValue()));            
                double price = table[row][1].getDoubleValue();
                if (callType == 0) {
                    callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                                  (new Callability(Callability::Price(price, 
                                                                                      Callability::Price::Clean),
                                                                   Callability::Call,d )));
                }
                else {
                    callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                                  (new Callability(Callability::Price(price, 
                                                                                      Callability::Price::Clean),
                                                                   Callability::Put,d )));                    
                }
            }
        }
        catch (std::exception& ex){}

        Calendar calendar=UnitedStates(UnitedStates::GovernmentBond);        
        QuantLib::Integer fixingDays = 1;
        Date todayDate = calendar.advance(settle, -fixingDays, Days);
        Settings::instance().evaluationDate() = todayDate;

        RelinkableHandle<Quote> underlying;
        RelinkableHandle<BlackVolTermStructure> volatility;
        RelinkableHandle<YieldTermStructure> riskFree;
        underlying.linkTo(boost::shared_ptr<Quote>(new SimpleQuote(price)));
        volatility.linkTo(flatVol(todayDate,  
                                  boost::shared_ptr<SimpleQuote>(new SimpleQuote(sigma)), 
                                  dayCounter));
        
        riskFree.linkTo(flatRate(todayDate, boost::shared_ptr<SimpleQuote>(new SimpleQuote(rff)), dayCounter));
        
        boost::shared_ptr<BlackScholesProcess> blackProcess;
        blackProcess = boost::shared_ptr<BlackScholesProcess>(
                                                                    new BlackScholesProcess(underlying, 
                                                                                                  riskFree, volatility));


        RelinkableHandle<Quote> creditSpread;
        creditSpread.linkTo(
                            boost::shared_ptr<Quote>(new SimpleQuote(spread)));

        boost::shared_ptr<Exercise> ex(
                                       new AmericanExercise(issue,
                                                            maturity));
        Size timeSteps = numSteps;
        boost::shared_ptr<PricingEngine> engine(
                                                new BinomialConvertibleEngine<CoxRossRubinstein>(blackProcess,
                                                                                                 timeSteps));

        
        Schedule sch(issue, maturity, period, calendar, Following,
                     Following, DateGeneration::Backward, false);
        std::vector<double> rates;
        rates.push_back(coupon);
        ConvertibleFixedCouponBond bond(ex, convRatio,
                                        dividendSchedule, 
                                        callabilitySchedule,
                                        creditSpread, 
                                        issue, 1, rates,
                                        dayCounter, sch, 100);
        
        bond.setPricingEngine(engine);

        RcppResultSet rs;
        rs.add("NPV", bond.NPV());
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
