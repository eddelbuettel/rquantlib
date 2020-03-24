
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2010         Dirk Eddelbuettel and Khanh Nguyen
//  Copyright (C) 2011 - 2020  Dirk Eddelbuettel
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
Rcpp::List calibrateHullWhiteUsingCapsEngine(std::vector<QuantLib::Date> termStrcDateVec,
                                             std::vector<double> termStrcZeroVec,
                                             Rcpp::DataFrame capDF,
                                             std::vector<QuantLib::Date> iborDateVec,
                                             std::vector<double> iborZeroVec,
                                             std::string iborType,
                                             QuantLib::Date evalDate) {

    QuantLib::Settings::instance().evaluationDate() = evalDate;

    QuantLib::Handle<QuantLib::YieldTermStructure>
        term(rebuildCurveFromZeroRates(termStrcDateVec, termStrcZeroVec));

    //set up ibor index
    QuantLib::Handle<QuantLib::YieldTermStructure>
        indexStrc(rebuildCurveFromZeroRates(iborDateVec, iborZeroVec));
    QuantLib::ext::shared_ptr<QuantLib::IborIndex> index = buildIborIndex(iborType, indexStrc);
    //process capDataDF
    std::vector<QuantLib::ext::shared_ptr <QuantLib::CalibrationHelper>> caps;

    //Rcpp::DataFrame capDF(capDataDF);
    Rcpp::NumericVector i0v = capDF[0];
    Rcpp::CharacterVector  s1v = capDF[1];
    Rcpp::NumericVector d2v = capDF[2];
    Rcpp::NumericVector i3v = capDF[3];
    Rcpp::NumericVector i4v = capDF[4];
    Rcpp::NumericVector i5v = capDF[5];
    //std::vector<std::vector<ColDatum> > table = capDF.getTableData();
    //int nrow = table.size();
    int nrow = i0v.size();
    for (int row=0; row<nrow;row++) {
        QuantLib::Period p = periodByTimeUnit(i0v[row], Rcpp::as<std::string>(s1v[row]));
        QuantLib::ext::shared_ptr<QuantLib::Quote> vol(new QuantLib::SimpleQuote(d2v[row]));
        QuantLib::DayCounter dc = getDayCounter(i4v[row]);
        QuantLib::ext::shared_ptr<QuantLib::BlackCalibrationHelper>
            helper(new QuantLib::CapHelper(p, QuantLib::Handle<QuantLib::Quote>(vol), index,
                                           getFrequency(i3v[row]),
                                           dc,
                                           (i5v[row]==1) ? true : false,
                                           term));
        QuantLib::ext::shared_ptr<QuantLib::BlackCapFloorEngine>
            engine(new QuantLib::BlackCapFloorEngine(term, d2v[row]));

        helper->setPricingEngine(engine);
        caps.push_back(helper);
    }

    //set up the HullWhite model
    QuantLib::ext::shared_ptr<QuantLib::HullWhite> model(new QuantLib::HullWhite(term));

    //calibrate the data
    QuantLib::LevenbergMarquardt optimizationMethod(1.0e-8,1.0e-8,1.0e-8);
    QuantLib::EndCriteria endCriteria(10000, 100, 1e-6, 1e-8, 1e-8);
    model->calibrate(caps, optimizationMethod, endCriteria);
    //EndCriteria::Type ecType = model->endCriteria();
    //return the result
    QuantLib::Array xMinCalculated = model->params();

    return Rcpp::List::create(Rcpp::Named("alpha") = xMinCalculated[0],
                              Rcpp::Named("sigma") = xMinCalculated[1]);
}

// [[Rcpp::export]]
Rcpp::List calibrateHullWhiteUsingSwapsEngine(std::vector<QuantLib::Date> termStrcDateVec,
                                              std::vector<double> termStrcZeroVec,
                                              Rcpp::DataFrame swapDF,
                                              std::vector<QuantLib::Date> iborDateVec,
                                              std::vector<double> iborZeroVec,
                                              std::string iborType,
                                              QuantLib::Date evalDate) {

    QuantLib::Settings::instance().evaluationDate() = evalDate;

    //set up the HullWhite model
    QuantLib::Handle<QuantLib::YieldTermStructure>
        term(rebuildCurveFromZeroRates(termStrcDateVec, termStrcZeroVec));

    QuantLib::ext::shared_ptr<QuantLib::HullWhite> model(new QuantLib::HullWhite(term));

    //set up ibor index
    QuantLib::Handle<QuantLib::YieldTermStructure>
        indexStrc(rebuildCurveFromZeroRates(iborDateVec, iborZeroVec));
    QuantLib::ext::shared_ptr<QuantLib::IborIndex> index = buildIborIndex(iborType, indexStrc);
    //process capDataDF
    QuantLib::ext::shared_ptr<QuantLib::PricingEngine>
        engine(new QuantLib::JamshidianSwaptionEngine(model));
    std::vector<QuantLib::ext::shared_ptr <QuantLib::CalibrationHelper>> swaps;

    //Rcpp::DataFrame swapDF(swapDataDF);
    Rcpp::NumericVector i0v = swapDF[0];
    Rcpp::CharacterVector  s1v = swapDF[1];
    Rcpp::NumericVector i2v = swapDF[2];
    Rcpp::CharacterVector s3v = swapDF[3];
    Rcpp::NumericVector d4v = swapDF[4];
    Rcpp::NumericVector i5v = swapDF[5];
    Rcpp::CharacterVector s6v = swapDF[6];
    Rcpp::NumericVector i7v = swapDF[7];
    Rcpp::NumericVector i8v = swapDF[8];
    //std::vector<std::vector<ColDatum> > table = swapDF.getTableData();
    //int nrow = table.size();
    int nrow = i0v.size();
    for (int row=0; row<nrow;row++) {
        QuantLib::Period maturity = periodByTimeUnit(i0v[row],
                                                     Rcpp::as<std::string>(s1v[row]));
        QuantLib::Period length = periodByTimeUnit(i0v[row],
                                                   Rcpp::as<std::string>(s3v[row]));

        QuantLib::ext::shared_ptr<QuantLib::Quote> vol(new QuantLib::SimpleQuote(d4v[row]));

        QuantLib::Period fixedLegTenor = periodByTimeUnit(i5v[row],
                                                          Rcpp::as<std::string>(s6v[row]));
        QuantLib::DayCounter fixedLegDayCounter = getDayCounter(i7v[row]);
        QuantLib::DayCounter floatingLegDayCounter = getDayCounter(i8v[row]);

        QuantLib::ext::shared_ptr<QuantLib::BlackCalibrationHelper>
            helper(new QuantLib::SwaptionHelper(maturity, length,
                                                QuantLib::Handle<QuantLib::Quote>(vol),
                                                index,
                                                fixedLegTenor,
                                                fixedLegDayCounter,
                                                floatingLegDayCounter,
                                                term));
        helper->setPricingEngine(engine);
        swaps.push_back(helper);
    }

    //calibrate the data
    QuantLib::LevenbergMarquardt optimizationMethod(1.0e-8,1.0e-8,1.0e-8);
    QuantLib::EndCriteria endCriteria(10000, 100, 1e-6, 1e-8, 1e-8);
    model->calibrate(swaps, optimizationMethod, endCriteria);
    //EndCriteria::Type ecType = model->endCriteria();
    //return the result
    QuantLib::Array xMinCalculated = model->params();

    return Rcpp::List::create(Rcpp::Named("alpha") = xMinCalculated[0],
                              Rcpp::Named("sigma") = xMinCalculated[1]);
}
