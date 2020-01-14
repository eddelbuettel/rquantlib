
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2002 - 2020  Dirk Eddelbuettel
//  Copyright (C) 2005 - 2006  Dominick Samperi
//  Copyright (C) 2009 - 2012  Dirk Eddelbuettel and Khanh Nguyen
//
//  RQuantLib is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  RQuantLib is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with RQuantLib.  If not, see <http://www.gnu.org/licenses/>.

#include <rquantlib_internal.h>

// [[Rcpp::interfaces(r, cpp)]]

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
QuantLib::ext::shared_ptr<QuantLib::VanillaOption>
makeOption(const QuantLib::ext::shared_ptr<QuantLib::StrikedTypePayoff>& payoff,
           const QuantLib::ext::shared_ptr<QuantLib::Exercise>& exercise,
           const QuantLib::ext::shared_ptr<QuantLib::Quote>& u,
           const QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>& q,
           const QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>& r,
           const QuantLib::ext::shared_ptr<QuantLib::BlackVolTermStructure>& vol,
           EngineType engineType,
           QuantLib::Size binomialSteps,
           QuantLib::Size samples) {

    QuantLib::ext::shared_ptr<QuantLib::GeneralizedBlackScholesProcess> stochProcess = makeProcess(u,q,r,vol);
    QuantLib::ext::shared_ptr<QuantLib::PricingEngine> engine;
    typedef QuantLib::ext::shared_ptr<QuantLib::PricingEngine> spPE; // shorthand used below

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
        engine = spPE(new QuantLib::FdBlackScholesVanillaEngine(stochProcess, binomialSteps, samples));
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
    QuantLib::ext::shared_ptr<QuantLib::VanillaOption> option(new QuantLib::EuropeanOption(payoff, exercise));
    option->setPricingEngine(engine);
    return option;
}

// QuantLib option setup utils, copied from the test-suite sources
QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> buildTermStructure(Rcpp::List rparam, Rcpp::List tslist) {

    QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> curve;
    try {

        Rcpp::CharacterVector tsnames = tslist.names();
        QuantLib::Date todaysDate(Rcpp::as<QuantLib::Date>(rparam["tradeDate"]));
        QuantLib::Date settlementDate(Rcpp::as<QuantLib::Date>(rparam["settleDate"]));
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
            QuantLib::ext::shared_ptr<QuantLib::Quote> flatRate(new QuantLib::SimpleQuote(rateQuote));
            QuantLib::ext::shared_ptr<QuantLib::FlatForward>
                ts(new QuantLib::FlatForward(settlementDate, QuantLib::Handle<QuantLib::Quote>(flatRate), QuantLib::Actual365Fixed()));
            curve =  ts;
        } else {									// Build curve based on a set of observed rates and/or prices.
            std::vector<QuantLib::ext::shared_ptr<QuantLib::RateHelper> > curveInput;
            for (int i = 0; i < tslist.size(); i++) {
                std::string name = Rcpp::as<std::string>(tsnames[i]);
                double val = Rcpp::as<double>(tslist[i]);
                QuantLib::ext::shared_ptr<QuantLib::RateHelper> rh = ObservableDB::instance().getRateHelper(name, val);
                // edd 2009-11-01 FIXME NULL_RateHelper no longer builds under 0.9.9
                // if (rh == NULL_RateHelper)
                if (rh.get() == NULL)
                    throw std::range_error("Unknown rate in getRateHelper");
                curveInput.push_back(rh);
            }
            QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> ts =
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

QuantLib::Schedule getSchedule(Rcpp::List rparam) {

    QuantLib::Date effectiveDate(Rcpp::as<QuantLib::Date>(rparam["effectiveDate"]));
    QuantLib::Date maturityDate(Rcpp::as<QuantLib::Date>(rparam["maturityDate"]));
    QuantLib::Period period = QuantLib::Period(getFrequency(Rcpp::as<double>(rparam["period"])));
    std::string cal = Rcpp::as<std::string>(rparam["calendar"]);
    QuantLib::Calendar calendar;
    if(!cal.empty()) {
        QuantLib::ext::shared_ptr<QuantLib::Calendar> p = getCalendar(cal);
        calendar = *p;
    }
    QuantLib::BusinessDayConvention businessDayConvention =
        getBusinessDayConvention(Rcpp::as<double>(rparam["businessDayConvention"]));
    QuantLib::BusinessDayConvention terminationDateConvention =
        getBusinessDayConvention(Rcpp::as<double>(rparam["terminationDateConvention"]));
    // keep these default values for the last two parameters for backward compatibility
    // (although in QuantLib::schedule they have no default values)
    QuantLib::DateGeneration::Rule dateGeneration = QuantLib::DateGeneration::Backward;
    if(rparam.containsElementNamed("dateGeneration") ) {
        dateGeneration = getDateGenerationRule(Rcpp::as<double>(rparam["dateGeneration"]));
    }
    bool endOfMonth = false;
    if(rparam.containsElementNamed("endOfMonth") ) {
        endOfMonth = (Rcpp::as<double>(rparam["endOfMonth"]) == 1) ? true : false;
    }

    QuantLib::Schedule schedule(effectiveDate,
                                maturityDate,
                                period,
                                calendar,
                                businessDayConvention,
                                terminationDateConvention,
                                dateGeneration,
                                endOfMonth);
    return schedule;
}

QuantLib::ext::shared_ptr<QuantLib::FixedRateBond> getFixedRateBond(
    Rcpp::List bondparam, std::vector<double> ratesVec, Rcpp::List scheduleparam) {

    // get bond parameters
    double settlementDays = Rcpp::as<double>(bondparam["settlementDays"]);
    double faceAmount = Rcpp::as<double>(bondparam["faceAmount"]);
    QuantLib::DayCounter accrualDayCounter =
        getDayCounter(Rcpp::as<double>(bondparam["dayCounter"]));
    QuantLib::BusinessDayConvention paymentConvention = QuantLib::Following;
    if(bondparam.containsElementNamed("paymentConvention") ) {
        paymentConvention = getBusinessDayConvention(Rcpp::as<double>(bondparam["paymentConvention"]));
    }
    double redemption = 100.0;
    if(bondparam.containsElementNamed("redemption") ) {
        redemption = Rcpp::as<double>(bondparam["redemption"]);
    }
    QuantLib::Date issueDate;
    if(bondparam.containsElementNamed("issueDate") ) {
        issueDate = Rcpp::as<QuantLib::Date>(bondparam["issueDate"]);
    }
    QuantLib::Calendar paymentCalendar;
    if(bondparam.containsElementNamed("paymentCalendar") ) {
        QuantLib::ext::shared_ptr<QuantLib::Calendar> p = getCalendar(Rcpp::as<std::string>(bondparam["paymentCalendar"]));
        paymentCalendar = *p;
    }
    QuantLib::Period exCouponPeriod;
    if(bondparam.containsElementNamed("exCouponPeriod") ) {
        exCouponPeriod = QuantLib::Period(Rcpp::as<double>(bondparam["exCouponPeriod"]), QuantLib::Days);
    }
    QuantLib::Calendar exCouponCalendar;
    if(bondparam.containsElementNamed("exCouponCalendar") ) {
        QuantLib::ext::shared_ptr<QuantLib::Calendar> p = getCalendar(Rcpp::as<std::string>(bondparam["exCouponCalendar"]));
        exCouponCalendar = *p;
    }
    QuantLib::BusinessDayConvention exCouponConvention = QuantLib::Unadjusted;
    if(bondparam.containsElementNamed("exCouponConvention") ) {
        exCouponConvention = getBusinessDayConvention(Rcpp::as<double>(bondparam["exCouponConvention"]));
    }
    bool exCouponEndOfMonth = false;
    if(bondparam.containsElementNamed("exCouponEndOfMonth") ) {
        exCouponEndOfMonth = (Rcpp::as<double>(bondparam["exCouponEndOfMonth"]) == 1) ? true : false;
    }

    QuantLib::Schedule schedule = getSchedule(scheduleparam);
    QuantLib::ext::shared_ptr<QuantLib::FixedRateBond> result(
        new QuantLib::FixedRateBond(settlementDays,
                                    faceAmount,
                                    schedule,
                                    ratesVec,
                                    accrualDayCounter,
                                    paymentConvention,
                                    redemption,
                                    issueDate,
                                    paymentCalendar,
                                    exCouponPeriod,
                                    exCouponCalendar,
                                    exCouponConvention,
                                    exCouponEndOfMonth));
    return result;
}

QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>
rebuildCurveFromZeroRates(std::vector<QuantLib::Date> dates,
                          std::vector<double> zeros) {
    QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>
        rebuilt_curve(new QuantLib::InterpolatedZeroCurve<QuantLib::LogLinear>(dates,
                                                                               zeros,
                                                                               QuantLib::ActualActual()));
    return rebuilt_curve;
}

QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> getFlatCurve(Rcpp::List curve) {
    QuantLib::Rate riskFreeRate = Rcpp::as<double>(curve["riskFreeRate"]);
    QuantLib::Date today(Rcpp::as<QuantLib::Date>(curve["todayDate"]));
    QuantLib::ext::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(riskFreeRate));
    QuantLib::Settings::instance().evaluationDate() = today;
    return flatRate(today, rRate, QuantLib::Actual360());
}

QuantLib::ext::shared_ptr<QuantLib::IborIndex> getIborIndex(Rcpp::List rparam, const QuantLib::Date today) {
    std::string type = Rcpp::as<std::string>(rparam["type"]);
    if (type == "USDLibor"){
        double riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        double period = Rcpp::as<double>(rparam["period"]);
        QuantLib::ext::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(riskFreeRate));
        QuantLib::Settings::instance().evaluationDate() = today;
        QuantLib::Handle<QuantLib::YieldTermStructure> curve(flatRate(today, rRate, QuantLib::Actual360()));
        QuantLib::ext::shared_ptr<QuantLib::IborIndex> iindex(new QuantLib::USDLibor(period * QuantLib::Months, curve));
        return iindex;
    }
    else return QuantLib::ext::shared_ptr<QuantLib::IborIndex>();
}

// std::vector<double> getDoubleVector(SEXP vecSexp) {
//     if (::Rf_length(vecSexp) == 0) {
//         return(std::vector<double>());
//     } else {
//         return std::vector<double>( Rcpp::as<std::vector< double> >( Rcpp::NumericVector(vecSexp) ) );
//     }
// }

QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>
makeFlatCurve(const QuantLib::Date& today,
              const QuantLib::ext::shared_ptr<QuantLib::Quote>& forward,
              const QuantLib::DayCounter& dc) {
    return QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>(new QuantLib::FlatForward(today, QuantLib::Handle<QuantLib::Quote>(forward), dc));
}

QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>
flatRate(const QuantLib::Date& today,
         const QuantLib::ext::shared_ptr<QuantLib::Quote>& forward,
         const QuantLib::DayCounter& dc) {
    return QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>(new QuantLib::FlatForward(today, QuantLib::Handle<QuantLib::Quote>(forward), dc));
}

QuantLib::ext::shared_ptr<QuantLib::BlackVolTermStructure>
makeFlatVolatility(const QuantLib::Date& today,
                   const QuantLib::ext::shared_ptr<QuantLib::Quote>& vol,
                   const QuantLib::DayCounter dc) {
    return QuantLib::ext::shared_ptr<QuantLib::BlackVolTermStructure>(new QuantLib::BlackConstantVol(today,
                                                                                             QuantLib::NullCalendar(),
                                                                                             QuantLib::Handle<QuantLib::Quote>(vol), dc));
}

QuantLib::ext::shared_ptr<QuantLib::BlackVolTermStructure>
flatVol(const QuantLib::Date& today,
        const QuantLib::ext::shared_ptr<QuantLib::Quote>& vol,
        const QuantLib::DayCounter& dc) {
    return QuantLib::ext::shared_ptr<QuantLib::BlackVolTermStructure>(
           new QuantLib::BlackConstantVol(today,
                                          QuantLib::NullCalendar(),
                                          QuantLib::Handle<QuantLib::Quote>(vol), dc));
}

typedef QuantLib::BlackScholesMertonProcess BSMProcess; // shortcut
QuantLib::ext::shared_ptr<QuantLib::GeneralizedBlackScholesProcess>
makeProcess(const QuantLib::ext::shared_ptr<QuantLib::Quote>& u,
            const QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>& q,
            const QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>& r,
            const QuantLib::ext::shared_ptr<QuantLib::BlackVolTermStructure>& vol) {
    return QuantLib::ext::shared_ptr<BSMProcess>(new BSMProcess(QuantLib::Handle<QuantLib::Quote>(u),
                                                        QuantLib::Handle<QuantLib::YieldTermStructure>(q),
                                                        QuantLib::Handle<QuantLib::YieldTermStructure>(r),
                                                        QuantLib::Handle<QuantLib::BlackVolTermStructure>(vol)));
}

// R uses dates indexed to Jan 1, 1970. RcppDate uses an internal Julian Date representation,
// but Quantlib uses the 'spreadsheet' format indexed to 1905 so we need to adjust
// int dateFromR(const RcppDate &d) {
//     return(d.getJDN() - RcppDate::Jan1970Offset + RcppDate::QLtoJan1970Offset);
// }

//static const unsigned int QLtoJan1970Offset = 25569;  	// Offset between R / Unix epoch

// // R and Rcpp::Date use the same 'days since epoch' representation; QL uses Excel style
// int dateFromR(const Rcpp::Date &d) {
//     static const unsigned int QLtoJan1970Offset = 25569;  	// Offset to R / Unix epoch
//     return(d.getDate() + QLtoJan1970Offset);
// }

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
    else if (n==6)
        return QuantLib::Thirty360();
#ifdef RQUANTLIB_USE_ACTUAL365NOLEAP
     else if (n==7)
         return QuantLib::Actual365NoLeap();
#endif
    else if (n==8)
        return QuantLib::ActualActual(QuantLib::ActualActual::ISMA);
    else if (n==9)
        return QuantLib::ActualActual(QuantLib::ActualActual::Bond);
    else if (n==10)
        return QuantLib::ActualActual(QuantLib::ActualActual::ISDA);
    else if (n==11)
        return QuantLib::ActualActual(QuantLib::ActualActual::Historical);
    else if (n==12)
        return QuantLib::ActualActual(QuantLib::ActualActual::AFB);
    else // if (n==13)
        return QuantLib::ActualActual(QuantLib::ActualActual::Euro);
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
    else if (n==4)
        return QuantLib::Unadjusted;
    else if (n==5)
        return QuantLib::HalfMonthModifiedFollowing;
    else if (n==6)
        return QuantLib::Nearest;
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
    else if (n==5)
        return QuantLib::DateGeneration::TwentiethIMM;
    else if (n==6)
        return QuantLib::DateGeneration::OldCDS;
    else if (n==7)
        return QuantLib::DateGeneration::CDS;
    else
        return QuantLib::DateGeneration::TwentiethIMM;
}

QuantLib::ext::shared_ptr<QuantLib::IborIndex> buildIborIndex(std::string type,
                                                      const QuantLib::Handle<QuantLib::YieldTermStructure>& iborStrc){
    if (type == "Euribor10M")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor10M(iborStrc));
    if (type == "Euribor11M")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor11M(iborStrc));
    if (type == "Euribor1M")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor1M(iborStrc));
    if (type == "Euribor1Y")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor1Y(iborStrc));
    if (type == "Euribor2M")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor2M(iborStrc));
    if (type == "Euribor2W")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor2W(iborStrc));
    if (type == "Euribor3M")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor3M(iborStrc));
    if (type == "Euribor3W")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor3W(iborStrc));
    if (type == "Euribor4M")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor4M(iborStrc));
    if (type == "Euribor5M")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor5M(iborStrc));
    if (type == "Euribor6M")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor6M(iborStrc));
    if (type == "Euribor7M")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor7M(iborStrc));
    if (type == "Euribor8M")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor8M(iborStrc));
    if (type == "Euribor9M")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::Euribor9M(iborStrc));
    if (type == "EuriborSW")
        return QuantLib::ext::shared_ptr<QuantLib::IborIndex>(new QuantLib::EuriborSW(iborStrc));
    return QuantLib::ext::shared_ptr<QuantLib::IborIndex>();
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


QuantLib::DividendSchedule getDividendSchedule(Rcpp::DataFrame divScheDF) {

    QuantLib::DividendSchedule dividendSchedule;
    try {
        Rcpp::CharacterVector s0v = divScheDF[0];
        Rcpp::NumericVector n1v = divScheDF[1];
        Rcpp::NumericVector n2v = divScheDF[2];
        Rcpp::NumericVector n3v = divScheDF[3];
        int nrow = s0v.size();

        for (int row=0; row<nrow; row++){
            int type = (s0v[row] == "Fixed") ? 1 : 0; //  (table[row][0].getStringValue()=="Fixed") ? 1 : 0;
            double amount = n1v[row]; // table[row][1].getDoubleValue();
            double rate = n2v[row]; // table[row][2].getDoubleValue();
            Rcpp::Date rd = Rcpp::Date(n3v[row]);
            QuantLib::Date d(Rcpp::as<QuantLib::Date>(Rcpp::wrap(rd))); //table[row][3].getDateValue()));
            if (type==1) {
                dividendSchedule.push_back(QuantLib::ext::shared_ptr<QuantLib::Dividend>(new QuantLib::FixedDividend(amount, d)));
            } else {
                dividendSchedule.push_back(QuantLib::ext::shared_ptr<QuantLib::Dividend>(new QuantLib::FractionalDividend(rate, amount, d)));
            }
        }
    } catch (std::exception& ex) {
        forward_exception_to_r(ex);
    }
    return dividendSchedule;
}

QuantLib::CallabilitySchedule getCallabilitySchedule(Rcpp::DataFrame callScheDF) {

    QuantLib::CallabilitySchedule callabilitySchedule;

    // QuantLib::Callability::Price triggers 'deprecated' under clang++-9
    // yet the new QuantLib::Bond::Price appeared only in QuantLib 1.17 so
    // it is a little nice to protect buildability under 1.16 with this
    // it also shortens the line a little below :)
#if QL_HEX_VERSION >= 0x011700f0
    typedef QuantLib::Bond::Price QlBondPrice;
#else
    typedef QuantLib::Callability::Price QlBondPrice;
#endif

    try {
        // RcppFrame rcppCallabilitySchedule(callabilityScheduleFrame);
        // std::vector<std::vector<ColDatum> > table = rcppCallabilitySchedule.getTableData();
        // int nrow = table.size();
        Rcpp::NumericVector n0v = callScheDF[0];
        Rcpp::CharacterVector s1v = callScheDF[1];
        Rcpp::NumericVector n2v = callScheDF[2];
        int nrow = n0v.size();
        for (int row=0; row<nrow; row++) {
            double price = n0v[row]; //table[row][0].getDoubleValue();
            int type = (s1v[row]=="P") ? 1 : 0;
            Rcpp::Date rd = Rcpp::Date(n2v[row]);
            QuantLib::Date d(Rcpp::as<QuantLib::Date>(Rcpp::wrap(rd)));
            if (type==1){
                callabilitySchedule.push_back(QuantLib::ext::shared_ptr<QuantLib::Callability>
                                              (new QuantLib::Callability(QlBondPrice(price, QlBondPrice::Clean), QuantLib::Callability::Put,d )));
            } else {
                callabilitySchedule.push_back(QuantLib::ext::shared_ptr<QuantLib::Callability>
                                              (new QuantLib::Callability(QlBondPrice(price, QlBondPrice::Clean), QuantLib::Callability::Call,d )));
            }
        }
    } catch (std::exception& ex){
        forward_exception_to_r(ex);
    }
    return callabilitySchedule;
}

QuantLib::Duration::Type getDurationType(const double n) {
    if (n==0)
        return QuantLib::Duration::Simple;
    else if (n==1)
        return QuantLib::Duration::Macaulay;
    else if (n==2)
        return QuantLib::Duration::Modified;
    else {
        throw std::range_error("Invalid duration type " + boost::lexical_cast<std::string>(n));
    }
}

//' This function returns the QuantLib version string as encoded in the header
//' file \code{config.hpp} and determined at compilation time of the QuantLib library.
//'
//' @title Return the QuantLib version number
//' @return A character variable
//' @references \url{http://quantlib.org} for details on \code{QuantLib}.
//' @author Dirk Eddelbuettel
//' @examples
//'   getQuantLibVersion()
// [[Rcpp::export]]
std::string getQuantLibVersion() {
    return std::string(QL_PACKAGE_VERSION);
}

//' This function returns a named vector of boolean variables describing several
//' configuration options determined at compilation time of the QuantLib library.
//'
//' Not all of these features are used (yet) by RQuantLib.
//' @title Return configuration options of the QuantLib library
//' @return A named vector of logical variables
//' @references \url{http://quantlib.org} for details on \code{QuantLib}.
//' @author Dirk Eddelbuettel
//' @examples
//'   getQuantLibCapabilities()
// [[Rcpp::export]]
Rcpp::LogicalVector getQuantLibCapabilities() {
    bool hasSessions = false,        	// not (yet?) used by RQuantLib
        hasHighResolutionDate=false,    // supported as of RQuantLib 0.4.2
        hasNegativeRates=false;         // not explicitly supported or denied

#ifdef QL_ENABLE_SESSIONS
    hasSessions = true;
#endif

#ifdef QL_HIGH_RESOLUTION_DATE
    hasHighResolutionDate=true;
#endif

#ifdef QL_NEGATIVE_RATES
    hasNegativeRates=true;
#endif

    return Rcpp::LogicalVector::create(Rcpp::Named("sessions") = hasSessions,
                                       Rcpp::Named("intradayDate") = hasHighResolutionDate,
                                       Rcpp::Named("negativeRates") = hasNegativeRates);
}
