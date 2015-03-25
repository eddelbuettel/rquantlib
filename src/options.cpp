#include <ql/quantlib.hpp>
#include <Rcpp.h>
#include <vector>
#include <iostream>
#include <ql/time/calendars/target.hpp>
#include <ql/math/matrix.hpp>
using namespace Rcpp;
using namespace QuantLib;
#include "rquantlib.h"

// [[Rcpp::depends(RQuantLib)]]

//boost::shared_ptr<QuantLib::YieldTermStructure>    flatRate(const QuantLib::Date& today,
//const boost::shared_ptr<QuantLib::Quote>& forward,
//const QuantLib::DayCounter& dc) {
//  return boost::shared_ptr<QuantLib::YieldTermStructure>(
//    new QuantLib::FlatForward(today, QuantLib::Handle<QuantLib::Quote>(forward), dc));
//}
//
//boost::shared_ptr<QuantLib::BlackVolTermStructure> flatVol(const QuantLib::Date& today,const boost::shared_ptr<QuantLib::Quote>& vol,const QuantLib::DayCounter& dc) {
//  return boost::shared_ptr<QuantLib::BlackVolTermStructure>(new
//  QuantLib::BlackConstantVol(today, QuantLib::NullCalendar(), QuantLib::Handle<QuantLib::Quote>(vol), dc));
//}
//
boost::shared_ptr<QuantLib::BlackVolTermStructure> volSurface(const QuantLib::Date& today,const std::vector<QuantLib::Date> &expirations, const std::vector<QuantLib::Real>& strikes,const QuantLib::Matrix& volMatrix, const QuantLib::DayCounter& dc) {
   const  QuantLib::Calendar calendar =  QuantLib::TARGET();

  boost::shared_ptr<BlackVarianceSurface> volatilitySurface(new QuantLib::BlackVarianceSurface(today,calendar, expirations, strikes, volMatrix, dc));
  
    // Volatility surface interpolation
	volatilitySurface->enableExtrapolation(true);

	// Change interpolator to bicubic splines
	volatilitySurface->setInterpolation<Bicubic>(Bicubic());
  
  return volatilitySurface;
  
}
//
//
//QuantLib::Option::Type getOptionType(const std::string &type) {
//  QuantLib::Option::Type optionType;
//  if (type=="call") {
//    optionType = QuantLib::Option::Call;
//  } else if (type=="put") {
//    optionType = QuantLib::Option::Put;
//  } else {
//    throw std::range_error("Unknown option " + type);
//  }
//  return optionType;
//}


// [[Rcpp::export]]
Rcpp::List doubleKOVannaVolgaEngine(std::string type,
double barrierUp, 
double barrierDown,
double underlying,double strike,
double dividendYield,double riskFreeRate,double maturity,
double vol25Put1, double volAtm1, double vol25Call1,double volatility) {
  
  QuantLib::Option::Type optionType = getOptionType(type);
  QuantLib::Time t=maturity;
  
  QuantLib::Barrier::Type barrierType1= QuantLib::Barrier::DownOut,barrierType2= QuantLib::Barrier::UpOut;
  
  QuantLib::Real barrier1=barrierDown, barrier2=barrierUp,K=strike,rebate=0;
  
  QuantLib::Volatility v=volatility;
  
  int length = int(maturity*360 + 0.5); // FIXME: this could be better, but same rounding in QL
  
  // new QuantLib 0.3.5 framework: digitals, updated for 0.3.7
  // updated again for QuantLib 0.9.0, 
  // cf QuantLib-0.9.0/test-suite/digitaloption.cpp
  QuantLib::Date today = QuantLib::Date::todaysDate();
  QuantLib::Settings::instance().evaluationDate() = today;
  
  QuantLib::DayCounter dc = QuantLib::Actual360();
  boost::shared_ptr<QuantLib::SimpleQuote> spot(new QuantLib::SimpleQuote(underlying));
  boost::shared_ptr<QuantLib::SimpleQuote> qRate(new QuantLib::SimpleQuote(dividendYield));
  boost::shared_ptr<QuantLib::YieldTermStructure> qTS = flatRate(today,qRate,dc);
  boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(riskFreeRate));
  boost::shared_ptr<QuantLib::YieldTermStructure> rTS = flatRate(today,rRate,dc);
  boost::shared_ptr<QuantLib::SimpleQuote> volAtm(new QuantLib::SimpleQuote(volAtm1));
  boost::shared_ptr<QuantLib::SimpleQuote> vol25Put(new QuantLib::SimpleQuote(vol25Put1));
  boost::shared_ptr<QuantLib::SimpleQuote> vol25Call(new QuantLib::SimpleQuote(vol25Call1));
  
  //    boost::shared_ptr<QuantLib::StrikedTypePayoff> payoff;
  //    if (binType=="cash") {
  //        boost::shared_ptr<QuantLib::StrikedTypePayoff> con(new QuantLib::CashOrNothingPayoff(optionType, strike, cashPayoff));
  //        payoff = con;
  //    } else if (binType=="asset") {
  //        boost::shared_ptr<QuantLib::StrikedTypePayoff> aon(new QuantLib::AssetOrNothingPayoff(optionType, strike));
  //        payoff = aon;
  //    } else if (binType=="gap") {
  //        boost::shared_ptr<QuantLib::StrikedTypePayoff> gap(new QuantLib::GapPayoff(optionType, strike, cashPayoff));
  //        payoff = gap;
  //    } else {
  //        throw std::range_error("Unknown binary option type " + binType);
  //    }
  
  boost::shared_ptr< QuantLib::StrikedTypePayoff> payoff =
  boost::make_shared< QuantLib::PlainVanillaPayoff>(optionType,
  K);
  
  QuantLib::Date exDate = today + QuantLib::Integer(maturity * 365 + 0.5);
  boost::shared_ptr<QuantLib::Exercise> exercise = boost::make_shared<QuantLib::EuropeanExercise>(exDate);
  
  
  
  QuantLib::Handle<QuantLib::DeltaVolQuote> volAtmQuote = QuantLib::Handle<QuantLib::DeltaVolQuote>(
    boost::make_shared<QuantLib::DeltaVolQuote>(
      QuantLib::Handle<QuantLib::Quote>(volAtm),
      QuantLib::DeltaVolQuote::Fwd,
      t,
      QuantLib::DeltaVolQuote::AtmDeltaNeutral));
      
      //always delta neutral atm
      QuantLib::Handle<QuantLib::DeltaVolQuote> vol25PutQuote(QuantLib::Handle<QuantLib::DeltaVolQuote>(
        boost::make_shared<QuantLib::DeltaVolQuote>(
          -0.25,
          QuantLib::Handle<QuantLib::Quote>(vol25Put),
          t,
          QuantLib::DeltaVolQuote::Fwd)));
          
          QuantLib::Handle<QuantLib::DeltaVolQuote> vol25CallQuote(QuantLib::Handle<QuantLib::DeltaVolQuote>(
            boost::make_shared<QuantLib::DeltaVolQuote>(
              0.25,
              QuantLib::Handle<QuantLib::Quote>(vol25Call),
              t,
              QuantLib::DeltaVolQuote::Fwd)));
              
              std::vector<QuantLib::Barrier::Type> barrierTypes;
              barrierTypes.push_back(barrierType1);
              barrierTypes.push_back(barrierType2);
              std::vector<QuantLib::Real> barriers;
              barriers.push_back(barrier1);
              barriers.push_back(barrier2);
              std::vector<QuantLib::Real> rebates;
              rebates.push_back(rebate);
              rebates.push_back(rebate);
              QuantLib::DoubleBarrierOption doubleBarrierOption(barrierTypes,
              barriers,
              rebates,
              payoff,
              exercise);
              
              QuantLib::Real bsVanillaPrice =
              QuantLib::blackFormula(optionType, K,
              spot->value()*qTS->discount(t)/rTS->discount(t),
              v * sqrt(t), rTS->discount(t));
              boost::shared_ptr<QuantLib::PricingEngine> vannaVolgaEngine =
              boost::make_shared<QuantLib::VannaVolgaDoubleBarrierEngine>(
                volAtmQuote,
                vol25PutQuote,
                vol25CallQuote,
                QuantLib::Handle<QuantLib::Quote>(spot),
                QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
                QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
                true,
                bsVanillaPrice);
                doubleBarrierOption.setPricingEngine(vannaVolgaEngine);
                
                QuantLib::Real calculated = doubleBarrierOption.NPV();
                
                return Rcpp::List::create(Rcpp::Named("calc") = calculated);
}

// [[Rcpp::export]]
Rcpp::List oneTouchEngine(std::string type, double underlying,double strike, double cashPayoff,double dividendYield,double riskFreeRate,double maturity,double volatility,const std::vector<double> strikes,const std::vector<int> expirationsRcpp, const Rcpp::NumericMatrix volMatrixRcpp) {
  
  
  
  QuantLib::Option::Type optionType = getOptionType(type);
  QuantLib::Time t=maturity;
  
  QuantLib::Real K=strike;
  
  //QuantLib::Volatility v=volatility;
  
  
  QuantLib::Date today =  QuantLib::Date::todaysDate();
  
  
  
  QuantLib::Matrix volMatrix=QuantLib::Matrix((QuantLib::Size)volMatrixRcpp.nrow(),(QuantLib::Size)volMatrixRcpp.ncol());
   for (int i = 0;i<volMatrixRcpp.nrow();i++){
     for(int j =0;j<volMatrixRcpp.ncol();j++){
            double vol = volMatrixRcpp(i,j);
            volMatrix[i][j] = vol;            
        }
  }
  
int n = expirationsRcpp.size();
        std::vector<QuantLib::Date> expirations(expirationsRcpp.size());
        for (int i = 0;i<n;i++){
            QuantLib::Date day=today + expirationsRcpp[i];
            expirations[i] = day;            
        }

  
  
  QuantLib::DayCounter dc = QuantLib::Actual360();
  boost::shared_ptr<QuantLib::SimpleQuote> spot(new QuantLib::SimpleQuote(underlying));
  boost::shared_ptr<QuantLib::SimpleQuote> qRate(new QuantLib::SimpleQuote(dividendYield));
  boost::shared_ptr<QuantLib::YieldTermStructure> qTS = flatRate(today,qRate,dc);
  boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(riskFreeRate));
  boost::shared_ptr<QuantLib::YieldTermStructure> rTS = flatRate(today,rRate,dc);
  boost::shared_ptr< QuantLib::SimpleQuote> vol(new  QuantLib::SimpleQuote(volatility));
  boost::shared_ptr< QuantLib::BlackVolTermStructure> volTS = volSurface(today, expirations, strikes, volMatrix, dc);
  
  QuantLib::Size timeStepsPerYear = 90;
  QuantLib::Size maxSamples = 1000000;
  QuantLib::BigNatural seed = 1;
  
  
  
  boost::shared_ptr<QuantLib::StrikedTypePayoff> payoff(new QuantLib::CashOrNothingPayoff(
    optionType, K, cashPayoff));
    //FLOATING_POINT_EXCEPTION
    QuantLib::Date exDate = today + QuantLib::Integer(t*360+0.5);
    boost::shared_ptr<QuantLib::Exercise> amExercise(
      new QuantLib::AmericanExercise(today, exDate));
      
      
      boost::shared_ptr<QuantLib::BlackScholesMertonProcess> stochProcess(new
      QuantLib::BlackScholesMertonProcess(QuantLib::Handle<QuantLib::Quote>(spot),
      QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
      QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
      QuantLib::Handle<QuantLib::BlackVolTermStructure>(volTS)));
      
      QuantLib::Size requiredSamples = QuantLib::Size(std::pow(2.0, 14) - 1);
//      boost::shared_ptr<QuantLib::PricingEngine> mcldEngine =
//      QuantLib::MakeMCDigitalEngine<QuantLib::LowDiscrepancy>(stochProcess)
//      .withStepsPerYear(timeStepsPerYear)
//      .withBrownianBridge()
//      .withSamples(requiredSamples)
//      .withMaxSamples(maxSamples)
//      .withSeed(seed);
//      
        boost::shared_ptr<PricingEngine> amEngine(new AnalyticDigitalAmericanEngine(stochProcess));
      
      QuantLib::VanillaOption opt(payoff, amExercise);
      opt.setPricingEngine(amEngine);
      
      QuantLib::Real calculated = opt.NPV();
      
      
      return Rcpp::List::create(Rcpp::Named("calc") = calculated);                    
      
}