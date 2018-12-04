
#include <ql/quantlib.hpp>

using namespace QuantLib;

int main(int argc, char *argv[]) {

  Date tradeDate(20,September,2004);
  Date settleDate(22,September,2004);
  double dt = 0.25;

  Integer fixingDays = 3;
  Calendar calendar = TARGET();
  DayCounter zcBondsDayCounter = Actual365Fixed();
  
  Settings::instance().evaluationDate() = tradeDate;

  std::string interpWhat = "discount";
  std::string interpHow = "loglinear";
  bool flatquotes = false;

  DayCounter termStructureDayCounter = ActualActual(ActualActual::ISDA);
  DayCounter depositDayCounter = Actual360();
  std::vector<boost::shared_ptr<QuantLib::RateHelper> > curveInput;

  double fixDayCount = 6; //"Thirty360";
  double fixFreq = 1;  //"Annual";
  int floatFreq = 6;   //"SemiAnnual"

  // Rate d1wQuote = 0.0382;
  // Rate d1mQuote = 0.0372;
  // Rate fut1Quote = 96.2875;
  // Rate fut2Quote = 96.7875;
  // Rate fut3Quote = 96.9875;
  // Rate fut4Quote = 96.6875;
  // Rate fut5Quote = 96.4875;
  // Rate fut6Quote = 96.3875;
  // Rate fut7Quote = 96.2875;
  // Rate fut8Quote = 96.0875;
  // Rate s3yQuote = 0.0398;
  // Rate s5yQuote = 0.0443;
  // Rate s10yQuote = 0.05165;
  // Rate s15yQuote = 0.055175;
  std::vector<std::string> names{ "d1w", "d1m", "fut1", "fut2", "fut3", "fut4",
      "fut6", "fut7", "fut8", "s3y", "s5y", "s10y", "s15y" };
  std::vector<double> val{0.0382, 0.0372, 96.2875, 96.7875, 96.9875, 96.6875, 96.4875,
      96.3875, 96.2875, 96.0875, 0.0398, 0.0443, 0.05165, 0.055175 };
 
  ext::shared_ptr<Quote> d1wQ(new SimpleQuote(0.0382));
  ext::shared_ptr<RateHelper> d1wR(new DepositRateHelper(Handle<Quote>(d1wQ), 1*Weeks, fixingDays, calendar, ModifiedFollowing, true, zcBondsDayCounter));

  ext::shared_ptr<Quote> d1mQ(new SimpleQuote(0.0372));
  ext::shared_ptr<RateHelper> d1mR(new DepositRateHelper(Handle<Quote>(d1mQ), 1*Months, fixingDays, calendar, ModifiedFollowing, true, zcBondsDayCounter));

  Integer futMonths = 3;
  Date imm = IMM::nextDate(settleDate);
  ext::shared_ptr<Quote> fut1P(new SimpleQuote(96.2875));
  ext::shared_ptr<RateHelper> fut1R(new FuturesRateHelper(Handle<Quote>(fut1P), imm, futMonths, calendar, ModifiedFollowing, true, depositDayCounter));
  
  imm = IMM::nextDate(imm+1);
  ext::shared_ptr<Quote> fut2P(new SimpleQuote(96.7875));
  ext::shared_ptr<RateHelper> fut2R(new FuturesRateHelper(Handle<Quote>(fut2P), imm, futMonths, calendar, ModifiedFollowing, true, depositDayCounter));

  imm = IMM::nextDate(imm+1);
  ext::shared_ptr<Quote> fut3P(new SimpleQuote(96.9875));
  ext::shared_ptr<RateHelper> fut3R(new FuturesRateHelper(Handle<Quote>(fut3P), imm, futMonths, calendar, ModifiedFollowing, true, depositDayCounter));

  imm = IMM::nextDate(imm+1);
  ext::shared_ptr<Quote> fut4P(new SimpleQuote(96.6875));
  ext::shared_ptr<RateHelper> fut4R(new FuturesRateHelper(Handle<Quote>(fut4P), imm, futMonths, calendar, ModifiedFollowing, true, depositDayCounter));

  imm = IMM::nextDate(imm+1);
  ext::shared_ptr<Quote> fut5P(new SimpleQuote(96.4875));
  ext::shared_ptr<RateHelper> fut5R(new FuturesRateHelper(Handle<Quote>(fut5P), imm, futMonths, calendar, ModifiedFollowing, true, depositDayCounter));

  imm = IMM::nextDate(imm+1);
  ext::shared_ptr<Quote> fut6P(new SimpleQuote(96.3875));
  ext::shared_ptr<RateHelper> fut6R(new FuturesRateHelper(Handle<Quote>(fut6P), imm, futMonths, calendar, ModifiedFollowing, true, depositDayCounter));

  imm = IMM::nextDate(imm+1);
  ext::shared_ptr<Quote> fut7P(new SimpleQuote(96.2875));
  ext::shared_ptr<RateHelper> fut7R(new FuturesRateHelper(Handle<Quote>(fut7P), imm, futMonths, calendar, ModifiedFollowing, true, depositDayCounter));

  imm = IMM::nextDate(imm+1);
  ext::shared_ptr<Quote> fut8P(new SimpleQuote(96.0875));
  ext::shared_ptr<RateHelper> fut8R(new FuturesRateHelper(Handle<Quote>(fut8P), imm, futMonths, calendar, ModifiedFollowing, true, depositDayCounter));

  // Rate s3yQuote = 0.0398;
  // Rate s5yQuote = 0.0443;
  // Rate s10yQuote = 0.05165;
  // Rate s15yQuote = 0.055175;
  Frequency swFixedLegFrequency = Annual;
  BusinessDayConvention swFixedLegConvention = Unadjusted;
  DayCounter swFixedLegDayCounter = Thirty360(Thirty360::European);
  ext::shared_ptr<IborIndex> swFloatingLegIndex(new Euribor6M);

  ext::shared_ptr<Quote> s3yQ(new SimpleQuote(0.0398));
  ext::shared_ptr<RateHelper> s3yR(new SwapRateHelper(Handle<Quote>(s3yQ), 3*Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

  ext::shared_ptr<Quote> s5yQ(new SimpleQuote(0.0443));
  ext::shared_ptr<RateHelper> s5yR(new SwapRateHelper(Handle<Quote>(s5yQ), 5*Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

  ext::shared_ptr<Quote> s10yQ(new SimpleQuote(0.05165));
  ext::shared_ptr<RateHelper> s10yR(new SwapRateHelper(Handle<Quote>(s10yQ), 10*Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));

  ext::shared_ptr<Quote> s15yQ(new SimpleQuote(0.055175));
  ext::shared_ptr<RateHelper> s15yR(new SwapRateHelper(Handle<Quote>(s15yQ), 15*Years, calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, swFloatingLegIndex));
  
  std::vector<ext::shared_ptr<RateHelper>> rates;
  rates.push_back(d1wR);
  rates.push_back(d1mR);
  rates.push_back(fut1R);
  rates.push_back(fut2R);
  rates.push_back(fut3R);
  rates.push_back(fut4R);
  rates.push_back(fut5R);
  rates.push_back(fut6R);
  rates.push_back(fut7R);
  rates.push_back(fut8R);
  rates.push_back(s3yR);
  rates.push_back(s5yR);
  rates.push_back(s10yR);
  rates.push_back(s15yR);

  double tolerance = 1.0e-8;

  // discount + logLinear case
  ext::shared_ptr<YieldTermStructure> curve(new PiecewiseYieldCurve<Discount, LogLinear>(settleDate, rates, termStructureDayCounter, std::vector<Handle<Quote> >(), std::vector<Date>(), tolerance));


  double faceAmount = 100;
  Date maturityDate = Date(30, November, 2008);
  Date issueDate = Date(30, November, 2004);
  double redemption = 100;
  
  double settlementDays = 1;
  std::string cal = "UnitedStates/GovernmentBond";
  BusinessDayConvention bdc = Unadjusted;

  ZeroCouponBond bond(settlementDays, calendar, faceAmount, maturityDate, bdc, redemption, issueDate);
      
  //ext::shared_ptr<PricingEngine> bondEngine(new DiscountingBondEngine(Handle<YieldTermStructure>(curve)));
  RelinkableHandle<YieldTermStructure> discountingTermStructure;
  discountingTermStructure.linkTo(curve);
  ext::shared_ptr<DiscountingBondEngine> dbe(new DiscountingBondEngine(discountingTermStructure));
  ext::shared_ptr<PricingEngine> bondEngine(dbe);
  bond.setPricingEngine(bondEngine);

  std::cout << "NPV            " << bond.NPV() 					<< std::endl
            << "clean Price    " << bond.cleanPrice() 				<< std::endl
            << "dirty Price    " << bond.dirtyPrice() 				<< std::endl
            << "accrued Coupon " << bond.accruedAmount() 			<< std::endl
            << "yield          " << bond.yield(Actual360(), Compounded, Annual) << std::endl;
  exit(0);
}
