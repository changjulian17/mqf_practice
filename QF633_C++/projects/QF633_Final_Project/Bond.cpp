#include "Bond.h"
#include "Market.h"

void Bond::generateSchedule()
{
  //implement this
	if (startDate == maturityDate || frequency <= 0 || frequency > 1)
		throw std::runtime_error("Error: start date is later than end date, or invalid frequency!");

	string tenorStr;
	if (frequency == 0.25)
		tenorStr = "3M";
	else if (frequency == 0.5)
		tenorStr = "6M";
	else
		tenorStr = "1Y";

	Date seed = startDate;
	while (seed < maturityDate) {
		bondSchedule.push_back(seed);
		seed = dateAddTenor(seed, tenorStr);
	}
	bondSchedule.push_back(maturityDate);
	if (bondSchedule.size() < 2)
		throw std::runtime_error("Error: invalid schedule, check input!");

}
double Bond::Payoff(double s) const
{ 		// implement direction
	double pv = notional * (s - tradePrice);
	return pv;
}
double Bond::Pv(const Market& mkt) const {
    Date valueDate = mkt.asOf;
    auto rc = mkt.getCurve(rateCurve);
    double pv = 0.0;

    for (size_t i = 1; i < bondSchedule.size(); ++i) {
        const auto& dt = bondSchedule[i];
        if (dt < valueDate) continue;

        double tau = (bondSchedule[i] - bondSchedule[i - 1]) / 360.0;
        double df = rc->getDf(dt);
        pv += notional * tau * coupon * df;

        if (i == bondSchedule.size() - 1)
            pv += notional * df;  // principal
    }

    if (direction == DirectionType::Long) return pv;
    if (direction == DirectionType::Short) return -pv;

    throw std::runtime_error("Unsupported direction type for Bond");
}



