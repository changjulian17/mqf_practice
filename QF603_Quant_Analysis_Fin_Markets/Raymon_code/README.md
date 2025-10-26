# DU Pipeline v4

## Overview

This notebook implements a full alpha factor research and backtesting pipeline, covering data acquisition, factor construction, cleaning, and performance evaluation.

focus on ML instead of doing NN

reference: https://arxiv.org/abs/2507.07107

## Contents

1. **Part 0 – Data Acquisition**

   * Downloads and preprocesses stock price data.
   * Typical fields: OHLCV, returns, sector info.

2. **Part 1 – Factor Construction**

   * Computes multiple *alpha signals* using technical and statistical features.
   * Designed for multi-factor experiments.

3. **Part 2 – Handling NaNs**

   * Implements median/rolling imputation to ensure stable factor coverage.

4. **Part 3 – IC Evaluation**

   * Computes **Information Coefficients (IC)** and **ICIR** per factor.
   * Includes baseline and timing-window IC analysis.

5. **Part 4 – Portfolio Simulation**

   * Long-short strategy based on top/bottom quantiles of each factor.
   * Ensures dollar neutrality and proper scaling.

## Requirements

* Python ≥ 3.9
* Libraries: `pandas`, `numpy`, `tqdm`, `matplotlib`, `scipy`, `sklearn`

## Usage

```bash
jupyter notebook "Du pipeline_v4.ipynb"
```

Run all cells sequentially. Adjust parameters (e.g., window lengths, lookahead horizon) in the setup section.

## Output

* Cleaned factor tables
* IC/IR statistics
* Cumulative P&L and rank correlation plots


