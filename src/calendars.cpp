
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2002 - 2020  Dirk Eddelbuettel
//  Copyright (C) 2010         Dirk Eddelbuettel and Khanh Nguyen
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

// returns a reference-counted pointer to a matching Calendar object
QuantLib::ext::shared_ptr<QuantLib::Calendar> getCalendar(const std::string &calstr) {
    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal;

    if (calstr == "TARGET") { 		// generic calendar
        pcal.reset(new QuantLib::TARGET());

    } else if (calstr == "Argentina") {
        pcal.reset(new QuantLib::Argentina());

    } else if (calstr == "Australia") {
        pcal.reset(new QuantLib::Australia());

    } else if (calstr == "Brazil") {
        pcal.reset(new QuantLib::Brazil());

    } else if (calstr == "Canada" || calstr == "Canada/Settlement") {
        pcal.reset(new QuantLib::Canada(QuantLib::Canada::Settlement));
    } else if (calstr == "Canada/TSX") {
        pcal.reset(new QuantLib::Canada(QuantLib::Canada::TSX));

    } else if (calstr == "China") {
        pcal.reset(new QuantLib::China());

    } else if (calstr == "CzechRepublic") {
        pcal.reset(new QuantLib::CzechRepublic());

    } else if (calstr == "Denmark") {
        pcal.reset(new QuantLib::Denmark());

    } else if (calstr == "Finland") {
        pcal.reset(new QuantLib::Finland());

    } else if (calstr == "Germany" || calstr == "Germany/FrankfurtStockExchange") {
        pcal.reset(new QuantLib::Germany(QuantLib::Germany::FrankfurtStockExchange));
    } else if (calstr == "Germany/Settlement") {
        pcal.reset(new QuantLib::Germany(QuantLib::Germany::Settlement));
    } else if (calstr == "Germany/Xetra") {
        pcal.reset(new QuantLib::Germany(QuantLib::Germany::Xetra));
    } else if (calstr == "Germany/Eurex") {
        pcal.reset(new QuantLib::Germany(QuantLib::Germany::Eurex));

    } else if (calstr == "HongKong") {
        pcal.reset(new QuantLib::HongKong());

    } else if (calstr == "Hungary") {
        pcal.reset(new QuantLib::Hungary());

    } else if (calstr == "Iceland") {
        pcal.reset(new QuantLib::Iceland());

    } else if (calstr == "India") {
        pcal.reset(new QuantLib::India());

    } else if (calstr == "Indonesia") {
        pcal.reset(new QuantLib::Indonesia());

    } else if (calstr == "Italy" || calstr == "Italy/Settlement") {
        pcal.reset(new QuantLib::Italy(QuantLib::Italy::Settlement));
    } else if (calstr == "Italy/Exchange") {
        pcal.reset(new QuantLib::Italy(QuantLib::Italy::Exchange));

    } else if (calstr == "Japan" || calstr == "Japan/Settlement") {
        pcal.reset(new QuantLib::Japan());

    } else if (calstr == "Mexico") {
        pcal.reset(new QuantLib::Mexico());

    } else if (calstr == "NewZealand") {
        pcal.reset(new QuantLib::NewZealand());

    } else if (calstr == "Norway") {
        pcal.reset(new QuantLib::Norway());

    } else if (calstr == "Poland") {
        pcal.reset(new QuantLib::Poland());

    } else if (calstr == "Russia") {
        pcal.reset(new QuantLib::Russia());

    } else if (calstr == "SaudiArabia") {
        pcal.reset(new QuantLib::SaudiArabia());

    } else if (calstr == "Singapore") {
        pcal.reset(new QuantLib::Singapore());

    } else if (calstr == "Slovakia") {
        pcal.reset(new QuantLib::Slovakia());

    } else if (calstr == "SouthAfrica") {
        pcal.reset(new QuantLib::SouthAfrica());

    } else if (calstr == "SouthKorea" || calstr == "SouthKorea/Settlement") {
        pcal.reset(new QuantLib::SouthKorea(QuantLib::SouthKorea::Settlement));
    } else if (calstr == "SouthKorea/KRX") {
        pcal.reset(new QuantLib::SouthKorea(QuantLib::SouthKorea::KRX));

    } else if (calstr == "Sweden") {
        pcal.reset(new QuantLib::Sweden());

    } else if (calstr == "Switzerland") {
        pcal.reset(new QuantLib::Switzerland());

    } else if (calstr == "Taiwan") {
        pcal.reset(new QuantLib::Taiwan());

    } else if (calstr == "Turkey") {
        pcal.reset(new QuantLib::Turkey());

    } else if (calstr == "Ukraine") {
        pcal.reset(new QuantLib::Ukraine());

    } else if (calstr == "UnitedKingdom" || calstr == "UnitedKingdom/Settlement") {
        pcal.reset(new QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Settlement));
    } else if (calstr == "UnitedKingdom/Exchange") {
        pcal.reset(new QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange));
    } else if (calstr == "UnitedKingdom/Metals") {
        pcal.reset(new QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Metals));

    } else if (calstr == "UnitedStates" || calstr == "UnitedStates/Settlement") {
        pcal.reset(new QuantLib::UnitedStates(QuantLib::UnitedStates::Settlement));
    } else if (calstr == "UnitedStates/NYSE") {
        pcal.reset(new QuantLib::UnitedStates(QuantLib::UnitedStates::NYSE));
    } else if (calstr == "UnitedStates/GovernmentBond") {
        pcal.reset(new QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond));
    } else if (calstr == "UnitedStates/NERC") {
        pcal.reset(new QuantLib::UnitedStates(QuantLib::UnitedStates::NERC));

    } else if (calstr == "WeekendsOnly") {
        pcal.reset(new QuantLib::WeekendsOnly());

    } else if (calstr == "Null" || calstr == "null" || calstr == "NULL") {
        pcal.reset(new QuantLib::NullCalendar());

    } else {
        throw std::invalid_argument("Calendar " + calstr + " not recognised ");
    }

    return pcal;
}

//setCalendarContext <- function(calendar="TARGET",
//                               fixingDays = 2,
//                               settleDate = Sys.Date() + 2) {

// [[Rcpp::export]]
bool setCalendarContext(std::string calendar, int fixingDays, QuantLib::Date settleDate) {

    // Rcpp Attribute cannot reflect complicated default arguments
    if (settleDate.serialNumber() == 0) {
        calendar = "TARGET";
        fixingDays = 2;
        settleDate = QuantLib::Date::todaysDate() + 2;
    }
    // set fixingDays and settleDate
    RQLContext::instance().fixingDays = fixingDays;
    RQLContext::instance().settleDate = settleDate;

    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    RQLContext::instance().calendar = *pcal; // set calendar in global singleton

    return true;
}

// [[Rcpp::export]]
std::vector<bool> isBusinessDay(std::string calendar, std::vector<QuantLib::Date> dates) {
    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    int n = dates.size();
    std::vector<bool> bizdays(n);
    for (int i=0; i<n; i++) {
        bizdays[i] = pcal->isBusinessDay(dates[i]);
    }
    return bizdays;
}

// [[Rcpp::export]]
std::vector<bool> isHoliday(std::string calendar, std::vector<QuantLib::Date> dates) {
    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    int n = dates.size();
    std::vector<bool> hdays(n);
    for (int i=0; i<n; i++) {
        hdays[i] = pcal->isHoliday(dates[i]);
    }
    return hdays;
}

// [[Rcpp::export]]
std::vector<bool> isWeekend(std::string calendar, std::vector<QuantLib::Date> dates) {
    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    int n = dates.size();
    std::vector<bool> weekends(n);
    for (int i=0; i<n; i++) {
        weekends[i] = pcal->isWeekend(dates[i].weekday());
    }
    return weekends;
}

// [[Rcpp::export]]
std::vector<bool> isEndOfMonth(std::string calendar, std::vector<QuantLib::Date> dates) {
    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    int n = dates.size();
    std::vector<bool> eom(n);
    for (int i=0; i<n; i++) {
        eom[i] = pcal->isEndOfMonth(dates[i]);
    }
    return eom;
}

// [[Rcpp::export]]
std::vector<QuantLib::Date> getEndOfMonth(std::string calendar, std::vector<QuantLib::Date> dates) {
    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    int n = dates.size();
    std::vector<QuantLib::Date> ndates(n);
    for (int i=0; i<n; i++) {
        ndates[i] = pcal->endOfMonth(dates[i]);
    }
    return ndates;
}

// [[Rcpp::export]]
std::vector<QuantLib::Date> adjust(std::string calendar, std::vector<QuantLib::Date> dates, int bdc=0) {
    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    QuantLib::BusinessDayConvention bdcval = getBusinessDayConvention(bdc);
    int n = dates.size();
    std::vector<QuantLib::Date> adjusted(n);
    for (int i=0; i<n; i++) {
        adjusted[i] = pcal->adjust(dates[i], bdcval);
    }
    return adjusted;
}

// [[Rcpp::export]]
std::vector<QuantLib::Date> advance1(std::string calendar,
         double amount, double unit, int bdcVal, double emr,
         std::vector<QuantLib::Date> dates) {

    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(bdcVal);
    int n = dates.size();
    std::vector<QuantLib::Date> advance(n);

    for (int i=0; i<n; i++) {
        advance[i] = pcal->advance(dates[i], amount, getTimeUnit(unit), bdc, (emr == 1) ? true : false);
    }
    return advance;
}

// [[Rcpp::export]]
std::vector<QuantLib::Date> advance2(std::string calendar,
                                     double period, int bdcVal, double emr,
                                     std::vector<QuantLib::Date> dates) {

    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(bdcVal);
    int n = dates.size();
    std::vector<QuantLib::Date> advance(n);

    for (int i=0; i<n; i++) {
        advance[i] = pcal->advance(dates[i], QuantLib::Period(getFrequency(period)),
                                   bdc, (emr == 1) ? true : false);
    }
    return advance;
}

// [[Rcpp::export]]
std::vector<double> businessDaysBetween(std::string calendar,
                                        std::vector<QuantLib::Date> from,
                                        std::vector<QuantLib::Date> to,
                                        bool includeFirst=true, bool includeLast=false) {
    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    int n = from.size();
    std::vector<double> between(n);
    for (int i=0; i<n; i++) {
        between[i] = pcal->businessDaysBetween(from[i], to[i], includeFirst, includeLast);
    }
    return between;
}

// [[Rcpp::export]]
std::vector<QuantLib::Date> getHolidayList(std::string calendar,
                                           QuantLib::Date from, QuantLib::Date to,
                                           bool includeWeekends=false) {

    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
#if QL_HEX_VERSION >= 0x011800f0
    std::vector<QuantLib::Date> holidays = pcal->holidayList(from, to, includeWeekends);
#else
    std::vector<QuantLib::Date> holidays =
        QuantLib::Calendar::holidayList(*pcal, from, to, includeWeekends);
#endif
    return holidays;
}

// [[Rcpp::export]]
void addHolidays(std::string calendar, std::vector<QuantLib::Date> dates) {
    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    int n = dates.size();
    for (int i=0; i<n; i++) {
        pcal->addHoliday(dates[i]);
    }
}

// [[Rcpp::export]]
void removeHolidays(std::string calendar, std::vector<QuantLib::Date> dates) {
    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    int n = dates.size();
    for (int i=0; i<n; i++) {
        pcal->removeHoliday(dates[i]);
    }
}
