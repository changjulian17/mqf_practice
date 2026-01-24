#include <cmath>
#include "black.h"

double norm_cdf(double x) {
    return 0.5 * erfc(-x * M_SQRT1_2);
}

double BlackScholes(
	double notional, 
	double strike, 
	double expiry, 
	double spot, 
	double vol, 
	double rate, 
	bool isCall)
{
    double d1 = (log(spot / strike) + (rate + 0.5 * vol * vol) * expiry) / (vol * sqrt(expiry));
    double d2 = d1 - vol * sqrt(expiry);
    double df = exp(-rate * expiry);

    if (isCall)
        return notional * (spot * norm_cdf(d1) - strike * df * norm_cdf(d2));
    else
        return notional * (strike * df * norm_cdf(-d2) - spot * norm_cdf(-d1));
};

