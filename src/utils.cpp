// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2002 - 2012  Dirk Eddelbuettel 
// Copyright (C) 2005 - 2006  Dominick Samperi
// Copyright (C) 2009 - 2012  Dirk Eddelbuettel and Khanh Nguyen
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

QuantLib::Option::Type getOptionType(const std::string &type) {
    QuantLib::Option::Type optionType;
    if (type=="call") {
        optionType = QuantLib::Option::Call;
    } else if (type=="put") {
        optionType = QuantLib::Option::Put;
    } else {
        throw std::range_error("Unknown option " + type);
    }
    return optionType;
}

// cf QuantLib-0.9.0/test-suite/europeanoption.cpp
boost::shared_ptr<QuantLib::VanillaOption>
makeOption(const boost::shared_ptr<QuantLib::StrikedTypePayoff>& payoff,
           const boost::shared_ptr<QuantLib::Exercise>& exercise,
           const boost::shared_ptr<QuantLib::Quote>& u,
           const boost::shared_ptr<QuantLib::YieldTermStructure>& q,
           const boost::shared_ptr<QuantLib::YieldTermStructure>& r,
           const boost::shared_ptr<QuantLib::BlackVolTermStructure>& vol,
           EngineType engineType,
           QuantLib::Size binomialSteps,
           QuantLib::Size samples) {

    boost::shared_ptr<QuantLib::GeneralizedBlackScholesProcess> stochProcess = makeProcess(u,q,r,vol);
    boost::shared_ptr<QuantLib::PricingEngine> engine;
    typedef boost::shared_ptr<QuantLib::PricingEngine> spPE; // shorthand used below

    switch (engineType) {
    case Analytic:
        engine = spPE(new QuantLib::AnalyticEuropeanEngine(stochProcess));
        break;
    case JR:
        engine = spPE(new QuantLib::BinomialVanillaEngine<QuantLib::JarrowRudd>(stochProcess, binomialSteps));
        break;
    case CRR:
        engine = spPE(new QuantLib::BinomialVanillaEngine<QuantLib::CoxRossRubinstein>(stochProcess, binomialSteps));
        break;
    case EQP:
        engine = spPE(new QuantLib::BinomialVanillaEngine<QuantLib::AdditiveEQPBinomialTree>(stochProcess, binomialSteps));
        break;
    case TGEO:
        engine = spPE(new QuantLib::BinomialVanillaEngine<QuantLib::Trigeorgis>(stochProcess, binomialSteps));
        break;
    case TIAN:
        engine = spPE(new QuantLib::BinomialVanillaEngine<QuantLib::Tian>(stochProcess, binomialSteps));
        break;
    case LR:
        engine = spPE(new QuantLib::BinomialVanillaEngine<QuantLib::LeisenReimer>(stochProcess, binomialSteps));
        break;
    case JOSHI:
        engine = spPE(new QuantLib::BinomialVanillaEngine<QuantLib::Joshi4>(stochProcess, binomialSteps));
        break;
    case FiniteDifferences:
        engine = spPE(new QuantLib::FDEuropeanEngine<QuantLib::CrankNicolson>(stochProcess, binomialSteps, samples));
        break;
    case Integral:
        engine = spPE(new QuantLib::IntegralEngine(stochProcess));
        break;
    case PseudoMonteCarlo:
        engine = QuantLib::MakeMCEuropeanEngine<QuantLib::PseudoRandom>(stochProcess)
            .withStepsPerYear(1)
            .withSamples(samples)
            .withSeed(42);
        break;
    case QuasiMonteCarlo:
        engine = QuantLib::MakeMCEuropeanEngine<QuantLib::LowDiscrepancy>(stochProcess)
            .withStepsPerYear(1)
            .withSamples(samples);
        break;
    default:
        QL_FAIL("Unknown engine type");
    }
    boost::shared_ptr<QuantLib::VanillaOption> option(new QuantLib::EuropeanOption(payoff, exercise));
    option->setPricingEngine(engine);
    return option;
}

// QuantLib option setup utils, copied from the test-suite sources

boost::shared_ptr<QuantLib::YieldTermStructure> buildTermStructure(SEXP params, SEXP tsQuotes, SEXP times){

    boost::shared_ptr<QuantLib::YieldTermStructure> curve;
    try {
      
        Rcpp::List rparam(params);
        Rcpp::List tslist(tsQuotes);
        Rcpp::CharacterVector tsnames = tslist.names();

        QuantLib::Date todaysDate( dateFromR( Rcpp::as<Rcpp::Date>(rparam["tradeDate"]))); 
        QuantLib::Date settlementDate( dateFromR( Rcpp::as<Rcpp::Date>(rparam["settleDate"]))); 
        // cout << "TradeDate: " << todaysDate << endl << "Settle: " << settlementDate << endl;
        
        RQLContext::instance().settleDate = settlementDate;
        QuantLib::Settings::instance().evaluationDate() = todaysDate;

        std::string firstQuoteName = Rcpp::as<std::string>(tsnames[0]);
        //double dt = rparam.getDoubleValue("dt");
        
        std::string interpWhat, interpHow;
        if (firstQuoteName.compare("flat") != 0) { // Get interpolation method (not needed for "flat" case)
            interpWhat = Rcpp::as<std::string>(rparam["interpWhat"]);
            interpHow  = Rcpp::as<std::string>(rparam["interpHow"]);
        }
        
        // initialise from the singleton instance
        QuantLib::Calendar calendar = RQLContext::instance().calendar;
        //Integer fixingDays = RQLContext::instance().fixingDays;

        // Any DayCounter would be fine;  ActualActual::ISDA ensures that 30 years is 30.0
        QuantLib::DayCounter termStructureDayCounter = QuantLib::ActualActual(QuantLib::ActualActual::ISDA);
        double tolerance = 1.0e-15;
        
        if (firstQuoteName.compare("flat") == 0) {	// Create a flat term structure.
            double rateQuote = Rcpp::as<double>(tslist[0]);
            boost::shared_ptr<QuantLib::Quote> flatRate(new QuantLib::SimpleQuote(rateQuote));
            boost::shared_ptr<QuantLib::FlatForward> 
                ts(new QuantLib::FlatForward(settlementDate, QuantLib::Handle<QuantLib::Quote>(flatRate), QuantLib::Actual365Fixed()));
            curve =  ts;
        } else {									// Build curve based on a set of observed rates and/or prices.
            std::vector<boost::shared_ptr<QuantLib::RateHelper> > curveInput;
            for (int i = 0; i < tslist.size(); i++) {
                std::string name = Rcpp::as<std::string>(tsnames[i]);
                double val = Rcpp::as<double>(tslist[i]);
                boost::shared_ptr<QuantLib::RateHelper> rh = ObservableDB::instance().getRateHelper(name, val);
                // edd 2009-11-01 FIXME NULL_RateHelper no longer builds under 0.9.9
                // if (rh == NULL_RateHelper)
                if (rh.get() == NULL)
                    throw std::range_error("Unknown rate in getRateHelper");
                curveInput.push_back(rh);
            }
            boost::shared_ptr<QuantLib::YieldTermStructure> ts =
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

QuantLib::Schedule getSchedule(SEXP sch) {
   
    Rcpp::List rparam(sch);
    QuantLib::Date effectiveDate(dateFromR( Rcpp::as<Rcpp::Date>(rparam["effectiveDate"]) ));
    QuantLib::Date maturityDate(dateFromR( Rcpp::as<Rcpp::Date>(rparam["maturityDate"]) ));      
    double frequency = Rcpp::as<double>(rparam["period"]);
    std::string cal = Rcpp::as<std::string>(rparam["calendar"]);
    double businessDayConvention = Rcpp::as<double>(rparam["businessDayConvention"]);
    double terminationDateConvention = Rcpp::as<double>(rparam["terminationDateConvention"]);
    QuantLib::Calendar calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    if (cal == "us"){
        calendar = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    } else if (cal == "uk"){
        calendar = QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
    }
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(businessDayConvention);   
    QuantLib::BusinessDayConvention t_bdc = getBusinessDayConvention(terminationDateConvention);
    QuantLib::Schedule schedule(effectiveDate, maturityDate,
                                QuantLib::Period(getFrequency(frequency)),
                                calendar, bdc, t_bdc, 
                                QuantLib::DateGeneration::Backward, false);
    return schedule;
}

boost::shared_ptr<QuantLib::YieldTermStructure> rebuildCurveFromZeroRates(SEXP dateSexp, SEXP zeroSexp) {
    Rcpp::DateVector rcppdates  = Rcpp::DateVector(dateSexp);
    int n = rcppdates.size();
    std::vector<QuantLib::Date> dates(n);
    for (int i = 0;i<n; i++) {
        dates[i] = QuantLib::Date(dateFromR(rcppdates[i]));
    }
    Rcpp::NumericVector zeros(zeroSexp);    //extract coupon rates vector
    boost::shared_ptr<QuantLib::YieldTermStructure>  
        rebuilt_curve(new QuantLib::InterpolatedZeroCurve<QuantLib::LogLinear>(dates, 
                                                                               Rcpp::as< std::vector<double> >(zeros), 
                                                                               QuantLib::ActualActual()));
    return rebuilt_curve;
}

boost::shared_ptr<QuantLib::YieldTermStructure> getFlatCurve(SEXP flatcurve){
    Rcpp::List curve(flatcurve);
    QuantLib::Rate riskFreeRate = Rcpp::as<double>(curve["riskFreeRate"]);
    QuantLib::Date today(dateFromR( Rcpp::as<Rcpp::Date>(curve["todayDate"])));       
    boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(riskFreeRate));
    QuantLib::Settings::instance().evaluationDate() = today;
    return flatRate(today, rRate, QuantLib::Actual360());
}

boost::shared_ptr<QuantLib::IborIndex> getIborIndex(SEXP index, const QuantLib::Date today){
    Rcpp::List rparam(index);
    std::string type = Rcpp::as<std::string>(rparam["type"]);
    if (type == "USDLibor"){
        double riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        double period = Rcpp::as<double>(rparam["period"]);
        boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(riskFreeRate));
        QuantLib::Settings::instance().evaluationDate() = today;
        QuantLib::Handle<QuantLib::YieldTermStructure> curve(flatRate(today, rRate, QuantLib::Actual360()));
        boost::shared_ptr<QuantLib::IborIndex> iindex(new QuantLib::USDLibor(period * QuantLib::Months, curve));
        return iindex;
    }
    else return boost::shared_ptr<QuantLib::IborIndex>();
}

std::vector<double> getDoubleVector(SEXP vecSexp) {
    if (::Rf_length(vecSexp) == 0) {
        return(std::vector<double>());
    } else {
        return std::vector<double>( Rcpp::as<std::vector< double> >( Rcpp::NumericVector(vecSexp) ) );
    }
}

boost::shared_ptr<QuantLib::YieldTermStructure>
makeFlatCurve(const QuantLib::Date& today,
              const boost::shared_ptr<QuantLib::Quote>& forward,
              const QuantLib::DayCounter& dc) {
    return boost::shared_ptr<QuantLib::YieldTermStructure>(new QuantLib::FlatForward(today, QuantLib::Handle<QuantLib::Quote>(forward), dc));
}

boost::shared_ptr<QuantLib::YieldTermStructure>
flatRate(const QuantLib::Date& today,
         const boost::shared_ptr<QuantLib::Quote>& forward,
         const QuantLib::DayCounter& dc) {
    return boost::shared_ptr<QuantLib::YieldTermStructure>(new QuantLib::FlatForward(today, QuantLib::Handle<QuantLib::Quote>(forward), dc));
}
  
boost::shared_ptr<QuantLib::BlackVolTermStructure> 
makeFlatVolatility(const QuantLib::Date& today,
                   const boost::shared_ptr<QuantLib::Quote>& vol,
                   const QuantLib::DayCounter dc) {
    return boost::shared_ptr<QuantLib::BlackVolTermStructure>(new QuantLib::BlackConstantVol(today, 
                                                                                             QuantLib::NullCalendar(), 
                                                                                             QuantLib::Handle<QuantLib::Quote>(vol), dc));
}

boost::shared_ptr<QuantLib::BlackVolTermStructure>
flatVol(const QuantLib::Date& today,
        const boost::shared_ptr<QuantLib::Quote>& vol,
        const QuantLib::DayCounter& dc) {
    return boost::shared_ptr<QuantLib::BlackVolTermStructure>(
           new QuantLib::BlackConstantVol(today, 
                                          QuantLib::NullCalendar(), 
                                          QuantLib::Handle<QuantLib::Quote>(vol), dc));
}

typedef QuantLib::BlackScholesMertonProcess BSMProcess; // shortcut
boost::shared_ptr<QuantLib::GeneralizedBlackScholesProcess>
makeProcess(const boost::shared_ptr<QuantLib::Quote>& u,
            const boost::shared_ptr<QuantLib::YieldTermStructure>& q,
            const boost::shared_ptr<QuantLib::YieldTermStructure>& r,
            const boost::shared_ptr<QuantLib::BlackVolTermStructure>& vol) {
    return boost::shared_ptr<BSMProcess>(new BSMProcess(QuantLib::Handle<QuantLib::Quote>(u),
                                                        QuantLib::Handle<QuantLib::YieldTermStructure>(q),
                                                        QuantLib::Handle<QuantLib::YieldTermStructure>(r),
                                                        QuantLib::Handle<QuantLib::BlackVolTermStructure>(vol)));
}

// R uses dates indexed to Jan 1, 1970. RcppDate uses an internal Julian Date representation,
// but Quantlib uses the 'spreadsheet' format indexed to 1905 so we need to adjust
// int dateFromR(const RcppDate &d) {
//     return(d.getJDN() - RcppDate::Jan1970Offset + RcppDate::QLtoJan1970Offset);
// }

static const unsigned int QLtoJan1970Offset = 25569;  	// Offset between R / Unix epoch 

// R and Rcpp::Date use the same 'days since epoch' representation; QL uses Excel style
int dateFromR(const Rcpp::Date &d) {
    return(d.getDate() + QLtoJan1970Offset);
}

QuantLib::DayCounter getDayCounter(const double n){
    if (n==0) 
        return QuantLib::Actual360();
    else if (n==1) 
        return QuantLib::Actual365Fixed();
    else if (n==2) 
        return QuantLib::ActualActual();
    else if (n==3) 
        return QuantLib::Business252();
    else if (n==4) 
        return QuantLib::OneDayCounter();
    else if (n==5) 
        return QuantLib::SimpleDayCounter();
    else  
        return QuantLib::Thirty360();
}

QuantLib::BusinessDayConvention getBusinessDayConvention(const double n){
    if (n==0) 
        return QuantLib::Following;
    else if (n==1) 
        return QuantLib::ModifiedFollowing;
    else if (n==2) 
        return QuantLib::Preceding;
    else if (n==3) 
        return QuantLib::ModifiedPreceding;
    else  
        return QuantLib::Unadjusted;
}

QuantLib::Compounding getCompounding(const double n){
    if (n==0) 
        return QuantLib::Simple;
    else if (n==1) 
        return QuantLib::Compounded;
    else if (n==2) 
        return QuantLib::Continuous;
    else 
        return QuantLib::SimpleThenCompounded;
}

QuantLib::Frequency getFrequency(const double n){

    if (n==-1) 
        return QuantLib::NoFrequency;
    else if (n==0) 
        return QuantLib::Once;
    else if (n==1) 
        return QuantLib::Annual;
    else if (n==2) 
        return QuantLib::Semiannual;
    else if (n==3) 
        return QuantLib::EveryFourthMonth;
    else if (n==4) 
        return QuantLib::Quarterly;
    else if (n==6) 
        return QuantLib::Bimonthly;
    else if (n==12) 
        return QuantLib::Monthly;
    else if (n==13) 
        return QuantLib::EveryFourthWeek;
    else if (n==26) 
        return QuantLib::Biweekly;
    else if (n==52) 
        return QuantLib::Weekly;
    else if (n==365) 
        return QuantLib::Daily;
    else 
        return QuantLib::OtherFrequency;   
}

QuantLib::Period periodByTimeUnit(int length, std::string unit){
    QuantLib::TimeUnit tu = QuantLib::Years;
    if (unit=="Days") 
        tu = QuantLib::Days;
    if (unit=="Weeks") 
        tu = QuantLib::Weeks;
    if (unit=="Months") 
        tu = QuantLib::Months;
    return QuantLib::Period(length, tu);
}

QuantLib::TimeUnit getTimeUnit(const double n){
    if (n==0) 
        return QuantLib::Days;
    else if (n==1) 
        return QuantLib::Weeks;
    else if (n==2) 
        return QuantLib::Months;
    else 
        return QuantLib::Years;
}

QuantLib::DateGeneration::Rule getDateGenerationRule(const double n){
    if (n==0) 
        return QuantLib::DateGeneration::Backward;
    else if (n==1) 
        return QuantLib::DateGeneration::Forward;
    else if (n==2) 
        return QuantLib::DateGeneration::Zero;
    else if (n==3) 
        return QuantLib::DateGeneration::ThirdWednesday;
    else if (n==4) 
        return QuantLib::DateGeneration::Twentieth;
    else 
        return QuantLib::DateGeneration::TwentiethIMM;
}

boost::shared_ptr<QuantLib::IborIndex> buildIborIndex(std::string type,
                                                      const QuantLib::Handle<QuantLib::YieldTermStructure>& iborStrc){
    if (type == "Euribor10M") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor10M(iborStrc));
    if (type == "Euribor11M") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor11M(iborStrc));
    if (type == "Euribor1M") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor1M(iborStrc));
    if (type == "Euribor1Y") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor1Y(iborStrc));
    if (type == "Euribor2M") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor2M(iborStrc));
    if (type == "Euribor2W") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor2W(iborStrc));
    if (type == "Euribor3M") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor3M(iborStrc));
    if (type == "Euribor3W") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor3W(iborStrc));
    if (type == "Euribor4M") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor4M(iborStrc));
    if (type == "Euribor5M") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor5M(iborStrc));
    if (type == "Euribor6M") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor6M(iborStrc));
    if (type == "Euribor7M") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor7M(iborStrc));
    if (type == "Euribor8M") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor8M(iborStrc));
    if (type == "Euribor9M") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor9M(iborStrc));
    if (type == "EuriborSW") 
        return boost::shared_ptr<QuantLib::IborIndex>(new QuantLib::EuriborSW(iborStrc));
    return boost::shared_ptr<QuantLib::IborIndex>();   
}

Rcpp::DataFrame getCashFlowDataFrame(const QuantLib::Leg &bondCashFlow) {
    Rcpp::DateVector dates(bondCashFlow.size());
    Rcpp::NumericVector amount(bondCashFlow.size());

    for (unsigned int i = 0; i< bondCashFlow.size(); i++){
        QuantLib::Date d = bondCashFlow[i]->date();
        dates[i] = Rcpp::Date(d.month(), d.dayOfMonth(), d.year());
        amount[i] = bondCashFlow[i]->amount();
    }
    return Rcpp::DataFrame::create(Rcpp::Named("Date") = dates,
                                   Rcpp::Named("Amount") = amount);
}


QuantLib::DividendSchedule getDividendSchedule(SEXP dividendScheduleFrame) {

    QuantLib::DividendSchedule dividendSchedule;
    try {
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
            QuantLib::Date d(dateFromR(Rcpp::Date(n3v[row]))); //table[row][3].getDateValue()));            
            if (type==1) {
                dividendSchedule.push_back(boost::shared_ptr<QuantLib::Dividend>(new QuantLib::FixedDividend(amount, d)));
            } else {
                dividendSchedule.push_back(boost::shared_ptr<QuantLib::Dividend>(new QuantLib::FractionalDividend(rate, amount, d)));
            }
        }
    } catch (std::exception& ex) { 
        forward_exception_to_r(ex); 
    }
    return dividendSchedule;
}

QuantLib::CallabilitySchedule getCallabilitySchedule(SEXP callabilityScheduleFrame) {

    QuantLib::CallabilitySchedule callabilitySchedule;

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
            QuantLib::Date d(dateFromR(Rcpp::Date(n2v[row])));
            if (type==1){
                callabilitySchedule.push_back(boost::shared_ptr<QuantLib::Callability>
                                              (new QuantLib::Callability(QuantLib::Callability::Price(price, 
                                                                                                      QuantLib::Callability::Price::Clean),
                                                                         QuantLib::Callability::Put,d )));
            } else {
                callabilitySchedule.push_back(boost::shared_ptr<QuantLib::Callability>
                                              (new QuantLib::Callability(QuantLib::Callability::Price(price, 
                                                                                                      QuantLib::Callability::Price::Clean),
                                                                         QuantLib::Callability::Call,d )));
            }            
        }
    } catch (std::exception& ex){
        forward_exception_to_r(ex); 
    }
    return callabilitySchedule;
}
