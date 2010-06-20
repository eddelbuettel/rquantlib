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

#ifndef rquantlib_hpp
#define rquantlib_hpp

#include <ql/quantlib.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace QuantLib;

#include <Rcpp.h>

//#define NULL_RateHelper (boost::shared_ptr<RateHelper>)Null<boost::shared_ptr<RateHelper> >()

// Prototypes for convenience functions (some macros)
//void insertListElement(SEXP &list, SEXP &names,
//                       const int pos, const double value, 
//                       const char *label);
//SEXP getListElement(SEXP list, char *str);

// Used to maintain context while in an R function.
class RQLContext : public Singleton<RQLContext> {
public:
    RQLContext() { 
        fixingDays = 2;
        calendar = TARGET();
        settleDate = Date::todaysDate()+2;
    }
    // The tradeDate (evaluation date) is maintained by Settings,
    // (which is a singleton structure provided by QuantLib)
    // and used to translate between dates and real-valued times.
    Date settleDate;
    Calendar calendar;
    Integer fixingDays;
};

// Instrument types used to construct the yield curve.
enum RQLObservableType { RQLDeposit, RQLSwap, RQLFuture, RQLFRA };

// Used to identify the specifics of a particular contract.
class RQLObservable {
public:
    RQLObservable(RQLObservableType type,
                  int n1, int n2,
                  TimeUnit units)
        : type_(type), n1_(n1), n2_(n2), units_(units) {}
    RQLObservableType getType() { return type_; }
    int getN1() { return n1_; }
    int getN2() { return n2_; }
    TimeUnit getUnits() { return units_; }
private:
    RQLObservableType type_;
    int n1_, n2_;    // n2 used for FRA's
    TimeUnit units_; // not used for futures and FRA's
};

typedef std::map<std::string, RQLObservable*> RQLMap;
typedef std::map<std::string, RQLObservable*>::const_iterator RQLMapIterator;

// Database used to maintain curve construction instrument details.
class ObservableDB : public Singleton<ObservableDB> {
public:
    ObservableDB();
    boost::shared_ptr<RateHelper> getRateHelper(std::string& ticker, Rate r);
private:
    RQLMap db_;
};

boost::shared_ptr<YieldTermStructure> 
getTermStructure(std::string& interpWhat, std::string& interpHow, 
                 const Date& settleDate,
                 const std::vector<boost::shared_ptr<RateHelper> >& curveInput,
                 DayCounter& dayCounter, Real tolerance);

boost::shared_ptr<YieldTermStructure>
makeFlatCurve(const Date& today,
              const boost::shared_ptr<Quote>& forward,
              const DayCounter& dc);

boost::shared_ptr<YieldTermStructure>
flatRate(const Date& today,
         const boost::shared_ptr<Quote>& forward,
         const DayCounter& dc);

boost::shared_ptr<BlackVolTermStructure> 
makeFlatVolatility(const Date& today,
                   const boost::shared_ptr<Quote>& vol,
                   DayCounter dc);

boost::shared_ptr<BlackVolTermStructure>
flatVol(const Date& today,
        const boost::shared_ptr<Quote>& vol,
        const DayCounter& dc);

enum EngineType {Analytic,
                 JR, CRR, EQP, TGEO, TIAN, LR, JOSHI,
                 FiniteDifferences, Integral,
                 PseudoMonteCarlo, QuasiMonteCarlo };

boost::shared_ptr<VanillaOption>
makeOption(const boost::shared_ptr<StrikedTypePayoff>& payoff,
           const boost::shared_ptr<Exercise>& exercise,
           const boost::shared_ptr<Quote>& u,
           const boost::shared_ptr<YieldTermStructure>& q,
           const boost::shared_ptr<YieldTermStructure>& r,
           const boost::shared_ptr<BlackVolTermStructure>& vol,
           EngineType engineType = Analytic,
           Size binomialSteps=128,
           Size samples=100); 

boost::shared_ptr<GeneralizedBlackScholesProcess>
makeProcess(const boost::shared_ptr<Quote>& u,
            const boost::shared_ptr<YieldTermStructure>& q,
            const boost::shared_ptr<YieldTermStructure>& r,
            const boost::shared_ptr<BlackVolTermStructure>& vol);

int dateFromR(const RcppDate &d);

//utility functions for parameters of fixed-income instrument function
Frequency getFrequency(const double n);
TimeUnit getTimeUnit(const double n);
Compounding getCompounding(const double n);
BusinessDayConvention getBusinessDayConvention(const double n);
DayCounter getDayCounter(const double n);
DateGeneration::Rule getDateGenerationRule(const double n);
boost::shared_ptr<YieldTermStructure> buildTermStructure(SEXP params,
                                                         SEXP tsQuotes,
                                                         SEXP times);
Schedule getSchedule(SEXP sch);
boost::shared_ptr<IborIndex> getIborIndex(SEXP index, const Date today);
std::vector<double> getDoubleVector(SEXP vector);
boost::shared_ptr<YieldTermStructure> getFlatCurve(SEXP flatcurve);
boost::shared_ptr<YieldTermStructure> rebuildCurveFromZeroRates(SEXP dateSexp, SEXP zeroSexp);
boost::shared_ptr<IborIndex> buildIborIndex(std::string type,
                                            const Handle<YieldTermStructure>& iborStrc);
Calendar* getCalendar(SEXP calParameters);
Period periodByTimeUnit(int length, std::string unit);

// simple option type creator based on string
Option::Type getOptionType(const std::string &t);

// create a data.frame with dates and amounts
Rcpp::DataFrame getCashFlowDataFrame(const Leg &bondCashFlow);

// fill QL data structures based on data.frames
DividendSchedule getDividendSchedule(SEXP dividendScheduleFrame);
CallabilitySchedule getCallabilitySchedule(SEXP callabilityScheduleFrame);

#endif
