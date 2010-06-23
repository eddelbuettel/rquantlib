// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
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

RcppExport SEXP zeroprice(SEXP params) {

    try {
		Rcpp::List rparam(params);

        double yield = Rcpp::as<double>(rparam["Yield"]);
        QuantLib::Date maturity(dateFromR(Rcpp::Date(Rcpp::as<int>(rparam["Maturity"]))));
        QuantLib::Date settle(dateFromR(Rcpp::Date(Rcpp::as<int>(rparam["Settle"]))));

        Calendar calendar=UnitedStates(UnitedStates::GovernmentBond);
        QuantLib::Integer fixingDays = 2;
        Date todaysDate = calendar.advance(settle, -fixingDays, Days);
        Settings::instance().evaluationDate() = todaysDate;

        double period = Rcpp::as<double>(rparam["Period"]);
        double basis = Rcpp::as<double>(rparam["Basis"]);
        DayCounter dayCounter = getDayCounter(basis);
        Frequency freq = getFrequency(period);
        Period p(freq);
        //double EMR = Rcpp::as<double>(rparam["EMR");

        ZeroCouponBond bond(1, calendar, 
                            100, maturity, Unadjusted, 
                            100.0, settle);

        double price = bond.cleanPrice(yield, dayCounter, Compounded, freq);
        
        return Rcpp::wrap(price);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP zeroyield(SEXP params){

    try {
		Rcpp::List rparam(params);

        double price = Rcpp::as<double>(rparam["Price"]);
        QuantLib::Date maturity(dateFromR(Rcpp::Date(Rcpp::as<int>(rparam["Maturity"]))));
        QuantLib::Date settle(dateFromR(Rcpp::Date(Rcpp::as<int>(rparam["Settle"]))));

        Calendar calendar=UnitedStates(UnitedStates::GovernmentBond);
        QuantLib::Integer fixingDays = 2;
        Date todaysDate = calendar.advance(settle, -fixingDays, Days);
        Settings::instance().evaluationDate() = todaysDate;

        double period = Rcpp::as<double>(rparam["Period"]);
        double basis = Rcpp::as<double>(rparam["Basis"]);
        DayCounter dayCounter = getDayCounter(basis);
        Frequency freq = getFrequency(period);
        Period p(freq);
        //double EMR = Rcpp::as<double>(rparam["EMR");


        ZeroCouponBond bond(1, calendar, 100, maturity, Unadjusted, 100.0, settle);

        double yield = bond.yield(price, dayCounter, Compounded, freq);
        
        return Rcpp::wrap(yield);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP zbtyield(SEXP MatVec, SEXP BondMat, 
                         SEXP yieldVec, SEXP SettlVec,
                         SEXP cpVec, SEXP param) {
    try {   

        Rcpp::List par(param);
        //double oc = Rcpp::as<double>(rparam["OC");

        Rcpp::DateVector rmat(MatVec);
        Rcpp::DateVector rsettle(SettlVec);
        Rcpp::NumericVector yields(yieldVec);
        Rcpp::NumericVector cleanPrice(cpVec);
        Rcpp::NumericMatrix bondparam(BondMat);
        //std::vector<double> cleanPrice(rcp.stlVector());        
        //std::vector<double> yields(ryields.stlVector());
        //std::vector<std::vector<double> > bondparam(rbondmat.stlMatrix());

        std::cout << "read done";
        int n = rmat.size();
        std::vector<QuantLib::Date> MatDates(rmat.size());
        for (int i = 0;i<n;i++){
            QuantLib::Date day(dateFromR(rmat[i]));
            MatDates[i] = day;            
        }

        std::vector<QuantLib::Date> SettleDates(rsettle.size());
        for (int i = 0;i<n;i++){
            QuantLib::Date day(dateFromR(rsettle[i]) );
            SettleDates[i] = day;            
        }


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
        Settings::instance().evaluationDate() = todaysDate;
        Period p(getFrequency(2));
        double faceAmount = 100;
        DayCounter dayCounter = getDayCounter(2);
        bool emr = true;
        for (Size  j = 0; j< numberOfBonds;j++){

            if (bondparam.ncol() > 1) {
                p = Period(getFrequency(bondparam(j,2)));                           
                faceAmount = bondparam(j,1);
                dayCounter = getDayCounter(3);
                emr = (bondparam(j,4)==0) ? false : true;
            }
            
            Schedule schedule(SettleDates[j], MatDates[j],p, calendar,
                              Unadjusted, Unadjusted,
                              DateGeneration::Backward, emr);
            boost::shared_ptr<FixedRateBondHelper> 
                helper(new FixedRateBondHelper(quoteHandle[j], 1, faceAmount, schedule,
                                               std::vector<Rate>(1,bondparam(j,0)),
                                               dayCounter, Unadjusted, 100, SettleDates[j]));
            instruments.push_back(helper);
        }
        
		/*
        bool constrainAtZero = true;
        Real tolerance = 1.0e-10;
        Size max = 5000;

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

        boost::shared_ptr<YieldTermStructure> 
            curve(new PiecewiseYieldCurve<ZeroYield,Cubic>(1, calendar, instruments, dayCounter));
        std::cout << "here";
        int numCol = 2;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "date";
        colNames[1] = "zeroRates";
        
        Rcpp::DateVector dates(numberOfBonds);
		Rcpp::NumericVector zeros(numberOfBonds);

        Date current = SettleDates[0];
        //int n1 = curve->maxDate() - SettleDates[0];
        for (unsigned int i = 0; i<numberOfBonds;i++){
            Date d = MatDates[i];
            dates[i] = Rcpp::Date(d.month(), d.dayOfMonth(), d.year());
			zeros[i] = curve->zeroRate(d, ActualActual(), Simple);
            current++; // ?
        }

		return Rcpp::DataFrame::create(Rcpp::Named("date")=dates,
									   Rcpp::Named("zeroRates")=zeros);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}
