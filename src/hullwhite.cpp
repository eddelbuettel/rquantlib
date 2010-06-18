// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2010         Dirk Eddelbuettel and Khanh Nguyen
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

using namespace boost;

RcppExport SEXP QL_HullWhiteCalibrationUsingCap(SEXP termStrcDateVec,
												SEXP termStrcZeroVec,
												SEXP capDataDF,
												SEXP iborDateVec,
												SEXP iborZeroVec,
												SEXP iborparams,
												SEXP evaluationDate) {
	
    try {
        RcppDateVector dv(evaluationDate);
        QuantLib::Date evalDate(dateFromR(dv(0)));
        Settings::instance().evaluationDate() = evalDate;

        Handle<YieldTermStructure> 
			term(rebuildCurveFromZeroRates(termStrcDateVec,
										   termStrcZeroVec));
        
        //set up ibor index
        Handle<YieldTermStructure> 
			indexStrc(rebuildCurveFromZeroRates(iborDateVec,
												iborZeroVec));    
		Rcpp::List param(iborparams);
        std::string iborType = Rcpp::as<std::string>(param["type"]);
        boost::shared_ptr<IborIndex> index = buildIborIndex(iborType, indexStrc);
        //process capDataDF
        std::vector<boost::shared_ptr <CalibrationHelper> > caps;
        try {
			Rcpp::DataFrame capDF(capDataDF);
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
                //Period p = periodByTimeUnit(table[row][0].getIntValue(),
                //                            table[row][1].getStringValue());
                Period p = periodByTimeUnit(i0v[row], 
											Rcpp::as<std::string>(s1v[row]));
                
                boost::shared_ptr<Quote> 
					//vol(new SimpleQuote(table[row][2].getDoubleValue()));
					vol(new SimpleQuote(d2v[row]));
				//DayCounter dc = getDayCounter(table[row][4].getIntValue());
				DayCounter dc = getDayCounter(i4v[row]);
                boost::shared_ptr<CalibrationHelper> 
                    helper(new CapHelper(p, Handle<Quote>(vol), index, 
                                         //getFrequency(table[row][3].getIntValue()),
										 getFrequency(i3v[row]),
                                         dc,
                                         //(table[row][5].getIntValue()==1)? true:false,>(
										 (i5v[row]==1) ? true : false,
                                         term));
                boost::shared_ptr<BlackCapFloorEngine> 
					engine(new BlackCapFloorEngine(term, 
												   //table[row][2].getDoubleValue()));
												   d2v[row]));
                
                helper->setPricingEngine(engine);                
                caps.push_back(helper);
			}
        } catch(std::exception& ex) { 
			forward_exception_to_r(ex); 
		}
        
        //set up the HullWhite model
        boost::shared_ptr<HullWhite> model(new HullWhite(term));
        
        //calibrate the data
        LevenbergMarquardt optimizationMethod(1.0e-8,1.0e-8,1.0e-8);
        EndCriteria endCriteria(10000, 100, 1e-6, 1e-8, 1e-8);
        model->calibrate(caps, optimizationMethod, endCriteria);
        //EndCriteria::Type ecType = model->endCriteria();
        //return the result
        Array xMinCalculated = model->params();

        return Rcpp::List::create(Rcpp::Named("alpha") = xMinCalculated[0],
								  Rcpp::Named("sigma") = xMinCalculated[1]);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }
    return R_NilValue;
}


RcppExport SEXP QL_HullWhiteCalibrationUsingSwap(SEXP termStrcDateVec,
												 SEXP termStrcZeroVec,
												 SEXP swapDataDF,
												 SEXP iborDateVec,
												 SEXP iborZeroVec,
												 SEXP iborparams,
												 SEXP evaluationDate){

    try {
        RcppDateVector dv(evaluationDate);
        QuantLib::Date evalDate(dateFromR(dv(0)));
        Settings::instance().evaluationDate() = evalDate;

        //set up the HullWhite model       
        Handle<YieldTermStructure> 
			term(rebuildCurveFromZeroRates(termStrcDateVec,
										   termStrcZeroVec));
        
        boost::shared_ptr<HullWhite> model(new HullWhite(term));        
        
        //set up ibor index
        Handle<YieldTermStructure> 
			indexStrc(rebuildCurveFromZeroRates(iborDateVec,
												iborZeroVec));    
		Rcpp::List param(iborparams);
        std::string iborType = Rcpp::as<std::string>(param["type"]);
        boost::shared_ptr<IborIndex> index = buildIborIndex(iborType, indexStrc);
        //process capDataDF
        boost::shared_ptr<PricingEngine> 
			engine(new JamshidianSwaptionEngine(model));
        std::vector<boost::shared_ptr <CalibrationHelper> > swaps;

        try {
            //RcppFrame swapDF(swapDataDF);
			Rcpp::DataFrame swapDF(swapDataDF);
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
            for (int row=0; row<nrow;row++){

                //Period maturity = periodByTimeUnit(table[row][0].getIntValue(),
                //                                   table[row][1].getStringValue());
                Period maturity = periodByTimeUnit(i0v[row], 
												   Rcpp::as<std::string>(s1v[row]));
                //Period length = periodByTimeUnit(table[row][2].getIntValue(),
                //                            table[row][3].getStringValue());                                
                Period length = periodByTimeUnit(i0v[row], 
												 Rcpp::as<std::string>(s3v[row]));

                //boost::shared_ptr<Quote> vol(new SimpleQuote(table[row][4].getDoubleValue()));
                boost::shared_ptr<Quote> vol(new SimpleQuote(d4v[row]));

                //Period fixedLegTenor = periodByTimeUnit(table[row][5].getIntValue(),
                //                            table[row][6].getStringValue());
                Period fixedLegTenor = periodByTimeUnit(i5v[row], 
														Rcpp::as<std::string>(s6v[row]));                                
				//DayCounter fixedLegDayCounter = getDayCounter(table[row][7].getIntValue());
                DayCounter fixedLegDayCounter = getDayCounter(i7v[row]);
                //DayCounter floatingLegDayCounter = getDayCounter(table[row][8].getIntValue());
				DayCounter floatingLegDayCounter = getDayCounter(i8v[row]);

                boost::shared_ptr<CalibrationHelper> 
                    helper(new SwaptionHelper(maturity, length, 
                                              Handle<Quote>(vol), 
                                              index, 
                                              fixedLegTenor, 
                                              fixedLegDayCounter,
                                              floatingLegDayCounter,
                                              term));
                helper->setPricingEngine(engine);                
                swaps.push_back(helper);
            }
        } catch(std::exception& ex) { 
			forward_exception_to_r(ex); 
		}
        
        //calibrate the data
        LevenbergMarquardt optimizationMethod(1.0e-8,1.0e-8,1.0e-8);
        EndCriteria endCriteria(10000, 100, 1e-6, 1e-8, 1e-8);
        model->calibrate(swaps, optimizationMethod, endCriteria);
        //EndCriteria::Type ecType = model->endCriteria();
        //return the result
        Array xMinCalculated = model->params();

        return Rcpp::List::create(Rcpp::Named("alpha") = xMinCalculated[0],
								  Rcpp::Named("sigma") = xMinCalculated[1]);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}
