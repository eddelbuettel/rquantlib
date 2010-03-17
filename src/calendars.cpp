// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2002 - 2009 Dirk Eddelbuettel <edd@debian.org>
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

#include "rquantlib.hpp"




RcppExport SEXP QL_isBusinessDay(SEXP calParameters, SEXP dateSexp){

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;

    try {
        RcppParams rparam(calParameters);

        std::string    calstr = rparam.getStringValue("calendar");
        RcppDateVector dates  = RcppDateVector(dateSexp);

        Calendar *pcal = NULL;

        if (calstr == "TARGET") { 		// generic calendar 
            pcal = new TARGET();

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

        } else if (calstr == "Japan") {
            pcal = new Japan();

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

        int n = dates.size();
        std::vector<int> bizdays(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            bizdays[i] = pcal->isBusinessDay(day);
        }

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


RcppExport SEXP QL_isHoliday(SEXP calParameters, SEXP dateSexp){

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;

    try {
        Calendar* pcal = getCalendar(calParameters);

        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<int> hdays(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            hdays[i] = pcal->isHoliday(day);
        }


        delete pcal;

        RcppResultSet rs;
        rs.add("holidays", hdays);
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




RcppExport SEXP QL_isWeekend(SEXP calParameters, SEXP dateSexp){

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;

    try {
        Calendar* pcal = getCalendar(calParameters);

        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<int> weekends(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            weekends[i] = pcal->isWeekend(day.weekday());
        }


        delete pcal;

        RcppResultSet rs;
        rs.add("weekend", weekends);
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


RcppExport SEXP QL_isEndOfMonth(SEXP calParameters, SEXP dateSexp){

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;

    try {
        Calendar* pcal = getCalendar(calParameters);

        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<int> eom(n);

        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            eom[i] = pcal->isEndOfMonth(day);
        }


        delete pcal;

        RcppResultSet rs;
        rs.add("End.Of.Month", eom);
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


RcppExport SEXP QL_endOfMonth(SEXP calParameters, SEXP dateSexp){

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;

    try {
        Calendar* pcal = getCalendar(calParameters);

        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> eom(n);

        RcppDateVector ret(dateSexp);
        RcppResultSet rs;
        
        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            eom[i] = pcal->endOfMonth(day);
            ret(i) =  RcppDate(eom[i].month(), eom[i].dayOfMonth(), eom[i].year());
        }
        
        rs.add("ret", ret);
        delete pcal;
        

       
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



RcppExport SEXP QL_adjust(SEXP calParameters, SEXP dateSexp){

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;

    try {
        Calendar* pcal = getCalendar(calParameters);
        RcppParams rparam(calParameters);        
        BusinessDayConvention bdc = getBusinessDayConvention(rparam.getDoubleValue("bdc"));

        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> adjusted(n);

        RcppDateVector ret(dateSexp);
        RcppResultSet rs;
        
        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            adjusted[i] = pcal->adjust(day, bdc);
            ret(i) =  RcppDate(adjusted[i].month(), 
                               adjusted[i].dayOfMonth(), 
                               adjusted[i].year());
        }
        
        rs.add("ret", ret);
        delete pcal;        
       
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


RcppExport SEXP QL_advance1(SEXP calParameters, SEXP dateSexp){

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;

    try {
        Calendar* pcal = getCalendar(calParameters);
        RcppParams rparam(calParameters);        
        BusinessDayConvention bdc = getBusinessDayConvention(rparam.getDoubleValue("bdc"));
        double emr = rparam.getDoubleValue("emr");
        double amount = rparam.getDoubleValue("amount");
        double unit = rparam.getDoubleValue("unit");


        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> advance(n);

        RcppDateVector ret(dateSexp);
        RcppResultSet rs;
        
        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            advance[i] = pcal->advance(day, amount,getTimeUnit(unit), bdc, (emr == 1)?true:false );
            ret(i) =  RcppDate(advance[i].month(), 
                               advance[i].dayOfMonth(), 
                               advance[i].year());
        }
        
        rs.add("ret", ret);
        delete pcal;        
       
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


RcppExport SEXP QL_advance2(SEXP calParameters, SEXP dateSexp){

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;

    try {
        Calendar* pcal = getCalendar(calParameters);
        RcppParams rparam(calParameters);        
        BusinessDayConvention bdc = getBusinessDayConvention(rparam.getDoubleValue("bdc"));
        double emr = rparam.getDoubleValue("emr");
        double period = rparam.getDoubleValue("period");


        RcppDateVector dates  = RcppDateVector(dateSexp);
        int n = dates.size();
        std::vector<QuantLib::Date> advance(n);

        RcppDateVector ret(dateSexp);
        RcppResultSet rs;
        
        for (int i=0; i<n; i++) {
            QuantLib::Date day( dateFromR(dates(i)) );
            advance[i] = pcal->advance(day, Period(getFrequency(period)), 
                                       bdc, (emr == 1)?true:false );
            ret(i) =  RcppDate(advance[i].month(), 
                               advance[i].dayOfMonth(), 
                               advance[i].year());
        }
        
        rs.add("ret", ret);
        delete pcal;        
       
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
RcppExport SEXP QL_businessDaysBetween(SEXP calParameters, 
                                       SEXP from, SEXP to){

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;

    try {
        Calendar* pcal = getCalendar(calParameters);
        RcppParams rparam(calParameters);
        double ifirst = rparam.getDoubleValue("includeFirst");
        double ilast = rparam.getDoubleValue("includeLast");

        RcppDateVector dates1  = RcppDateVector(from);
        RcppDateVector dates2  = RcppDateVector(to);

        int n = dates1.size();
        std::vector<double> between(n);


        RcppResultSet rs;
        
        for (int i=0; i<n; i++) {
            QuantLib::Date day1( dateFromR(dates1(i)) );
            QuantLib::Date day2( dateFromR(dates2(i)) );
            between[i] = pcal->businessDaysBetween(day1, day2,
                                                   (ifirst == 1)?true:false,
                                                   (ilast ==1)?true:false);
        }
        
        rs.add("ret", between);
        delete pcal;        
        
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


RcppExport SEXP QL_holidayList(SEXP calParameters, 
                                 SEXP from, 
                                 SEXP to){

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;

    try {
        Calendar* pcal = getCalendar(calParameters);
        RcppParams rparam(calParameters);
        double iw = rparam.getDoubleValue("includeWeekends");

        RcppDate d1 = rparam.getDateValue("from");
        RcppDate d2 = rparam.getDateValue("to");

        std::vector<QuantLib::Date> 
            holidays = QuantLib::Calendar::holidayList(*pcal,
                                                       Date(dateFromR(d1)), 
                                                       Date(dateFromR(d2)), (iw == 1)?true:false);                

        int numCol = 1;
        std::vector<std::string> colNames(numCol);
        colNames[0] = "Date";
        RcppFrame frame(colNames);

        for (unsigned int i = 0; i< holidays.size(); i++){
            std::vector<ColDatum> row(numCol);

            row[0].setDateValue(RcppDate(holidays[i].month(), 
                                         holidays[i].dayOfMonth(), 
                                         holidays[i].year()));
 
            frame.addRow(row);
        }

        delete pcal;
        RcppResultSet rs;
        if (holidays.size() > 0)
            rs.add("ret", frame);
        else 
            rs.add("ret", -1);
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
