
//  RQuantLib function DiscountCurve
//
//  Copyright (C) 2005 - 2007  Dominick Samperi
//  Copyright (C) 2007 - 2019  Dirk Eddelbuettel
//  Copyright (C) 2009 - 2011  Dirk Eddelbuettel and Khanh Nguyen
//
//  This file is part of RQuantLib.
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

// [[Rcpp::export]]
Rcpp::List discountCurveEngine(Rcpp::List rparams,
                               Rcpp::List tslist,
                               Rcpp::NumericVector times,
                               Rcpp::List legParams) {

    std::vector<std::string> tsNames = tslist.names();

    int i;
    QuantLib::Date todaysDate(Rcpp::as<QuantLib::Date>(rparams["tradeDate"]));
    QuantLib::Date settlementDate(Rcpp::as<QuantLib::Date>(rparams["settleDate"]));
    RQLContext::instance().settleDate = settlementDate;

    QuantLib::Date evalDate = QuantLib::Settings::instance().evaluationDate();

    QuantLib::Settings::instance().evaluationDate() = todaysDate;
    std::string firstQuoteName = tsNames[0];

    double dt = Rcpp::as<double>(rparams["dt"]);

    std::string interpWhat, interpHow;
    bool flatQuotes = true;
    if (firstQuoteName.compare("flat") != 0) {
        // Get interpolation method (not needed for "flat" case)
        interpWhat = Rcpp::as<std::string>(rparams["interpWhat"]);
        interpHow  = Rcpp::as<std::string>(rparams["interpHow"]);
        flatQuotes = false;
    }
    // initialise from the singleton instance
    QuantLib::Calendar calendar = RQLContext::instance().calendar;
    //Integer fixingDays = RQLContext::instance().fixingDays;

    // Any DayCounter would be fine.
    // ActualActual::ISDA ensures that 30 years is 30.0
    QuantLib::DayCounter termStructureDayCounter = QuantLib::ActualActual(QuantLib::ActualActual::ISDA);
    double tolerance = 1.0e-8;
    QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> curve;

    if (firstQuoteName.compare("flat") == 0) {            // Create a flat term structure.
        double rateQuote = Rcpp::as<double>(tslist[0]);
        //QuantLib::ext::shared_ptr<Quote> flatRate(new SimpleQuote(rateQuote));
        //QuantLib::ext::shared_ptr<FlatForward> ts(new FlatForward(settlementDate,
        //                        Handle<Quote>(flatRate),
        //                        ActualActual()));
        QuantLib::ext::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(rateQuote));
        curve = flatRate(settlementDate,rRate,QuantLib::ActualActual());

    } else {             // Build curve based on a set of observed rates and/or prices.
        std::vector<QuantLib::ext::shared_ptr<QuantLib::RateHelper> > curveInput;

        // For general swap inputs, not elegant but necessary to pass to getRateHelper()
        double fixDayCount = Rcpp::as<double>(legParams["dayCounter"]);
        double fixFreq   = Rcpp::as<double>(legParams["fixFreq"]) ;
        int floatFreq = Rcpp::as<int>(legParams["floatFreq"]);
        //int floatFreq2 = 6;

        for(i = 0; i < tslist.size(); i++) {
            std::string name = tsNames[i];
            double val = Rcpp::as<double>(tslist[i]);
            QuantLib::ext::shared_ptr<QuantLib::RateHelper> rh =
                ObservableDB::instance().getRateHelper(name, val, fixDayCount,fixFreq, floatFreq);
            // edd 2009-11-01 FIXME NULL_RateHelper no longer builds under 0.9.9
            // if (rh == NULL_RateHelper)
            if (rh.get() == NULL)
                throw std::range_error("Unknown rate in getRateHelper");
            curveInput.push_back(rh);
        }
        QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>
            ts = getTermStructure(interpWhat, interpHow, settlementDate,
                                  curveInput, termStructureDayCounter, tolerance);
        curve = ts;
    }

    // Return discount, forward rate, and zero coupon curves
    int ntimes = times.size();
    Rcpp::NumericVector disc(ntimes), fwds(ntimes), zero(ntimes);

    QuantLib::Date current = settlementDate;
    for (i = 0; i < ntimes; i++) {
        double t = times[i];
        disc[i] = curve->discount(t);
        fwds[i] = curve->forwardRate(t, t+dt, QuantLib::Continuous);
        zero[i] = curve->zeroRate(t, QuantLib::Continuous);
    }

    QuantLib::Settings::instance().evaluationDate() = evalDate;

    std::vector<QuantLib::Date> dates;
    std::vector<double> zeroRates;
    QuantLib::Date d = current;
    QuantLib::Date maxDate(31, QuantLib::December, 2150);
    while (d < curve->maxDate() && d < maxDate) { // TODO set a max of, say, 5 or 10 years for flat curve
        double z = curve->zeroRate(d, QuantLib::ActualActual(), QuantLib::Continuous);
        dates.push_back(d);
        zeroRates.push_back(z);
        d = advanceDate(d, 21);      // TODO: make the increment a parameter
    }

    //Rcpp::DataFrame frame = Rcpp::DataFrame::create(Rcpp::Named("date") = dates,
    //                                                Rcpp::Named("zeroRates") = zeroRates);
    Rcpp::List frame = Rcpp::List::create(Rcpp::Named("date") = dates,
                                          Rcpp::Named("zeroRates") = zeroRates);

    Rcpp::List rl = Rcpp::List::create(Rcpp::Named("times") = times,
                                       Rcpp::Named("discounts") = disc,
                                       Rcpp::Named("forwards") = fwds,
                                       Rcpp::Named("zerorates") = zero,
                                       Rcpp::Named("flatQuotes") = flatQuotes,
                                       Rcpp::Named("params") = rparams,
                                       Rcpp::Named("table") = frame);
    return rl;
}
