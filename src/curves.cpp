// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib helper functions for term structure construction
//
// Copyright (C) 2005 - 2007  Dominick Samperi
// Copyright (C) 2007 - 2010  Dirk Eddelbuettel
//
// $Id$
//
// This program is part of the RQuantLib library for R (GNU S).
// It is made available under the terms of the GNU General Public
// License, version 2, or at your option, any later version.
//
// This program is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU General Public License for more
// details.

//#ifndef _MSC_VER
//#include <stdexcept>
//#endif

#include <rquantlib.hpp>

// Database of interest rate instrument contract details.
ObservableDB::ObservableDB() {
    db_["d1w"] = new RQLObservable(RQLDeposit, 1, 0, Weeks);
    db_["d1m"] = new RQLObservable(RQLDeposit, 1, 0, Months);
    db_["d2m"] = new RQLObservable(RQLDeposit, 2, 0, Months);
    db_["d3m"] = new RQLObservable(RQLDeposit, 3, 0, Months);
    db_["d6m"] = new RQLObservable(RQLDeposit, 6, 0, Months);
    db_["d9m"] = new RQLObservable(RQLDeposit, 9, 0, Months);
    db_["d1y"] = new RQLObservable(RQLDeposit, 1, 0, Years);
    db_["s2y"] = new RQLObservable(RQLSwap, 2, 0, Years);
    db_["s3y"] = new RQLObservable(RQLSwap, 3, 0, Years);
    db_["s5y"] = new RQLObservable(RQLSwap, 5, 0, Years);
    db_["s10y"] = new RQLObservable(RQLSwap, 10, 0, Years);
    db_["s15y"] = new RQLObservable(RQLSwap, 15, 0, Years);
    db_["s20y"] = new RQLObservable(RQLSwap, 20, 0, Years);
    db_["s30y"] = new RQLObservable(RQLSwap, 30, 0, Years);
    db_["fut1"] = new RQLObservable(RQLFuture, 1, 0, Months);
    db_["fut2"] = new RQLObservable(RQLFuture, 2, 0, Months);
    db_["fut3"] = new RQLObservable(RQLFuture, 3, 0, Months);
    db_["fut4"] = new RQLObservable(RQLFuture, 4, 0, Months);
    db_["fut5"] = new RQLObservable(RQLFuture, 5, 0, Months);
    db_["fut6"] = new RQLObservable(RQLFuture, 6, 0, Months);
    db_["fut7"] = new RQLObservable(RQLFuture, 7, 0, Months);
    db_["fut8"] = new RQLObservable(RQLFuture, 8, 0, Months);
    db_["fra3x6"] = new RQLObservable(RQLFRA, 3, 6, Months);
    db_["fra6x9"] = new RQLObservable(RQLFRA, 6, 9, Months);
    db_["fra6x12"] = new RQLObservable(RQLFRA, 6, 12, Months);
}


// Get RateHelper used to build the yield curve corresponding to a
// database key ('ticker') and observed rate/price.
boost::shared_ptr<RateHelper> ObservableDB::getRateHelper(std::string& ticker, Rate r) {
    RQLMapIterator iter = db_.find(ticker);
    if(iter == db_.end()) {
        std::ostringstream oss;
        oss << "Unknown curve construction instrument: " << ticker;
        throw std::range_error(oss.str());
    }
    RQLObservable *p = iter->second;
    RQLObservableType type = p->getType();
    int n1 = p->getN1(), n2 = p->getN2();
    TimeUnit units = p->getUnits();

    Date settlementDate = RQLContext::instance().settleDate;
    Calendar calendar = RQLContext::instance().calendar;
    Integer fixingDays = RQLContext::instance().fixingDays;
    DayCounter depositDayCounter = Actual360();

    // Tried to use a switch statement here, but there was an
    // internal compiler error using g++ Version 3.2.2.
    if(type == RQLDeposit) {
        boost::shared_ptr<Quote> quote(new SimpleQuote(r));
        boost::shared_ptr<RateHelper> depo(new DepositRateHelper(
	        Handle<Quote>(quote),
            n1*units, fixingDays,	
            calendar, ModifiedFollowing, 
            true, /*fixingDays,*/ depositDayCounter));
        return depo;
    } else if (type == RQLSwap) {
        Frequency swFixedLegFrequency = Annual;
        BusinessDayConvention swFixedLegConvention = Unadjusted;
        DayCounter swFixedLegDayCounter = Thirty360(Thirty360::European);
        boost::shared_ptr<IborIndex> swFloatingLegIndex(new Euribor6M);
        boost::shared_ptr<Quote> quote(new SimpleQuote(r));
        boost::shared_ptr<RateHelper> swap(new SwapRateHelper(
            Handle<Quote>(quote),
            n1*Years, /*fixingDays,*/
            calendar, swFixedLegFrequency,
            swFixedLegConvention, swFixedLegDayCounter,
            swFloatingLegIndex));
        return swap;
    }
    else if(type == RQLFuture) {
        Integer futMonths = 3;
        Date imm = IMM::nextDate(settlementDate);
        for(int i = 1; i < n1; i++)
            imm = IMM::nextDate(imm+1);
        std::cout << "Curves: IMM Date is " << imm << std::endl;
        boost::shared_ptr<Quote> quote(new SimpleQuote(r));
        boost::shared_ptr<RateHelper> future(new FuturesRateHelper(
	    	Handle<Quote>(quote),
            imm,
            futMonths, calendar, ModifiedFollowing,
            true, // added bool endOfMonth variable
            depositDayCounter));
        return future;
    } else if(type == RQLFRA) {
        boost::shared_ptr<Quote> quote(new SimpleQuote(r));
        boost::shared_ptr<RateHelper> FRA(new FraRateHelper(
            Handle<Quote>(quote),
            n1, n2, fixingDays, calendar, ModifiedFollowing,
            true, /*fixingDays,*/ depositDayCounter));
        return FRA;
    } else {
      throw std::range_error("Bad type in curve construction");
    }
}

// Return the term structure built using a set of RateHelpers (curveInput)
// employing the specified interpolation method and day counter.
boost::shared_ptr<YieldTermStructure> 
getTermStructure (std::string& interpWhat, std::string& interpHow, const Date& settlementDate,
                  const std::vector<boost::shared_ptr<RateHelper> >& curveInput,
                  DayCounter& dayCounter, Real tolerance) {
    if(interpWhat.compare("discount") == 0 &&
       interpHow.compare("linear") == 0) {
        boost::shared_ptr<YieldTermStructure> ts(new
	       PiecewiseYieldCurve<Discount,Linear>(settlementDate, 
	       curveInput, dayCounter, 
  	       std::vector<Handle<Quote> >(),
  	       std::vector<Date>(),
  	       tolerance));
        return ts;
    } else if(interpWhat.compare("discount") == 0 &&
              interpHow.compare("loglinear") == 0) {
        boost::shared_ptr<YieldTermStructure> ts(new
	       PiecewiseYieldCurve<Discount,LogLinear>(settlementDate, 
	       curveInput, dayCounter, 
  	       std::vector<Handle<Quote> >(),
  	       std::vector<Date>(),
  	       tolerance));
        return ts;
    } else if(interpWhat.compare("discount") == 0 &&
              interpHow.compare("spline") == 0) {
        boost::shared_ptr<YieldTermStructure> ts(new
  	       PiecewiseYieldCurve<Discount, Cubic>(settlementDate, 
	       curveInput, dayCounter, 
  	       std::vector<Handle<Quote> >(),
  	       std::vector<Date>(),
  	       tolerance));
        return ts;
    } else if(interpWhat.compare("forward") == 0 &&
              interpHow.compare("linear") == 0) {
        boost::shared_ptr<YieldTermStructure> ts(new
	       PiecewiseYieldCurve<ForwardRate,Linear>(settlementDate, 
	       curveInput, dayCounter, 
  	       std::vector<Handle<Quote> >(),
  	       std::vector<Date>(),
  	       tolerance));
        return ts;
    } else if(interpWhat.compare("forward") == 0 &&
            interpHow.compare("loglinear") == 0) {
        boost::shared_ptr<YieldTermStructure> ts(new
	       PiecewiseYieldCurve<ForwardRate,LogLinear>(settlementDate, 
	       curveInput, dayCounter, 
  	       std::vector<Handle<Quote> >(),
  	       std::vector<Date>(),
  	       tolerance));
        return ts;
    } else if(interpWhat.compare("forward") == 0 &&
              interpHow.compare("spline") == 0) {
        boost::shared_ptr<YieldTermStructure> ts(new
	       PiecewiseYieldCurve<ForwardRate,Cubic>(settlementDate, 
	       curveInput, dayCounter, 
  	       std::vector<Handle<Quote> >(),
  	       std::vector<Date>(),
  	       tolerance));
        return ts;
    } else if(interpWhat.compare("zero") == 0 &&
              interpHow.compare("linear") == 0) {
        boost::shared_ptr<YieldTermStructure> ts(new
	       PiecewiseYieldCurve<ZeroYield,Linear>(settlementDate, 
	       curveInput, dayCounter, 
  	       std::vector<Handle<Quote> >(),
  	       std::vector<Date>(),
	       tolerance));
        return ts;
    } else if(interpWhat.compare("zero") == 0 &&
              interpHow.compare("loglinear") == 0) {
        boost::shared_ptr<YieldTermStructure> ts(new
	       PiecewiseYieldCurve<ZeroYield,LogLinear>(settlementDate, 
	       curveInput, dayCounter, 
  	       std::vector<Handle<Quote> >(),
  	       std::vector<Date>(),
  	       tolerance));
        return ts;
    } else if(interpWhat.compare("zero") == 0 &&
              interpHow.compare("spline") == 0) {
        boost::shared_ptr<YieldTermStructure> ts(new
	       PiecewiseYieldCurve<ZeroYield,Cubic>(settlementDate, 
	       curveInput, dayCounter, 
  	       std::vector<Handle<Quote> >(),
  	       std::vector<Date>(),
  	       tolerance));
        return ts;
    } else {
        Rprintf((char*)"interpWhat = %s\n", interpWhat.c_str());
        Rprintf((char*)"interpHow  = %s\n", interpHow.c_str());
        throw std::range_error("What/How term structure options not recognized");
    }
}
