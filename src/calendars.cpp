
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2002-2026  Dirk Eddelbuettel
//  Copyright (C) 2010       Dirk Eddelbuettel and Khanh Nguyen
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

    } else if (calstr == "UnitedStates" ||
               calstr == "UnitedStates/Settlement" ||
               calstr == "UnitedStates::Settlement") {
        pcal.reset(new QuantLib::UnitedStates(QuantLib::UnitedStates::Settlement));
    } else if (calstr == "UnitedStates/LiborImpact" ||
               calstr == "UnitedStates::LiborImpact") {
        pcal.reset(new QuantLib::UnitedStates(QuantLib::UnitedStates::LiborImpact));
    } else if (calstr == "UnitedStates/NYSE" ||
               calstr == "UnitedStates::NYSE") {
        pcal.reset(new QuantLib::UnitedStates(QuantLib::UnitedStates::NYSE));
    } else if (calstr == "UnitedStates/GovernmentBond" ||
               calstr == "UnitedStates::GovernmentBond") {
        pcal.reset(new QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond));
    } else if (calstr == "UnitedStates/NERC" ||
               calstr == "UnitedStates::NERC") {
        pcal.reset(new QuantLib::UnitedStates(QuantLib::UnitedStates::NERC));
    } else if (calstr == "UnitedStates/FederalReserve" ||
               calstr == "UnitedStates::FederalReserve") {
        pcal.reset(new QuantLib::UnitedStates(QuantLib::UnitedStates::FederalReserve));
    } else if (calstr == "UnitedStates/SOFR" ||
               calstr == "UnitedStates::SOFR") {
        pcal.reset(new QuantLib::UnitedStates(QuantLib::UnitedStates::SOFR));

    } else if (calstr == "Argentina") {
        pcal.reset(new QuantLib::Argentina());

    } else if (calstr == "Australia" ||
               calstr == "Australia/Settlement" ||
               calstr == "Australia::Settlement") {
        pcal.reset(new QuantLib::Australia(QuantLib::Australia::Settlement));
    } else if (calstr == "Australia/ASX" ||
               calstr == "Australia::ASX") {
        pcal.reset(new QuantLib::Australia(QuantLib::Australia::ASX));

    } else if (calstr == "Austria" ||
               calstr == "Austria/Settlement" ||
               calstr == "Austria::Settlement") {
        pcal.reset(new QuantLib::Austria(QuantLib::Austria::Settlement));
    } else if (calstr == "Austria/Exchange" ||
               calstr == "Austria::Exchange") {
        pcal.reset(new QuantLib::Austria(QuantLib::Austria::Exchange));

    } else if (calstr == "Bespoke") {
        pcal.reset(new QuantLib::BespokeCalendar());

    } else if (calstr == "Botswana") {
        pcal.reset(new QuantLib::Botswana());

    } else if (calstr == "Brazil" ||
               calstr == "Brazil/Settlement" ||
               calstr == "Brazil::Settlement") {
        pcal.reset(new QuantLib::Brazil(QuantLib::Brazil::Settlement));
    } else if (calstr == "Brazil/Exchange" ||
               calstr == "Brazil::Exchange") {
        pcal.reset(new QuantLib::Brazil(QuantLib::Brazil::Exchange));

    } else if (calstr == "Canada" ||
               calstr == "Canada/Settlement" ||
               calstr == "Canada::Settlement") {
        pcal.reset(new QuantLib::Canada(QuantLib::Canada::Settlement));
    } else if (calstr == "Canada/TSX" ||
               calstr == "Canada::TSX") {
        pcal.reset(new QuantLib::Canada(QuantLib::Canada::TSX));

    } else if (calstr == "Chile") {
        pcal.reset(new QuantLib::Chile());

    } else if (calstr == "China" ||
               calstr == "China/SSE" ||
               calstr == "China::SSE") {
        pcal.reset(new QuantLib::China(QuantLib::China::SSE));
    } else if (calstr == "China/IB" ||
               calstr == "China::IB") {
        pcal.reset(new QuantLib::China(QuantLib::China::IB));

#if QL_HEX_VERSION >= 0x014300f0
    } else if (calstr == "Croatia") {
        pcal.reset(new QuantLib::Croatia());
#endif

    } else if (calstr == "CzechRepublic") {
        pcal.reset(new QuantLib::CzechRepublic());

    } else if (calstr == "Denmark") {
        pcal.reset(new QuantLib::Denmark());

    } else if (calstr == "Finland") {
        pcal.reset(new QuantLib::Finland());

    } else if (calstr == "France" ||
               calstr == "France/Settlement" ||
               calstr == "France::Settlement") {
        pcal.reset(new QuantLib::France(QuantLib::France::Settlement));
    } else if (calstr == "France/Exchange" ||
               calstr == "France::Exchange") {
        pcal.reset(new QuantLib::France(QuantLib::France::Exchange));

    } else if (calstr == "Germany" ||
               calstr == "Germany/Settlement" ||
               calstr == "Germany::Settlement") {
        pcal.reset(new QuantLib::Germany(QuantLib::Germany::Settlement));
    } else if (calstr == "Germany/FrankfurtStockExchange" ||
               calstr == "Germany::FrankfurtStockExchange") {
        pcal.reset(new QuantLib::Germany(QuantLib::Germany::FrankfurtStockExchange));
    } else if (calstr == "Germany/Xetra" ||
               calstr == "Germany::Xetra") {
        pcal.reset(new QuantLib::Germany(QuantLib::Germany::Xetra));
    } else if (calstr == "Germany/Eurex" ||
               calstr == "Germany::Eurex") {
        pcal.reset(new QuantLib::Germany(QuantLib::Germany::Eurex));
    } else if (calstr == "Germany/Euwax" ||
               calstr == "Germany::Euwax") {
        pcal.reset(new QuantLib::Germany(QuantLib::Germany::Euwax));

    } else if (calstr == "HongKong") {
        pcal.reset(new QuantLib::HongKong());

    } else if (calstr == "Hungary") {
        pcal.reset(new QuantLib::Hungary());

    } else if (calstr == "Iceland") {
        pcal.reset(new QuantLib::Iceland());

    } else if (calstr == "India") {
        pcal.reset(new QuantLib::India());

    } else if (calstr == "Indonesia" ||
               calstr == "Indonesia/IDX" ||
               calstr == "Indonesia::IDX") {
        pcal.reset(new QuantLib::Indonesia(QuantLib::Indonesia::IDX));
    } else if (calstr == "Indonesia/BEJ" ||
               calstr == "Indonesia::BEJ") {
        pcal.reset(new QuantLib::Indonesia(QuantLib::Indonesia::BEJ));
    } else if (calstr == "Indonesia/JSX" ||
               calstr == "Indonesia::JSX") {
        pcal.reset(new QuantLib::Indonesia(QuantLib::Indonesia::JSX));

#if QL_HEX_VERSION <= 0x014201f0
    } else if (calstr == "Israel" || calstr == "Israel/Settlement") {
        pcal.reset(new QuantLib::Israel(QuantLib::Israel::Settlement));
    } else if (calstr == "Israel/TASE") {
        pcal.reset(new QuantLib::Israel(QuantLib::Israel::TASE));
#else
    } else if (calstr == "Israel" || calstr == "Israel/TASE") {
        pcal.reset(new QuantLib::Israel(QuantLib::Israel::TASE));
    } else if (calstr == "Israel/SHIR") {
        pcal.reset(new QuantLib::Israel(QuantLib::Israel::SHIR));
    } else if (calstr == "Israel/Telbor") {
        pcal.reset(new QuantLib::Israel(QuantLib::Israel::Telbor));
#endif

    } else if (calstr == "Italy" ||
               calstr == "Italy/Settlement" ||
               calstr == "Italy::Settlement") {
        pcal.reset(new QuantLib::Italy(QuantLib::Italy::Settlement));
    } else if (calstr == "Italy/Exchange" ||
               calstr == "Italy::Exchange") {
        pcal.reset(new QuantLib::Italy(QuantLib::Italy::Exchange));

    } else if (calstr == "Japan" || calstr == "Japan/Settlement") {
        pcal.reset(new QuantLib::Japan());

#if QL_HEX_VERSION >= 0x014300f0
    } else if (calstr == "Malta") {
        pcal.reset(new QuantLib::Malta());
#endif

    } else if (calstr == "Mexico") {
        pcal.reset(new QuantLib::Mexico());

#if QL_HEX_VERSION >= 0x014300f0
    } else if (calstr == "Montenegro") {
        pcal.reset(new QuantLib::Montenegro());
#endif

#if QL_HEX_VERSION >= 0x013700f0
    } else if (calstr == "NewZealand" ||
               calstr == "NewZealand/Wellington") {
        pcal.reset(new QuantLib::NewZealand(QuantLib::NewZealand::Wellington));
    } else if (calstr == "NewZealand/Auckland") {
        pcal.reset(new QuantLib::NewZealand(QuantLib::NewZealand::Auckland));
#else
    } else if (calstr == "NewZealand") {
        pcal.reset(new QuantLib::NewZealand());
#endif

#if QL_HEX_VERSION >= 0x014300f0
    } else if (calstr == "NorthMacedonia") {
        pcal.reset(new QuantLib::NorthMacedonia());
#endif

    } else if (calstr == "Norway") {
        pcal.reset(new QuantLib::Norway());

    } else if (calstr == "Null" ||
               calstr == "null" ||
               calstr == "NULL") {
        pcal.reset(new QuantLib::NullCalendar());

#if QL_HEX_VERSION >= 0x013600f0
    } else if (calstr == "Poland" ||
               calstr == "Poland/Settlement" ||
               calstr == "Poland::Settlement") {
        pcal.reset(new QuantLib::Poland(QuantLib::Poland::Settlement));
    } else if (calstr == "Poland/WSE" ||
               calstr == "Poland::WSE") {
        pcal.reset(new QuantLib::Poland(QuantLib::Poland::WSE));
#else
    } else if (calstr == "Poland") {
        pcal.reset(new QuantLib::Poland());
#endif

    } else if (calstr == "Romania" ||
               calstr == "Romania/Public" ||
               calstr == "Romania::Public") {
        pcal.reset(new QuantLib::Romania(QuantLib::Romania::Public));
    } else if (calstr == "Romania/BVB" ||
               calstr == "Romania::BVB") {
        pcal.reset(new QuantLib::Romania(QuantLib::Romania::BVB));

    } else if (calstr == "Russia" ||
               calstr == "Russia/Settlement" ||
               calstr == "Russia::Settlement") {
        pcal.reset(new QuantLib::Russia(QuantLib::Russia::Settlement));
    } else if (calstr == "Russia/MOEX" ||
               calstr == "Russia::MOEX") {
        pcal.reset(new QuantLib::Russia(QuantLib::Russia::MOEX));

    } else if (calstr == "SaudiArabia") {
        pcal.reset(new QuantLib::SaudiArabia());

#if QL_HEX_VERSION >= 0x014300f0
    } else if (calstr == "Serbia") {
        pcal.reset(new QuantLib::Serbia());
#endif

    } else if (calstr == "Singapore") {
        pcal.reset(new QuantLib::Singapore());

    } else if (calstr == "Slovakia") {
        pcal.reset(new QuantLib::Slovakia());

#if QL_HEX_VERSION >= 0x014300f0
    } else if (calstr == "Slovenia") {
        pcal.reset(new QuantLib::Slovenia());
#endif

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

    } else if (calstr == "Thailand") {
        pcal.reset(new QuantLib::Thailand());

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

#if QL_HEX_VERSION >= 0x014300f0
    } else if (calstr == "Uzbekistan") {
        pcal.reset(new QuantLib::Uzbekistan());
#endif

    } else if (calstr == "WeekendsOnly") {
        pcal.reset(new QuantLib::WeekendsOnly());

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
std::vector<QuantLib::Date> getBusinessDayList(std::string calendar,
                                               QuantLib::Date from, QuantLib::Date to) {

    QuantLib::ext::shared_ptr<QuantLib::Calendar> pcal(getCalendar(calendar));
    std::vector<QuantLib::Date> bizdays;
#if QL_HEX_VERSION >= 0x011800f0
    bizdays = pcal->businessDayList(from, to);
#endif
    return bizdays;
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
