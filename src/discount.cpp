// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib function DiscountCurve
//
// Copyright (C) 2005 - 2007 Dominick Samperi
// Copyright (C) 2007 - 2009 Dirk Eddelbuettel <edd@debian.org>
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

#include "rquantlib.hpp"

RcppExport SEXP QL_DiscountCurve(SEXP params, SEXP tsQuotes, SEXP times) {
    SEXP rl = R_NilValue;
    char* exceptionMesg = NULL;

    try {

        // Parameter wrapper classes.
        RcppParams rparam(params);
        RcppNumList tslist(tsQuotes);

        int i;

        Date todaysDate( dateFromR(rparam.getDateValue("tradeDate") )); 
        Date settlementDate( dateFromR(rparam.getDateValue("settleDate") )); 
        //std::cout << "TradeDate: " << todaysDate << std::endl << "Settle: " << settlementDate << std::endl;

        RQLContext::instance().settleDate = settlementDate;
        Settings::instance().evaluationDate() = todaysDate;
        std::string firstQuoteName = tslist.getName(0);

        double dt = rparam.getDoubleValue("dt");
	
        std::string interpWhat, interpHow;
        bool flatQuotes = true;
        if (firstQuoteName.compare("flat") != 0) {
            // Get interpolation method (not needed for "flat" case)
            interpWhat = rparam.getStringValue("interpWhat");
            interpHow  = rparam.getStringValue("interpHow");
            flatQuotes = false;
        }

        Calendar calendar = TARGET();
        RQLContext::instance().calendar = calendar;
        Integer fixingDays = 2;
        RQLContext::instance().fixingDays = fixingDays;

        // Any DayCounter would be fine.
        // ActualActual::ISDA ensures that 30 years is 30.0
        DayCounter termStructureDayCounter = ActualActual(ActualActual::ISDA);
        double tolerance = 1.0e-8;

        boost::shared_ptr<YieldTermStructure> curve;
        if (firstQuoteName.compare("flat") == 0) {
            // Create a flat term structure.
            double rateQuote = tslist.getValue(0);
            //boost::shared_ptr<Quote> flatRate(new SimpleQuote(rateQuote));
            //boost::shared_ptr<FlatForward> ts(new FlatForward(settlementDate,
            //			      Handle<Quote>(flatRate),
            //			      ActualActual()));
            boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(rateQuote));
            curve = flatRate(settlementDate,rRate,ActualActual());

    	} else {
            // Build curve based on a set of observed rates and/or prices.
            std::vector<boost::shared_ptr<RateHelper> > curveInput;
            for(i = 0; i < tslist.size(); i++) {
                std::string name = tslist.getName(i);
                double val = tslist.getValue(i);
                boost::shared_ptr<RateHelper> rh = ObservableDB::instance().getRateHelper(name, val);
                // edd 2009-11-01 FIXME NULL_RateHelper no longer builds under 0.9.9
                // if (rh == NULL_RateHelper)
                if (rh.get() == NULL)
                    throw std::range_error("Unknown rate in getRateHelper");
                curveInput.push_back(rh);
            }
            boost::shared_ptr<YieldTermStructure> ts =
                getTermStructure(interpWhat, interpHow, settlementDate, curveInput, termStructureDayCounter, tolerance);
            curve = ts;
        }

        // Return discount, forward rate, and zero coupon curves
        int numCol = 2;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "date";
        colNames[1] = "zeroRates";
        
        RcppFrame frame(colNames);
        
        int ntimes = Rf_length(times);
        SEXP disc  = PROTECT(Rf_allocVector(REALSXP, ntimes));
        SEXP fwds  = PROTECT(Rf_allocVector(REALSXP, ntimes));
        SEXP zero  = PROTECT(Rf_allocVector(REALSXP, ntimes));
        
        
        Date current = settlementDate;
        double t;
        for(i = 0; i < ntimes; i++) {          
            t = REAL(times)[i];                                                    
            REAL(disc)[i] = curve->discount(t);
            REAL(fwds)[i] = curve->forwardRate(t, t+dt, Continuous);
            REAL(zero)[i] = curve->zeroRate(t, Continuous);
        }


        int n = curve->maxDate() - settlementDate;
        for (int i = 0; i<n;i++){
            std::vector<ColDatum> row(numCol);
            Date d = current; 
            row[0].setDateValue(RcppDate(d.month(), d.dayOfMonth(), d.year()));
            
            double zrate = curve->zeroRate(current, ActualActual(), Continuous);
            row[1].setDoubleValue(zrate);                        
            frame.addRow(row);
            current++;
        }

        RcppResultSet rs;
        rs.add("times", times, false);
        rs.add("discounts", disc, true);
        rs.add("forwards", fwds, true);
        rs.add("zerorates", zero, true);
        rs.add("flatQuotes", flatQuotes);
        rs.add("params", params, false);
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

