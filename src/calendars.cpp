// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2002 - 2009 Dirk Eddelbuettel 
// Copyright (C) 2010        Dirk Eddelbuettel and Khanh Nguyen
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

#include <rquantlib.h>

// returns a reference-counted pointer to a matching Calendar object
boost::shared_ptr<Calendar> getCalendar(const std::string &calstr) {
    boost::shared_ptr<Calendar> pcal;

    if (calstr == "TARGET") { 		// generic calendar 
        pcal.reset(new TARGET());
        
    } else if (calstr == "Brazil") {
        pcal.reset(new Brazil());

    } else if (calstr == "Canada" || calstr == "Canada/Settlement") {
        pcal.reset(new Canada(Canada::Settlement));
    } else if (calstr == "Canada/TSX") {
        pcal.reset(new Canada(Canada::TSX));
        
    } else if (calstr == "Germany" || calstr == "Germany/FrankfurtStockExchange") {
        pcal.reset(new Germany(Germany::FrankfurtStockExchange));
    } else if (calstr == "Germany/Settlement") {
        pcal.reset(new Germany(Germany::Settlement));
    } else if (calstr == "Germany/Xetra") {
        pcal.reset(new Germany(Germany::Xetra));
    } else if (calstr == "Germany/Eurex") {
        pcal.reset(new Germany(Germany::Eurex));
        
    } else if (calstr == "Italy" || calstr == "Italy/Settlement") {
        pcal.reset(new Italy(Italy::Settlement));
    } else if (calstr == "Italy/Exchange") {
        pcal.reset(new Italy(Italy::Exchange));
        
    } else if (calstr == "Japan" || calstr == "Japan/Settlement") {
        pcal.reset(new Japan());
        
    } else if (calstr == "SouthKorea" || calstr == "SouthKorea/Settlement") {
        pcal.reset(new SouthKorea(SouthKorea::Settlement));
    } else if (calstr == "SouthKorea/KRX") {
        pcal.reset(new SouthKorea(SouthKorea::KRX));

    } else if (calstr == "UnitedKingdom" || calstr == "UnitedKingdom/Settlement") {
        pcal.reset(new UnitedKingdom(UnitedKingdom::Settlement));
    } else if (calstr == "UnitedKingdom/Exchange") {
        pcal.reset(new UnitedKingdom(UnitedKingdom::Exchange));
    } else if (calstr == "UnitedKingdom/Metals") {
        pcal.reset(new UnitedKingdom(UnitedKingdom::Metals));
        
    } else if (calstr == "UnitedStates" || calstr == "UnitedStates/Settlement") {
        pcal.reset(new UnitedStates(UnitedStates::Settlement));
    } else if (calstr == "UnitedStates/NYSE") {
        pcal.reset(new UnitedStates(UnitedStates::NYSE));
    } else if (calstr == "UnitedStates/GovernmentBond") {
        pcal.reset(new UnitedStates(UnitedStates::GovernmentBond));
    } else if (calstr == "UnitedStates/NERC") {
        pcal.reset(new UnitedStates(UnitedStates::NERC));

    } else {
        throw std::invalid_argument("Calendar " + calstr + " not recognised ");
    }

    return pcal;
}

RcppExport SEXP QL_setContext(SEXP parSEXP) {

    try {
        Rcpp::List par(parSEXP);        

        // set fixingDays and settleDate
        RQLContext::instance().fixingDays = Rcpp::as<int>(par["fixingDays"]);
        RQLContext::instance().settleDate = 
            Date(dateFromR( Rcpp::Date(Rcpp::as<int>(par["settleDate"])) ));

        boost::shared_ptr<Calendar> pcal( getCalendar(Rcpp::as<std::string>(par["calendar"])) );
        RQLContext::instance().calendar = *pcal; // set calendar in global singleton


    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }
    return R_NilValue;
}

RcppExport SEXP QL_isBusinessDay(SEXP calSexp, SEXP dateSexp){

    try {
        boost::shared_ptr<Calendar> pcal( getCalendar(Rcpp::as<std::string>(calSexp)) );

        Rcpp::DateVector dates  = Rcpp::DateVector(dateSexp);

        int n = dates.size();
        std::vector<int> bizdays(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates[i]) );
            bizdays[i] = pcal->isBusinessDay(day);
        }

        return Rcpp::wrap(bizdays);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }
    return R_NilValue;
}

RcppExport SEXP QL_isHoliday(SEXP calSexp, SEXP dateSexp){

    try {
        boost::shared_ptr<Calendar> pcal( getCalendar(Rcpp::as<std::string>(calSexp)) );

        Rcpp::DateVector dates  = Rcpp::DateVector(dateSexp);
        int n = dates.size();
        std::vector<int> hdays(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates[i]) );
            hdays[i] = pcal->isHoliday(day);
        }

        return Rcpp::wrap(hdays);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }
    return R_NilValue;
}

RcppExport SEXP QL_isWeekend(SEXP calSexp, SEXP dateSexp){

    try {
        boost::shared_ptr<Calendar> pcal( getCalendar(Rcpp::as<std::string>(calSexp)) );

        Rcpp::DateVector dates  = Rcpp::DateVector(dateSexp);
        int n = dates.size();
        std::vector<int> weekends(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates[i]) );
            weekends[i] = pcal->isWeekend(day.weekday());
        }

        return Rcpp::wrap(weekends);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP QL_isEndOfMonth(SEXP calSexp, SEXP dateSexp){

    try {
        boost::shared_ptr<Calendar> pcal( getCalendar(Rcpp::as<std::string>(calSexp)) );

        Rcpp::DateVector dates  = Rcpp::DateVector(dateSexp);
        int n = dates.size();
        std::vector<int> eom(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates[i]) );
            eom[i] = pcal->isEndOfMonth(day);
        }

        return Rcpp::wrap(eom);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP QL_endOfMonth(SEXP calSexp, SEXP dateSexp){

    try {
        boost::shared_ptr<Calendar> pcal( getCalendar(Rcpp::as<std::string>(calSexp)) );

        Rcpp::DateVector dates  = Rcpp::DateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> eom(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates[i]) );
            eom[i] = pcal->endOfMonth(day);
            dates[i] = Rcpp::Date(eom[i].month(), eom[i].dayOfMonth(), eom[i].year());
        }
       
        return Rcpp::wrap(dates);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP QL_adjust(SEXP calSexp, SEXP bdcSEXP, SEXP dateSexp){

    try {
        boost::shared_ptr<Calendar> pcal( getCalendar(Rcpp::as<std::string>(calSexp)) );
        BusinessDayConvention bdc = getBusinessDayConvention( Rcpp::as<double>(bdcSEXP) );
        Rcpp::DateVector dates  = Rcpp::DateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> adjusted(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates[i]) );
            adjusted[i] = pcal->adjust(day, bdc);
            dates[i] =  Rcpp::Date(adjusted[i].month(), 
                                   adjusted[i].dayOfMonth(), 
                                   adjusted[i].year());
        }

        return Rcpp::wrap(dates);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP QL_advance1(SEXP calSexp, SEXP params, SEXP dateSexp){

    try {
        boost::shared_ptr<Calendar> pcal( getCalendar(Rcpp::as<std::string>(calSexp)) );
        Rcpp::List rparam(params);        
        BusinessDayConvention bdc = getBusinessDayConvention( Rcpp::as<double>(rparam["bdc"]) );
        double emr = Rcpp::as<double>(rparam["emr"]);
        double amount = Rcpp::as<double>(rparam["amount"]);
        double unit = Rcpp::as<double>(rparam["unit"]);

        Rcpp::DateVector dates  = Rcpp::DateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> advance(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates[i]) );
            advance[i] = pcal->advance(day, amount,getTimeUnit(unit), bdc, (emr == 1)?true:false );
            dates[i] =  Rcpp::Date(advance[i].month(), 
                                   advance[i].dayOfMonth(), 
                                   advance[i].year());
        }
        
        return Rcpp::wrap(dates);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP QL_advance2(SEXP calSexp, SEXP param, SEXP dateSexp){

    try {
        boost::shared_ptr<Calendar> pcal( getCalendar(Rcpp::as<std::string>(calSexp)) );
        Rcpp::List rparam(param);        
        BusinessDayConvention bdc = getBusinessDayConvention( Rcpp::as<double>(rparam["bdc"]) );
        double emr = Rcpp::as<double>(rparam["emr"]);
        double period = Rcpp::as<double>(rparam["period"]);

        Rcpp::DateVector dates  = Rcpp::DateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> advance(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates[i]) );
            advance[i] = pcal->advance(day, Period(getFrequency(period)), 
                                       bdc, (emr == 1)?true:false );
            dates[i] =  Rcpp::Date(advance[i].month(), 
                                   advance[i].dayOfMonth(), 
                                   advance[i].year());
        }

        return Rcpp::wrap(dates);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP QL_businessDaysBetween(SEXP calSexp, SEXP params,
                                       SEXP from, SEXP to){

    try {
        boost::shared_ptr<Calendar> pcal( getCalendar(Rcpp::as<std::string>(calSexp)) );
        Rcpp::List rparam(params);
        double ifirst = Rcpp::as<double>(rparam["includeFirst"]);
        double ilast = Rcpp::as<double>(rparam["includeLast"]);

        Rcpp::DateVector dates1  = Rcpp::DateVector(from);
        Rcpp::DateVector dates2  = Rcpp::DateVector(to);

        int n = dates1.size();
        std::vector<double> between(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day1( dateFromR(dates1[i]) );
            QuantLib::Date day2( dateFromR(dates2[i]) );
            between[i] = pcal->businessDaysBetween(day1, day2,
                                                   (ifirst == 1) ? true: false,
                                                   (ilast == 1) ? true: false);
        }
        
        return Rcpp::wrap(between);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP QL_holidayList(SEXP calSexp, SEXP params) {

    try {
        boost::shared_ptr<Calendar> pcal( getCalendar(Rcpp::as<std::string>(calSexp)) );
        Rcpp::List rparam(params);
        double iw = Rcpp::as<double>(rparam["includeWeekends"]);
        Rcpp::Date d1 = Rcpp::Date( Rcpp::as<int>( rparam["from"] ));
        Rcpp::Date d2 = Rcpp::Date( Rcpp::as<int>( rparam["to"] ));

        std::vector<QuantLib::Date> 
            holidays = QuantLib::Calendar::holidayList(*pcal,
                                                       Date(dateFromR(d1)), 
                                                       Date(dateFromR(d2)), 
                                                       iw == 1 ? true : false);                

        if (holidays.size() > 0) {
            Rcpp::DateVector dv( holidays.size() );
            for (unsigned int i = 0; i< holidays.size(); i++){
                dv[i] = Rcpp::Date(holidays[i].month(), holidays[i].dayOfMonth(), holidays[i].year());
            }
            return Rcpp::wrap(dv);
        } else {
            return R_NilValue;
        }

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}
