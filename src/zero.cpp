
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2009 - 2012  Dirk Eddelbuettel and Khanh Nguyen
//  Copyright (C) 2013 - 2019  Dirk Eddelbuettel
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
double zeroprice(double yield, QuantLib::Date maturity, QuantLib::Date settle, int period, int basis) {

    //setup bond; initialise calendar from the singleton instance
    QuantLib::Calendar calendar = RQLContext::instance().calendar;
    QuantLib::Integer fixingDays = RQLContext::instance().fixingDays;

    QuantLib::Date todaysDate = calendar.advance(settle, -fixingDays, QuantLib::Days);
    QuantLib::Settings::instance().evaluationDate() = todaysDate;

    QuantLib::DayCounter dayCounter = getDayCounter(basis);
    QuantLib::Frequency freq = getFrequency(period);
    QuantLib::Period p(freq);
    //double EMR = Rcpp::as<double>(rparam["EMR");

    QuantLib::ZeroCouponBond bond(1, calendar,
                                  100, maturity, QuantLib::Unadjusted,
                                  100.0, settle);

    return bond.cleanPrice(yield, dayCounter, QuantLib::Compounded, freq);
}

// [[Rcpp::export]]
double zeroyield(double price, QuantLib::Date maturity, QuantLib::Date settle, int period, int basis) {

    //setup bond; initialise calendar from the singleton instance
    QuantLib::Calendar calendar = RQLContext::instance().calendar;
    QuantLib::Integer fixingDays = RQLContext::instance().fixingDays;

    QuantLib::Date todaysDate = calendar.advance(settle, -fixingDays, QuantLib::Days);
    QuantLib::Settings::instance().evaluationDate() = todaysDate;

    QuantLib::DayCounter dayCounter = getDayCounter(basis);
    QuantLib::Frequency freq = getFrequency(period);
    QuantLib::Period p(freq);
    //double EMR = Rcpp::as<double>(rparam["EMR");

    QuantLib::ZeroCouponBond bond(1, calendar, 100, maturity, QuantLib::Unadjusted, 100.0, settle);

    return bond.yield(price, dayCounter, QuantLib::Compounded, freq);
}

Rcpp::DataFrame zbtyield(std::vector<QuantLib::Date> MatDates,
                         Rcpp::NumericMatrix bondparam,
                         Rcpp::NumericVector yields,
                         std::vector<QuantLib::Date> SettleDates,
                         Rcpp::NumericVector cleanPrice) {

    //setting up the bonds
    const QuantLib::Size numberOfBonds = MatDates.size();

    std::vector<QuantLib::ext::shared_ptr<QuantLib::RateHelper> > instruments;

    std::vector< QuantLib::ext::shared_ptr<QuantLib::SimpleQuote> > quote;
    for (QuantLib::Size i=0; i<numberOfBonds; i++) {
        QuantLib::ext::shared_ptr<QuantLib::SimpleQuote> cp(new QuantLib::SimpleQuote(cleanPrice[i]));
        quote.push_back(cp);
    }

    std::vector< QuantLib::RelinkableHandle<QuantLib::Quote> > quoteHandle(numberOfBonds);
    for (QuantLib::Size i=0; i<numberOfBonds; i++) {
        quoteHandle[i].linkTo(quote[i]);
    }

    QuantLib::Calendar calendar = RQLContext::instance().calendar;
    QuantLib::Date todaysDate = calendar.advance(SettleDates[0], -2, QuantLib::Days);
    QuantLib::Settings::instance().evaluationDate() = todaysDate;
    QuantLib::Period p(getFrequency(2));
    double faceAmount = 100;
    QuantLib::DayCounter dayCounter = getDayCounter(2);
    bool emr = true;
    for (QuantLib::Size  j = 0; j< numberOfBonds;j++){

        if (bondparam.ncol() > 1) {
            p = QuantLib::Period(getFrequency(bondparam(j,2)));
            faceAmount = bondparam(j,1);
            dayCounter = getDayCounter(3);
            emr = (bondparam(j,4)==0) ? false : true;
        }

        QuantLib::Schedule schedule(SettleDates[j], MatDates[j],p, calendar,
                                    QuantLib::Unadjusted, QuantLib::Unadjusted,
                                    QuantLib::DateGeneration::Backward, emr);
        QuantLib::ext::shared_ptr<QuantLib::FixedRateBondHelper>
            helper(new QuantLib::FixedRateBondHelper(quoteHandle[j], 1, faceAmount, schedule,
                                                     std::vector<QuantLib::Rate>(1,bondparam(j,0)),
                                                     dayCounter, QuantLib::Unadjusted, 100, SettleDates[j]));
        instruments.push_back(helper);
    }

    /*
      bool constrainAtZero = true;
      Real tolerance = 1.0e-10;
      Size max = 5000;

      QuantLib::ext::shared_ptr<YieldTermStructure> curve;
      NelsonSiegelFitting nelsonSiegel;
      QuantLib::ext::shared_ptr<FittedBondDiscountCurve> ts3 (
      new FittedBondDiscountCurve(1,
      calendar,
      instruments,
      dayCounter,
      nelsonSiegel,
      tolerance,
      max));
      curve = ts3;
    */

    QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>
        curve(new QuantLib::PiecewiseYieldCurve<QuantLib::ZeroYield,
                                                QuantLib::Cubic>(1, calendar, instruments, dayCounter));
    int numCol = 2;
    std::vector<std::string> colNames(numCol);
    colNames[0] = "date";
    colNames[1] = "zeroRates";

    Rcpp::DateVector dates(numberOfBonds);
    Rcpp::NumericVector zeros(numberOfBonds);

    QuantLib::Date current = SettleDates[0];
    //int n1 = curve->maxDate() - SettleDates[0];
    for (unsigned int i = 0; i<numberOfBonds;i++){
        QuantLib::Date d = MatDates[i];
        dates[i] = Rcpp::Date(d.month(), d.dayOfMonth(), d.year());
        zeros[i] = curve->zeroRate(d, QuantLib::ActualActual(), QuantLib::Simple);
        current++; // ?
    }

    return Rcpp::DataFrame::create(Rcpp::Named("date")=dates,
                                   Rcpp::Named("zeroRates")=zeros);
}
