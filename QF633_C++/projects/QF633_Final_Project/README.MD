# QF633_FINAL_PROJECT

## Project Overview

QF633_FINAL_PROJECT is a C++ analytics engine for pricing and risk management of fixed income and equity derivative products. It supports swaps, bonds, and tree-based derivatives, providing both present value (PV) and risk sensitivities (Delta, Vega) using multiple pricing models (tree and Black-Scholes).

---

## Execution Summary

- **Input:**  
  The engine reads market data (curves, prices, volatility) and a list of trades from text files.
- **Processing:**  
  For each trade, it computes:
  - Present Value (PV) using the appropriate pricing model
  - Black-Scholes PV (BlackPV) for eligible products
  - Delta (per risk factor)
  - Vega
- **Output:**  
  Results are written to `output.txt` in a semicolon-delimited format, with a summary line at the end.

**Sample Output:**

```
TradeID;Product;Model;PV;BlackPV;Delta;Vega
1;Swap;Tree;1000000;950000;5000;200
2;Bond;BS;1050000;1025000;5200;210
...
Summary;Total;PV;Delta;Vega
;2050000;102500;410
```

---

## Required Configurations

### Prerequisites

- **Compiler:** C++17 compatible (e.g., `g++`, `clang++`)
- **Operating System:** macOS or Linux recommended

### Input Files

Place these files in the project directory:
- `usd_curve.txt` — USD interest rate curve
- `sgd_curve.txt` — SGD interest rate curve
- `bondPrice.txt` — Bond prices
- `stockPrice.txt` — Stock prices
- `vol.txt` — Volatility data
- `trade.txt` — Trade definitions

### Build Instructions

1. Open a terminal in the project directory.
2. Compile the project:
   ```sh
   g++ -std=c++17 -o Main Main.cpp Bond.cpp Swap.cpp Pricer.cpp RiskEngine.cpp Market.cpp MarketDataUtils.cpp PortfolioUtils.cpp Date.cpp
   ```
3. Run the executable:
  ./Main
4. Check `output.txt` for results

### File Structure
Main.cpp — Main program logic
Bond.cpp, Bond.h — Bond instrument
Swap.cpp, Swap.h — Swap instrument
TreeProduct.h — Tree-based derivatives
Pricer.cpp, Pricer.h — Pricing logic
RiskEngine.cpp, RiskEngine.h — Risk calculations
Market.cpp, Market.h — Market data
MarketDataUtils.cpp, MarketDataUtils.h — Utilities for market data
PortfolioUtils.cpp, PortfolioUtils.h — Portfolio aggregation
Date.cpp, Date.h — Date utilities
Types.h — Common types
output.txt — Output results


## Author
https://github.com/changjulian17