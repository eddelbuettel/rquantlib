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

#include <rquantlib.hpp>

Calendar* getCalendar(const std::string &calstr) {
    Calendar* pcal = NULL;

    if (calstr == "TARGET") { 		// generic calendar 
        pcal = new TARGET();
        
    } else if (calstr == "Brazil") {
        pcal = new Brazil();

    } else if (calstr == "Canada" || calstr == "Canada/Settlement") {
        pcal = new Canada(Canada::Settlement);
    } else if (calstr == "Canada/TSX") {
        pcal = new Canada(Canada::TSX);
        
    } else if (calstr == "Germany" || calstr == "Germany/FrankfurtStockExchange") {
        pcal = new Germany(Germany::FrankfurtStockExchange);
    } else if (calstr == "Germany/Settlement") {
        pcal = new Germany(Germany::Settlement);
    } else if (calstr == "Germany/Xetra") {
        pcal = new Germany(Germany::Xetra);
        } else if (calstr == "Germany/Eurex") {
        pcal = new Germany(Germany::Eurex);
        
    } else if (calstr == "Italy" || calstr == "Italy/Settlement") {
        pcal = new Italy(Italy::Settlement);
        } else if (calstr == "Italy/Exchange") {
        pcal = new Italy(Italy::Exchange);
        
    } else if (calstr == "Japan" || calstr == "Japan/Settlement") {
        pcal = new Japan();
        
    } else if (calstr == "SouthKorea" || calstr == "SouthKorea/Settlement") {
        pcal = new SouthKorea(SouthKorea::Settlement);
    } else if (calstr == "SouthKorea/KRX") {
        pcal = new SouthKorea(SouthKorea::KRX);

    } else if (calstr == "UnitedKingdom" || calstr == "UnitedKingdom/Settlement") {
        pcal = new UnitedKingdom(UnitedKingdom::Settlement);
        } else if (calstr == "UnitedKingdom/Exchange") {
        pcal = new UnitedKingdom(UnitedKingdom::Exchange);
    } else if (calstr == "UnitedKingdom/Metals") {
        pcal = new UnitedKingdom(UnitedKingdom::Metals);
        
    } else if (calstr == "UnitedStates" || calstr == "UnitedStates/Settlement") {
        pcal = new UnitedStates(UnitedStates::Settlement);
    } else if (calstr == "UnitedStates/NYSE") {
        pcal = new UnitedStates(UnitedStates::NYSE);
        } else if (calstr == "UnitedStates/GovernmentBond") {
        pcal = new UnitedStates(UnitedStates::GovernmentBond);
    } else if (calstr == "UnitedStates/NERC") {
        pcal = new UnitedStates(UnitedStates::NERC);

    } else {
        throw std::invalid_argument("Calendar " + calstr + " not recognised ");
    }

    return pcal;
}

RcppExport SEXP QL_isBusinessDay(SEXP calSexp, SEXP dateSexp){

    try {
        Calendar *pcal = getCalendar(Rcpp::as<std::string>(calSexp));

        RcppDateVector dates  = RcppDateVector(dateSexp);

        int n = dates.size();
        std::vector<int> bizdays(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            bizdays[i] = pcal->isBusinessDay(day);
        }
        delete pcal;

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
        Calendar* pcal = getCalendar(Rcpp::as<std::string>(calSexp));

        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<int> hdays(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            hdays[i] = pcal->isHoliday(day);
        }
        delete pcal;

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
        Calendar* pcal = getCalendar(Rcpp::as<std::string>(calSexp));

        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<int> weekends(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            weekends[i] = pcal->isWeekend(day.weekday());
        }
        delete pcal;

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
        Calendar* pcal = getCalendar(Rcpp::as<std::string>(calSexp));

        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<int> eom(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            eom[i] = pcal->isEndOfMonth(day);
        }
        delete pcal;

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
        Calendar* pcal = getCalendar(Rcpp::as<std::string>(calSexp));

        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> eom(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            eom[i] = pcal->endOfMonth(day);
            dates(i) = RcppDate(eom[i].month(), eom[i].dayOfMonth(), eom[i].year());
        }
        delete pcal;
       
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
        Calendar* pcal = getCalendar(Rcpp::as<std::string>(calSexp));
        BusinessDayConvention bdc = getBusinessDayConvention( Rcpp::as<double>(bdcSEXP) );
        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> adjusted(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            adjusted[i] = pcal->adjust(day, bdc);
            dates(i) =  RcppDate(adjusted[i].month(), 
                                 adjusted[i].dayOfMonth(), 
                                 adjusted[i].year());
        }
        delete pcal;        

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
        Calendar* pcal = getCalendar(Rcpp::as<std::string>(calSexp));
        Rcpp::List rparam(params);        
        BusinessDayConvention bdc = getBusinessDayConvention(rparam["bdc"]);
        double emr = rparam["emr"];
        double amount = rparam["amount"];
        double unit = rparam["unit"];

        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> advance(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            advance[i] = pcal->advance(day, amount,getTimeUnit(unit), bdc, (emr == 1)?true:false );
            dates(i) =  RcppDate(advance[i].month(), 
                                 advance[i].dayOfMonth(), 
                                 advance[i].year());
        }
        delete pcal;        
        
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
        Calendar* pcal = getCalendar(Rcpp::as<std::string>(calSexp));
        Rcpp::List rparam(param);        
        BusinessDayConvention bdc = getBusinessDayConvention(rparam["bdc"]);
        double emr = rparam["emr"];
        double period = rparam["period"];

        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> advance(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            advance[i] = pcal->advance(day, Period(getFrequency(period)), 
                                       bdc, (emr == 1)?true:false );
            dates(i) =  RcppDate(advance[i].month(), 
                                 advance[i].dayOfMonth(), 
                                 advance[i].year());
        }
        delete pcal;        

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
        Calendar* pcal = getCalendar(Rcpp::as<std::string>(calSexp));
        Rcpp::List rparam(params);
        double ifirst = rparam["includeFirst"];
        double ilast = rparam["includeLast"];

        RcppDateVector dates1  = RcppDateVector(from);
        RcppDateVector dates2  = RcppDateVector(to);

        int n = dates1.size();
        std::vector<double> between(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day1( dateFromR(dates1(i)) );
            QuantLib::Date day2( dateFromR(dates2(i)) );
            between[i] = pcal->businessDaysBetween(day1, day2,
                                                   (ifirst == 1) ? true: false,
                                                   (ilast == 1) ? true: false);
        }
        delete pcal;        
        
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
        Calendar* pcal = getCalendar(Rcpp::as<std::string>(calSexp));
        Rcpp::List rparam(params);
        double iw = rparam["includeWeekends"];
        RcppDate d1 = RcppDate( (int) rparam["from"] );
        RcppDate d2 = RcppDate( (int) rparam["to"] );

        std::vector<QuantLib::Date> 
            holidays = QuantLib::Calendar::holidayList(*pcal,
                                                       Date(dateFromR(d1)), 
                                                       Date(dateFromR(d2)), 
                                                       iw == 1 ? true : false);                

        RcppDateVector dv( holidays.size() );
        for (unsigned int i = 0; i< holidays.size(); i++){
            dv.set(i, RcppDate(holidays[i].month(), holidays[i].dayOfMonth(), holidays[i].year()));
        }
        delete pcal;
        return Rcpp::wrap(dv);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


/*

RcppExport SEXP QL_isBusinessDay(SEXP calParameters, SEXP dateSexp){

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;

    try {
        Calendar* pcal = getCalendar(calParameters);
        delete pcal;

        RcppResultSet rs;
        rs.add("bizdays", bizdays);
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


*/
