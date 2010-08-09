// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib function prototypes and macros
//
// Copyright 2002 - 2009  Dirk Eddelbuettel <edd@debian.org>
// Copyright 2005 - 2006  Dominick Samperi
//
// $Id$
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef rquantlib_h
#define rquantlib_h

#include <ql/quantlib.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

//using namespace QuantLib;

#include <Rcpp.h>

//#define NULL_RateHelper (boost::shared_ptr<RateHelper>)Null<boost::shared_ptr<RateHelper> >()

// Prototypes for convenience functions (some macros)
//void insertListElement(SEXP &list, SEXP &names,
//                       const int pos, const double value, 
//                       const char *label);
//SEXP getListElement(SEXP list, char *str);

// Used to maintain context while in an R function.
class RQLContext : public QuantLib::Singleton<RQLContext> {
public:
    RQLContext() { 
        fixingDays = 2;
        calendar = QuantLib::TARGET();
        settleDate = QuantLib::Date::todaysDate()+2;
    }
    // The tradeDate (evaluation date) is maintained by Settings,
    // (which is a singleton structure provided by QuantLib)
    // and used to translate between dates and real-valued times.
    QuantLib::Date settleDate;
    QuantLib::Calendar calendar;
    QuantLib::Integer fixingDays;
};

// Instrument types used to construct the yield curve.
enum RQLObservableType { RQLDeposit, RQLSwap, RQLFuture, RQLFRA };

// Used to identify the specifics of a particular contract.
class RQLObservable {
public:
    RQLObservable(RQLObservableType type,
                  int n1, int n2,
                  QuantLib::TimeUnit units)
        : type_(type), n1_(n1), n2_(n2), units_(units) {}
    RQLObservableType getType() { return type_; }
    int getN1() { return n1_; }
    int getN2() { return n2_; }
    QuantLib::TimeUnit getUnits() { return units_; }
private:
    RQLObservableType type_;
    int n1_, n2_;    // n2 used for FRA's
    QuantLib::TimeUnit units_; // not used for futures and FRA's
};

typedef std::map<std::string, RQLObservable*> RQLMap;
typedef std::map<std::string, RQLObservable*>::const_iterator RQLMapIterator;

// Database used to maintain curve construction instrument details.
class ObservableDB : public QuantLib::Singleton<ObservableDB> {
public:
    ObservableDB();
    boost::shared_ptr<QuantLib::RateHelper> getRateHelper(std::string& ticker, QuantLib::Rate r);
private:
    RQLMap db_;
};

boost::shared_ptr<QuantLib::YieldTermStructure> 
getTermStructure(std::string& interpWhat, std::string& interpHow, 
                 const QuantLib::Date& settleDate,
                 const std::vector<boost::shared_ptr<QuantLib::RateHelper> >& curveInput,
                 QuantLib::DayCounter& dayCounter, QuantLib::Real tolerance);

boost::shared_ptr<QuantLib::YieldTermStructure>
makeFlatCurve(const QuantLib::Date& today,
              const boost::shared_ptr<QuantLib::Quote>& forward,
              const QuantLib::DayCounter& dc);

boost::shared_ptr<QuantLib::YieldTermStructure>
flatRate(const QuantLib::Date& today,
         const boost::shared_ptr<QuantLib::Quote>& forward,
         const QuantLib::DayCounter& dc);

boost::shared_ptr<QuantLib::BlackVolTermStructure> 
makeFlatVolatility(const QuantLib::Date& today,
                   const boost::shared_ptr<QuantLib::Quote>& vol,
                   QuantLib::DayCounter dc);

boost::shared_ptr<QuantLib::BlackVolTermStructure>
flatVol(const QuantLib::Date& today,
        const boost::shared_ptr<QuantLib::Quote>& vol,
        const QuantLib::DayCounter& dc);

enum EngineType {Analytic,
                 JR, CRR, EQP, TGEO, TIAN, LR, JOSHI,
                 FiniteDifferences, Integral,
                 PseudoMonteCarlo, QuasiMonteCarlo };

boost::shared_ptr<QuantLib::VanillaOption>
makeOption(const boost::shared_ptr<QuantLib::StrikedTypePayoff>& payoff,
           const boost::shared_ptr<QuantLib::Exercise>& exercise,
           const boost::shared_ptr<QuantLib::Quote>& u,
           const boost::shared_ptr<QuantLib::YieldTermStructure>& q,
           const boost::shared_ptr<QuantLib::YieldTermStructure>& r,
           const boost::shared_ptr<QuantLib::BlackVolTermStructure>& vol,
           EngineType engineType = Analytic,
           QuantLib::Size binomialSteps=128,
           QuantLib::Size samples=100); 

boost::shared_ptr<QuantLib::GeneralizedBlackScholesProcess>
makeProcess(const boost::shared_ptr<QuantLib::Quote>& u,
            const boost::shared_ptr<QuantLib::YieldTermStructure>& q,
            const boost::shared_ptr<QuantLib::YieldTermStructure>& r,
            const boost::shared_ptr<QuantLib::BlackVolTermStructure>& vol);

// int dateFromR(const RcppDate &d); 	// using 'classic' API's RcppDate 
int dateFromR(const Rcpp::Date &d); // using 'new' API's Rcpp::Date

//utility functions for parameters of fixed-income instrument function
QuantLib::Frequency getFrequency(const double n);
QuantLib::TimeUnit getTimeUnit(const double n);
QuantLib::Compounding getCompounding(const double n);
QuantLib::BusinessDayConvention getBusinessDayConvention(const double n);
QuantLib::DayCounter getDayCounter(const double n);
QuantLib::DateGeneration::Rule getDateGenerationRule(const double n);
boost::shared_ptr<QuantLib::YieldTermStructure> buildTermStructure(SEXP params,
                                                                   SEXP tsQuotes,
                                                                   SEXP times);
QuantLib::Schedule getSchedule(SEXP sch);
boost::shared_ptr<QuantLib::IborIndex> getIborIndex(SEXP index, const QuantLib::Date today);
std::vector<double> getDoubleVector(SEXP vector);
boost::shared_ptr<QuantLib::YieldTermStructure> getFlatCurve(SEXP flatcurve);
boost::shared_ptr<QuantLib::YieldTermStructure> rebuildCurveFromZeroRates(SEXP dateSexp, SEXP zeroSexp);
boost::shared_ptr<QuantLib::IborIndex> 
    buildIborIndex(std::string type,
                   const QuantLib::Handle<QuantLib::YieldTermStructure>& iborStrc);
QuantLib::Calendar* getCalendar(SEXP calParameters);
QuantLib::Period periodByTimeUnit(int length, std::string unit);

// simple option type creator based on string
QuantLib::Option::Type getOptionType(const std::string &t);

// create a data.frame with dates and amounts
Rcpp::DataFrame getCashFlowDataFrame(const QuantLib::Leg &bondCashFlow);

// fill QL data structures based on data.frames
QuantLib::DividendSchedule getDividendSchedule(SEXP dividendScheduleFrame);
QuantLib::CallabilitySchedule getCallabilitySchedule(SEXP callabilityScheduleFrame);

#endif
