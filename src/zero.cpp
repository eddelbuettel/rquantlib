#include "rquantlib.hpp"

RcppExport SEXP zeroprice(SEXP params){
    SEXP rl = R_NilValue;
    char* exceptionMesg = NULL;
    try {
        RcppParams rparam(params);

        double yield = rparam.getDoubleValue("Yield");
        QuantLib::Date maturity(dateFromR(rparam.getDateValue("Maturity")));
        QuantLib::Date settle(dateFromR(rparam.getDateValue("Settle")));

        Calendar calendar=UnitedStates(UnitedStates::GovernmentBond);
        QuantLib::Integer fixingDays = 2;
        Date todaysDate = calendar.advance(settle, -fixingDays, Days);
        Settings::instance().evaluationDate() = todaysDate;


        double period = rparam.getDoubleValue("Period");
        double basis = rparam.getDoubleValue("Basis");
        DayCounter dayCounter = getDayCounter(basis);
        Frequency freq = getFrequency(period);
        Period p(freq);
        double EMR = rparam.getDoubleValue("EMR");


        ZeroCouponBond bond(1, calendar, 
                            100, maturity, Unadjusted, 
                            100.0, settle);

        double price = bond.cleanPrice(yield, dayCounter, Compounded, freq);
        
        RcppResultSet rs;
        rs.add("Price", price);
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

RcppExport SEXP zeroyield(SEXP params){
    SEXP rl = R_NilValue;
    char* exceptionMesg = NULL;
    try {
        RcppParams rparam(params);

        double price = rparam.getDoubleValue("Price");
        QuantLib::Date maturity(dateFromR(rparam.getDateValue("Maturity")));
        QuantLib::Date settle(dateFromR(rparam.getDateValue("Settle")));

        Calendar calendar=UnitedStates(UnitedStates::GovernmentBond);
        QuantLib::Integer fixingDays = 2;
        Date todaysDate = calendar.advance(settle, -fixingDays, Days);
        Settings::instance().evaluationDate() = todaysDate;

        double period = rparam.getDoubleValue("Period");
        double basis = rparam.getDoubleValue("Basis");
        DayCounter dayCounter = getDayCounter(basis);
        Frequency freq = getFrequency(period);
        Period p(freq);
        double EMR = rparam.getDoubleValue("EMR");


        ZeroCouponBond bond(1, calendar, 100, maturity, Unadjusted, 100.0, settle);

        double yield = bond.yield(price, dayCounter, Compounded, freq);
        
        RcppResultSet rs;
        rs.add("Yield", yield);
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

RcppExport SEXP zbtyield(SEXP MatVec, SEXP BondMat, 
                         SEXP yieldVec, SEXP SettlVec,
                         SEXP cpVec, SEXP param){
    SEXP rl = R_NilValue;
    char* exceptionMesg = NULL;
    try {   


        RcppParams rparam(param);
        double oc = rparam.getDoubleValue("OC");

        RcppDateVector rmat(MatVec);
        RcppDateVector rsettle(SettlVec);
        RcppVector<double> ryields(yieldVec);
        RcppVector<double> rcp(cpVec);
        RcppMatrix<double> rbondmat(BondMat);
        std::cout << "read done";
        int n = rmat.size();
        std::vector<QuantLib::Date> MatDates(rmat.size());
        for (int i = 0;i<n;i++){
            QuantLib::Date day(dateFromR(rmat(i)));
            MatDates[i] = day;            
        }

        std::vector<QuantLib::Date> SettleDates(rsettle.size());
        for (int i = 0;i<n;i++){
            QuantLib::Date day(dateFromR(rsettle(i)) );
            SettleDates[i] = day;            
        }

        ;
        std::vector<double> cleanPrice(rcp.stlVector());        
        std::vector<double> yields(ryields.stlVector());
        std::vector<std::vector<double> > bondparam(rbondmat.stlMatrix());

        //setting up the bonds
        const Size numberOfBonds = n;



        std::vector<boost::shared_ptr<RateHelper> > instruments;

        std::vector< boost::shared_ptr<SimpleQuote> > quote;
        for (Size i=0; i<numberOfBonds; i++) {
            boost::shared_ptr<SimpleQuote> cp(new SimpleQuote(cleanPrice[i]));
            quote.push_back(cp);
        }

        RelinkableHandle<Quote> quoteHandle[numberOfBonds];
        for (Size i=0; i<numberOfBonds; i++) {
            quoteHandle[i].linkTo(quote[i]);
        }

        Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
        Date todaysDate = calendar.advance(SettleDates[0], -2, Days);
        // nothing to do with Date::todaysDate
        Settings::instance().evaluationDate() = todaysDate;
        Period p(getFrequency(2));
        double faceAmount = 100;
        DayCounter dayCounter = getDayCounter(2);
        bool emr = true;
        for (Size  j = 0; j< numberOfBonds;j++){

            if (rbondmat.getDim2() > 1){
                p = Period(getFrequency(bondparam[j][2]));                           
                faceAmount = bondparam[j][1];
                dayCounter = getDayCounter(3);
                emr = (bondparam[j][4]==0) ? false : true;
            }
            
            Schedule schedule(SettleDates[j], MatDates[j],p, calendar,
                              Unadjusted, Unadjusted,
                              DateGeneration::Backward, emr);
            boost::shared_ptr<FixedRateBondHelper> 
                helper(
                       new FixedRateBondHelper(quoteHandle[j],
                                               1, faceAmount, 
                                               schedule,
                                               std::vector<Rate>(1,bondparam[j][0]),
                                               dayCounter, 
                                               Unadjusted, 100, SettleDates[j]
                                               )
                       );
            instruments.push_back(helper);
        }
        
        bool constrainAtZero = true;
        Real tolerance = 1.0e-10;
        Size max = 5000;

        /*
	boost::shared_ptr<YieldTermStructure> curve;
        NelsonSiegelFitting nelsonSiegel;
        boost::shared_ptr<FittedBondDiscountCurve> ts3 (
                                                        new FittedBondDiscountCurve(1,
                                                                                    calendar,
                                                                                    instruments,
                                                                                    dayCounter,
                                                                                    nelsonSiegel,
                                                                                    tolerance,
                                                                                    max));
        curve = ts3;
        */

        boost::shared_ptr<YieldTermStructure> curve (
                                                     new PiecewiseYieldCurve<ZeroYield,Cubic>(1,
                                                                                              calendar,
                                                                                              instruments,                                                                                                                               
                                                                                              dayCounter));
        std::cout << "here";
        int numCol = 2;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "date";
        colNames[1] = "zeroRates";
        
        RcppFrame frame(colNames);
        Date current = SettleDates[0];
        int n1 = curve->maxDate() - SettleDates[0];
        for (int i = 0; i<numberOfBonds;i++){
            std::vector<ColDatum> row(numCol);
            Date d = MatDates[i];
            row[0].setDateValue(RcppDate(d.month(),
                                         d.dayOfMonth(),
                                         d.year()));
            
            double zrate = curve->zeroRate(d, ActualActual(),
                                            Simple);
            row[1].setDoubleValue(zrate);                        



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
