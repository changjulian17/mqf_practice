#include <iostream>
#include<fstream>
#include <vector>

#include "black.h"

using namespace std;

struct OptionTrade
{
    double notional;
    double strike;
    double expiry;
    bool isCall;
};

void loadTradeFromFile(vector<OptionTrade>&tradesSet, const string& filePath) 
{
    /*
    load trade data from file
    insert into vector
    */
}

int main () {
    cout << "compute option pv task is started." << endl;
    vector<OptionTrade> tradesSet;
    string file = "";
    loadTradeFromFile(tradesSet, file);
    vector<double> pvResult;
    double spot = 100;
    double vol = .2;
    double rate = 0.045;

    for (auto& trade : tradesSet)
    {
        /*
        call black sholes model here
        * 
        */
        double n = trade.notional;        
        double pv = BlackScholes(trade.notional, trade.strike , trade.expiry, spot, vol, rate, trade.isCall);
        pvResult.push_back(pv);

    }

    /*
    save result back into a file
    */

    cout << "compute option pv task is completed." << endl;
    return 0;

}