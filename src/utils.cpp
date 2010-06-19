// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2002 - 2009  Dirk Eddelbuettel 
// Copyright (C) 2005 - 2006  Dominick Samperi
// Copyright (C) 2009 - 2010  Dirk Eddelbuettel and Khanh Nguyen
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

Option::Type getOptionType(const std::string &type) {
    Option::Type optionType;
    if (type=="call") {
        optionType = Option::Call;
    } else if (type=="put") {
        optionType = Option::Put;
    } else {
        throw std::range_error("Unknown option " + type);
    }
    return optionType;
}

// cf QuantLib-0.9.0/test-suite/europeanoption.cpp
boost::shared_ptr<VanillaOption>
makeOption(const boost::shared_ptr<StrikedTypePayoff>& payoff,
           const boost::shared_ptr<Exercise>& exercise,
           const boost::shared_ptr<Quote>& u,
           const boost::shared_ptr<YieldTermStructure>& q,
           const boost::shared_ptr<YieldTermStructure>& r,
           const boost::shared_ptr<BlackVolTermStructure>& vol,
           EngineType engineType,
           Size binomialSteps,
           Size samples) {

    boost::shared_ptr<GeneralizedBlackScholesProcess> stochProcess = makeProcess(u,q,r,vol);
    boost::shared_ptr<PricingEngine> engine;
    typedef boost::shared_ptr<PricingEngine> spPE; // shorthand used below

    switch (engineType) {
    case Analytic:
        engine = spPE(new AnalyticEuropeanEngine(stochProcess));
        break;
    case JR:
        engine = spPE(new BinomialVanillaEngine<JarrowRudd>(stochProcess, binomialSteps));
        break;
    case CRR:
        engine = spPE(new BinomialVanillaEngine<CoxRossRubinstein>(stochProcess, binomialSteps));
    case EQP:
        engine = spPE(new BinomialVanillaEngine<AdditiveEQPBinomialTree>(stochProcess, binomialSteps));
        break;
    case TGEO:
        engine = spPE(new BinomialVanillaEngine<Trigeorgis>(stochProcess, binomialSteps));
        break;
    case TIAN:
        engine = spPE(new BinomialVanillaEngine<Tian>(stochProcess, binomialSteps));
        break;
    case LR:
        engine = spPE(new BinomialVanillaEngine<LeisenReimer>(stochProcess, binomialSteps));
        break;
    case JOSHI:
        engine = spPE(new BinomialVanillaEngine<Joshi4>(stochProcess, binomialSteps));
        break;
    case FiniteDifferences:
        engine = spPE(new FDEuropeanEngine<CrankNicolson>(stochProcess, binomialSteps, samples));
        break;
    case Integral:
        engine = spPE(new IntegralEngine(stochProcess));
        break;
    case PseudoMonteCarlo:
        engine = MakeMCEuropeanEngine<PseudoRandom>(stochProcess)
            .withStepsPerYear(1)
            .withSamples(samples)
            .withSeed(42);
        break;
    case QuasiMonteCarlo:
        engine = MakeMCEuropeanEngine<LowDiscrepancy>(stochProcess)
            .withStepsPerYear(1)
            .withSamples(samples);
        break;
    default:
        QL_FAIL("Unknown engine type");
    }
    boost::shared_ptr<VanillaOption> option(new EuropeanOption(payoff, exercise));
    option->setPricingEngine(engine);
    return option;
}

// QuantLib option setup utils, copied from the test-suite sources

boost::shared_ptr<YieldTermStructure> buildTermStructure(SEXP params, SEXP tsQuotes, SEXP times){

    boost::shared_ptr<YieldTermStructure> curve;
    try {
      
        Rcpp::List rparam(params);
        Rcpp::List tslist(tsQuotes);
        Rcpp::CharacterVector tsnames = tslist.names();

        Date todaysDate( dateFromR( Rcpp::as<int>(rparam["tradeDate"])) ); 
        Date settlementDate( dateFromR( Rcpp::as<int>(rparam["settleDate"]) )); 
        // cout << "TradeDate: " << todaysDate << endl << "Settle: " << settlementDate << endl;
        
        RQLContext::instance().settleDate = settlementDate;
        Settings::instance().evaluationDate() = todaysDate;

        std::string firstQuoteName = Rcpp::as<std::string>(tsnames[0]);
        //double dt = rparam.getDoubleValue("dt");
        
        std::string interpWhat, interpHow;
        bool flatQuotes = true;
        if (firstQuoteName.compare("flat") != 0) { // Get interpolation method (not needed for "flat" case)
            interpWhat = Rcpp::as<std::string>(rparam["interpWhat"]);
            interpHow  = Rcpp::as<std::string>(rparam["interpHow"]);
            flatQuotes = false;
        }
        
        Calendar calendar = TARGET();
        RQLContext::instance().calendar = calendar;
        Integer fixingDays = 2;
        RQLContext::instance().fixingDays = fixingDays;

        // Any DayCounter would be fine;  ActualActual::ISDA ensures that 30 years is 30.0
        DayCounter termStructureDayCounter = ActualActual(ActualActual::ISDA);
        double tolerance = 1.0e-15;
        
        if (firstQuoteName.compare("flat") == 0) {	// Create a flat term structure.
            double rateQuote = tslist[0];
            boost::shared_ptr<Quote> flatRate(new SimpleQuote(rateQuote));
            boost::shared_ptr<FlatForward> 
                ts(new FlatForward(settlementDate, Handle<Quote>(flatRate), Actual365Fixed()));
            curve =  ts;
        } else {									// Build curve based on a set of observed rates and/or prices.
            std::vector<boost::shared_ptr<RateHelper> > curveInput;
            for (int i = 0; i < tslist.size(); i++) {
                std::string name = Rcpp::as<std::string>(tsnames[i]);
                double val = tslist[i];
                boost::shared_ptr<RateHelper> rh = ObservableDB::instance().getRateHelper(name, val);
                // edd 2009-11-01 FIXME NULL_RateHelper no longer builds under 0.9.9
                // if (rh == NULL_RateHelper)
                if (rh.get() == NULL)
                    throw std::range_error("Unknown rate in getRateHelper");
                curveInput.push_back(rh);
            }
            boost::shared_ptr<YieldTermStructure> ts =
                getTermStructure(interpWhat, interpHow, settlementDate, curveInput, 
                                 termStructureDayCounter, tolerance);
            curve = ts;
        }
        return curve;    
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return curve;
}

Schedule getSchedule(SEXP sch) {
   
    Rcpp::List rparam(sch);
    RcppDate iDate = RcppDate(Rcpp::as<int>(rparam["effectiveDate"]));
    QuantLib::Date effectiveDate(dateFromR(iDate));
    RcppDate mDate = RcppDate(Rcpp::as<int>(rparam["maturityDate"]));
    QuantLib::Date maturityDate(dateFromR(mDate));      
    double frequency = Rcpp::as<double>(rparam["period"]);
    std::string cal = Rcpp::as<std::string>(rparam["calendar"]);
    double businessDayConvention = Rcpp::as<double>(rparam["businessDayConvention"]);
    double terminationDateConvention = Rcpp::as<double>(rparam["terminationDateConvention"]);
    Calendar calendar = UnitedStates(UnitedStates::GovernmentBond);
    if (cal == "us"){
        calendar = UnitedStates(UnitedStates::GovernmentBond);
    } else if (cal == "uk"){
        calendar = UnitedKingdom(UnitedKingdom::Exchange);
    }
    BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);   
    BusinessDayConvention t_bdc = getBusinessDayConvention(terminationDateConvention);
    Schedule schedule(effectiveDate, maturityDate,
                      Period(getFrequency(frequency)),
                      calendar, bdc, t_bdc, 
                      DateGeneration::Backward, false);
    return schedule;
}

boost::shared_ptr<YieldTermStructure> rebuildCurveFromZeroRates(SEXP dateSexp, SEXP zeroSexp) {
    RcppDateVector rcppdates  = RcppDateVector(dateSexp);
    int n = rcppdates.size();
    std::vector<QuantLib::Date> dates(n);
    for (int i = 0;i<n; i++) {
        dates[i] = QuantLib::Date(dateFromR(rcppdates(i)));
    }
    Rcpp::NumericVector zeros(zeroSexp);    //extract coupon rates vector
    boost::shared_ptr<YieldTermStructure>  
        rebuilt_curve(new InterpolatedZeroCurve<LogLinear>(dates, 
                                                           Rcpp::as< std::vector<double> >(zeros), 
                                                           ActualActual()));
    return rebuilt_curve;
}

boost::shared_ptr<YieldTermStructure> getFlatCurve(SEXP flatcurve){
    Rcpp::List curve(flatcurve);
    Rate riskFreeRate = Rcpp::as<double>(curve["riskFreeRate"]);
    RcppDate today_Date = RcppDate(Rcpp::as<int>(curve["todayDate"]));       
    QuantLib::Date today(dateFromR(today_Date));
    
    boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(riskFreeRate));
    Settings::instance().evaluationDate() = today;
    return flatRate(today, rRate, Actual360());
}

boost::shared_ptr<IborIndex> getIborIndex(SEXP index, const Date today){
    Rcpp::List rparam(index);
    std::string type = Rcpp::as<std::string>(rparam["type"]);
    if (type == "USDLibor"){
        double riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        double period = Rcpp::as<double>(rparam["period"]);
        boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(riskFreeRate));
        Settings::instance().evaluationDate() = today;
        Handle<YieldTermStructure> curve(flatRate(today,rRate,Actual360()));
        boost::shared_ptr<IborIndex> iindex(new USDLibor(period * Months, curve));
        return iindex;
    }
    else return boost::shared_ptr<IborIndex>();
}

std::vector<double> getDoubleVector(SEXP vecSexp) {
    if (::Rf_length(vecSexp) == 0) {
        return(std::vector<double>());
    } else {
        return std::vector<double>( Rcpp::as<std::vector< double> >( Rcpp::NumericVector(vecSexp) ) );
    }
}

boost::shared_ptr<YieldTermStructure>
makeFlatCurve(const Date& today,
              const boost::shared_ptr<Quote>& forward,
              const DayCounter& dc) {
    return boost::shared_ptr<YieldTermStructure>(new FlatForward(today, Handle<Quote>(forward), dc));
}

boost::shared_ptr<YieldTermStructure>
flatRate(const Date& today,
         const boost::shared_ptr<Quote>& forward,
         const DayCounter& dc) {
    return boost::shared_ptr<YieldTermStructure>(new FlatForward(today, Handle<Quote>(forward), dc));
}
  
boost::shared_ptr<BlackVolTermStructure> 
makeFlatVolatility(const Date& today,
                   const boost::shared_ptr<Quote>& vol,
                   const DayCounter dc) {
    return boost::shared_ptr<BlackVolTermStructure>(
                                                    new BlackConstantVol(today, 
                                                                         NullCalendar(), 
                                                                         Handle<Quote>(vol), dc));
}

boost::shared_ptr<BlackVolTermStructure>
flatVol(const Date& today,
        const boost::shared_ptr<Quote>& vol,
        const DayCounter& dc) {
    return boost::shared_ptr<BlackVolTermStructure>(
           new BlackConstantVol(today, 
                                NullCalendar(), 
                                Handle<Quote>(vol), dc));
}

boost::shared_ptr<GeneralizedBlackScholesProcess>
makeProcess(const boost::shared_ptr<Quote>& u,
            const boost::shared_ptr<YieldTermStructure>& q,
            const boost::shared_ptr<YieldTermStructure>& r,
            const boost::shared_ptr<BlackVolTermStructure>& vol) {
    return boost::shared_ptr<BlackScholesMertonProcess>(new BlackScholesMertonProcess(Handle<Quote>(u),
                                                                                      Handle<YieldTermStructure>(q),
                                                                                      Handle<YieldTermStructure>(r),
                                                                                      Handle<BlackVolTermStructure>(vol)));
}

// R uses dates indexed to Jan 1, 1970. Rcpp uses an internal Julian Date representation,
// but Quantlib uses the 'spreadsheet' format indexed to 1905 so we need to adjust
int dateFromR(const RcppDate &d) {
    return(d.getJDN() - RcppDate::Jan1970Offset + RcppDate::QLtoJan1970Offset);
}

DayCounter getDayCounter(const double n){
    if (n==0) 
        return Actual360();
    else if (n==1) 
        return Actual365Fixed();
    else if (n==2) 
        return ActualActual();
    else if (n==3) 
        return Business252();
    else if (n==4) 
        return OneDayCounter();
    else if (n==5) 
        return SimpleDayCounter();
    else  
        return Thirty360();
}

BusinessDayConvention getBusinessDayConvention(const double n){
    if (n==0) 
        return Following;
    else if (n==1) 
        return ModifiedFollowing;
    else if (n==2) 
        return Preceding;
    else if (n==3) 
        return ModifiedPreceding;
    else  
        return Unadjusted;
}

Compounding getCompounding(const double n){
    if (n==0) 
        return Simple;
    else if (n==1) 
        return Compounded;
    else if (n==2) 
        return Continuous;
    else 
        return SimpleThenCompounded;
}

Frequency getFrequency(const double n){

    if (n==-1) 
        return NoFrequency;
    else if (n==0) 
        return Once;
    else if (n==1) 
        return Annual;
    else if (n==2) 
        return Semiannual;
    else if (n==3) 
        return EveryFourthMonth;
    else if (n==4) 
        return Quarterly;
    else if (n==6) 
        return Bimonthly;
    else if (n==12) 
        return Monthly;
    else if (n==13) 
        return EveryFourthWeek;
    else if (n==26) 
        return Biweekly;
    else if (n==52) 
        return Weekly;
    else if (n==365) 
        return Daily;
    else 
        return OtherFrequency;   
}

Period periodByTimeUnit(int length, std::string unit){
    TimeUnit tu = Years;
    if (unit=="Days") 
        tu=Days;
    if (unit=="Weeks") 
        tu=Weeks;
    if (unit=="Months") 
        tu=Months;
    return Period(length, tu);
}

TimeUnit getTimeUnit(const double n){
    if (n==0) 
        return Days;
    else if (n==1) 
        return Weeks;
    else if (n==2) 
        return Months;
    else return 
             Years;
}

DateGeneration::Rule getDateGenerationRule(const double n){
    if (n==0) 
        return DateGeneration::Backward;
    else if (n==1) 
        return DateGeneration::Forward;
    else if (n==2) 
        return DateGeneration::Zero;
    else if (n==3) 
        return DateGeneration::ThirdWednesday;
    else if (n==4) 
        return DateGeneration::Twentieth;
    else 
        return DateGeneration::TwentiethIMM;
}

boost::shared_ptr<IborIndex> buildIborIndex(std::string type,
                                            const Handle<YieldTermStructure>& iborStrc){
    if (type == "Euribor10M") 
        return boost::shared_ptr<IborIndex>(new Euribor10M(iborStrc));
    if (type == "Euribor11M") 
        return boost::shared_ptr<IborIndex>(new Euribor11M(iborStrc));
    if (type == "Euribor1M") 
        return boost::shared_ptr<IborIndex>(new Euribor1M(iborStrc));
    if (type == "Euribor1Y") 
        return boost::shared_ptr<IborIndex>(new Euribor1Y(iborStrc));
    if (type == "Euribor2M") 
        return boost::shared_ptr<IborIndex>(new Euribor2M(iborStrc));
    if (type == "Euribor2W") 
        return boost::shared_ptr<IborIndex>(new Euribor2W(iborStrc));
    if (type == "Euribor3M") 
        return boost::shared_ptr<IborIndex>(new Euribor3M(iborStrc));
    if (type == "Euribor3W") 
        return boost::shared_ptr<IborIndex>(new Euribor3W(iborStrc));
    if (type == "Euribor4M") 
        return boost::shared_ptr<IborIndex>(new Euribor4M(iborStrc));
    if (type == "Euribor5M") 
        return boost::shared_ptr<IborIndex>(new Euribor5M(iborStrc));
    if (type == "Euribor6M") 
        return boost::shared_ptr<IborIndex>(new Euribor6M(iborStrc));
    if (type == "Euribor7M") 
        return boost::shared_ptr<IborIndex>(new Euribor7M(iborStrc));
    if (type == "Euribor8M") 
        return boost::shared_ptr<IborIndex>(new Euribor8M(iborStrc));
    if (type == "Euribor9M") 
        return boost::shared_ptr<IborIndex>(new Euribor9M(iborStrc));
    if (type == "EuriborSW") 
        return boost::shared_ptr<IborIndex>(new EuriborSW(iborStrc));
    return boost::shared_ptr<IborIndex>();   
}

Rcpp::DataFrame getCashFlowDataFrame(const Leg &bondCashFlow) {
    //cashflow
    //int numCol = 2;
    //std::vector<std::string> colNames(numCol);
    //colNames[0] = "Date";
    //colNames[1] = "Amount";
    //RcppFrame frame(colNames);
    //Leg bondCashFlow = bond.cashflows();
    RcppDateVector dates(bondCashFlow.size());
    Rcpp::NumericVector amount(bondCashFlow.size());

    for (unsigned int i = 0; i< bondCashFlow.size(); i++){
        //std::vector<ColDatum> row(numCol);
        Date d = bondCashFlow[i]->date();
        //row[0].setDateValue(RcppDate(d.month(), d.dayOfMonth(), d.year()));
        //row[1].setDoubleValue(bondCashFlow[i]->amount());
        //frame.addRow(row);
        dates(i) = RcppDate(d.month(), d.dayOfMonth(), d.year());
        amount[i] = bondCashFlow[i]->amount();
    }
    return Rcpp::DataFrame::create(Rcpp::Named("Date") = dates,
                                   Rcpp::Named("Amount") = amount);
}


DividendSchedule getDividendSchedule(SEXP dividendScheduleFrame) {

    DividendSchedule dividendSchedule;
    try {
        //RcppFrame rcppDividendSchedule(dividendScheduleFrame);        
        //std::vector<std::vector<ColDatum> > table = rcppDividendSchedule.getTableData();
        //int nrow = table.size();
        //int ncol = table[0].size();
        Rcpp::DataFrame divScheDF(dividendScheduleFrame);
        Rcpp::CharacterVector s0v = divScheDF[0];
        Rcpp::NumericVector n1v = divScheDF[1];
        Rcpp::NumericVector n2v = divScheDF[2];
        Rcpp::NumericVector n3v = divScheDF[3];
        int nrow = s0v.size();

        for (int row=0; row<nrow; row++){          
            int type = (s0v[row] == "Fixed") ? 1 : 0; //  (table[row][0].getStringValue()=="Fixed") ? 1 : 0;
            double amount = n1v[row]; // table[row][1].getDoubleValue();
            double rate = n2v[row]; // table[row][2].getDoubleValue();
            QuantLib::Date d(dateFromR(n3v[row])); //table[row][3].getDateValue()));            
            if (type==1) {
                dividendSchedule.push_back(boost::shared_ptr<Dividend>(new FixedDividend(amount, d)));
            } else {
                dividendSchedule.push_back(boost::shared_ptr<Dividend>(new FractionalDividend(rate, amount, d)));
            }
        }
    } catch (std::exception& ex) { 
        forward_exception_to_r(ex); 
    }
    return dividendSchedule;
}

CallabilitySchedule getCallabilitySchedule(SEXP callabilityScheduleFrame) {

    CallabilitySchedule callabilitySchedule;

    try {
        // RcppFrame rcppCallabilitySchedule(callabilityScheduleFrame);
        // std::vector<std::vector<ColDatum> > table = rcppCallabilitySchedule.getTableData();
        // int nrow = table.size();
        Rcpp::DataFrame callScheDF(callabilityScheduleFrame);
        Rcpp::NumericVector n0v = callScheDF[0];
        Rcpp::CharacterVector s1v = callScheDF[1];
        Rcpp::NumericVector n2v = callScheDF[2];
        int nrow = n0v.size();
        for (int row=0; row<nrow; row++) {
            double price = n0v[row]; //table[row][0].getDoubleValue();
            int type = (s1v[row]=="P") ? 1 : 0;
            QuantLib::Date d(dateFromR(n2v[row]));
            if (type==1){
                callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                              (new Callability(Callability::Price(price, 
                                                                                  Callability::Price::Clean),
                                                               Callability::Put,d )));
            } else {
                callabilitySchedule.push_back(boost::shared_ptr<Callability>
                                              (new Callability(Callability::Price(price, 
                                                                                  Callability::Price::Clean),
                                                               Callability::Call,d )));
            }            
        }
    } catch (std::exception& ex){
        forward_exception_to_r(ex); 
    }
    return callabilitySchedule;
}
