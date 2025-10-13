# Step 4: Factor Loadings Generation - Modular Workflow

## Overview

Step 4 has been split into **3 modular notebooks** to prevent data loss when errors occur. Each notebook saves its results independently.

## Why Split?

**Problem**: The original notebook calculated both Alpha 101 and Alpha 179 sequentially. If Alpha 179 failed, you had to re-calculate Alpha 101 again.

**Solution**: Split into 3 notebooks that save intermediate results:
- If Alpha 179 fails, Alpha 101 results are already saved
- You can re-run only the failed part
- Faster debugging and iteration

---

## Workflow

### Step 4a: Alpha 101 Calculation
**File**: `factor_model_step4a_alpha101.ipynb`

**What it does**:
1. Loads winsorized data from Step 3
2. Converts to panel format (date × symbol)
3. Uses existing `avg_price` as VWAP proxy (20-day rolling avg from Step 2)
4. Loads Alpha 101 functions
5. Calculates all 101 alpha factors
6. **Saves results to disk**

**Note on VWAP**: Uses the actual `avg_price` column from your data (20-day rolling average of close) instead of approximating VWAP from daily OHLC bars. True VWAP requires intraday tick data which isn't available in daily data.

**Outputs**:
- ✅ `alpha101_results.parquet` - Successfully calculated Alpha 101 factors
- ✅ `alpha101_errors.csv` - Error log for failed factors
- ✅ `data_panels.parquet` - Data panels for Step 4b (avoids reprocessing)

**Run time**: ~5-10 minutes

---

### Step 4b: Alpha 179 Calculation
**File**: `factor_model_step4b_alpha179.ipynb`

**What it does**:
1. Loads data panels from Step 4a (fast!)
2. **Restarts kernel recommended** - loads Alpha 179 class fresh
3. Calculates all 179 alpha factors
4. **Saves results to disk**

**Inputs**:
- `data_panels.parquet` (from Step 4a)

**Outputs**:
- ✅ `alpha179_results.parquet` - Successfully calculated Alpha 179 factors
- ✅ `alpha179_errors.csv` - Error log for failed factors

**Run time**: ~10-20 minutes

**Important**: If this fails, you DON'T need to re-run Step 4a! Alpha 101 results are already saved.

---

### Step 4c: Combine and Standardize
**File**: `factor_model_step4c_combine_standardize.ipynb`

**What it does**:
1. Loads Alpha 101 results from Step 4a
2. Loads Alpha 179 results from Step 4b
3. Combines all factors
4. Applies cross-sectional z-score standardization
5. Handles missing values (fills with 0)
6. Analyzes factor correlations
7. **Saves final factor loadings**

**Inputs**:
- `alpha101_results.parquet` (from Step 4a)
- `alpha179_results.parquet` (from Step 4b)

**Outputs**:
- ✅ `russell2000_factor_loadings_step4.parquet` - **Final standardized factor loadings** (for Step 5)
- ✅ `factor_loadings_info.csv` - Factor metadata
- ✅ `factor_calculation_errors.csv` - Combined error log

**Run time**: ~2-5 minutes

---

## Execution Order

```
┌─────────────────────────────────────────────────────────────┐
│  Step 4a: Alpha 101                                         │
│  ───────────────────────                                    │
│  Input:  russell2000_winsorized_step3.parquet              │
│  Output: alpha101_results.parquet                           │
│          data_panels.parquet                                │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│  Step 4b: Alpha 179                                         │
│  ───────────────────────                                    │
│  Input:  data_panels.parquet                                │
│  Output: alpha179_results.parquet                           │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│  Step 4c: Combine & Standardize                             │
│  ────────────────────────────────                           │
│  Input:  alpha101_results.parquet                           │
│          alpha179_results.parquet                           │
│  Output: russell2000_factor_loadings_step4.parquet          │
└─────────────────────────────────────────────────────────────┘
```

---

## How to Run

### Option 1: Run All (Sequential)
```bash
# Run Step 4a
jupyter notebook factor_model_step4a_alpha101.ipynb

# Run Step 4b (restart kernel first!)
jupyter notebook factor_model_step4b_alpha179.ipynb

# Run Step 4c
jupyter notebook factor_model_step4c_combine_standardize.ipynb
```

### Option 2: Recovery from Errors

**If Step 4b (Alpha 179) fails**:
1. ✅ Step 4a results are already saved - don't re-run!
2. ❌ Fix the Alpha 179 errors (check `alpha179_errors.csv`)
3. 🔄 Re-run only Step 4b
4. ▶️ Then run Step 4c

**If Step 4c fails**:
1. ✅ Both Step 4a and 4b results are saved
2. 🔄 Re-run only Step 4c

---

## Key Benefits

### 1. **No Data Loss**
- Each stage saves its results
- Errors in later stages don't affect earlier work

### 2. **Faster Debugging**
- Only re-run the failed part
- Alpha 101 takes 5-10 min - don't recalculate unnecessarily!

### 3. **Independent Testing**
- Test Alpha 101 and Alpha 179 separately
- Easier to identify which factors have issues

### 4. **Flexible Workflow**
- Want to skip Alpha 179? Just run 4a → 4c
- Want to recalculate only Alpha 179? Just run 4b → 4c

---

## File Sizes (Approximate)

| File | Size | Description |
|------|------|-------------|
| `alpha101_results.parquet` | ~20-50 MB | Alpha 101 factor loadings |
| `alpha179_results.parquet` | ~30-80 MB | Alpha 179 factor loadings |
| `data_panels.parquet` | ~10-30 MB | Preprocessed data panels |
| `russell2000_factor_loadings_step4.parquet` | ~50-150 MB | **Final output** |

---

## Troubleshooting

### Error: "Alpha191 class not found" in Step 4b
**Solution**: Restart your Jupyter kernel before running Step 4b
- Kernel → Restart Kernel
- This clears cached versions of alpha179_factors.ipynb

### Error: "alpha101_results.parquet not found" in Step 4c
**Solution**: Run Step 4a first

### Error: "alpha179_results.parquet not found" in Step 4c
**Options**:
1. Run Step 4b first (recommended)
2. Skip Alpha 179 - Step 4c will continue with Alpha 101 only

### Error: High memory usage
**Solution**:
- Close other notebooks
- Restart kernel between steps
- Consider running on a machine with more RAM

---

## Original Notebook

The original monolithic notebook is still available:
- `factor_model_step4_loadings_generation.ipynb`

**When to use it**:
- When you want everything in one place
- When you have plenty of time and RAM
- When you're confident there won't be errors

**When to use the split version** (recommended):
- When debugging factor calculations
- When memory is limited
- When you want faster iteration
- When you want to avoid re-calculating everything

---

## Next Steps

After completing Step 4 (all 3 notebooks):

✅ **Output**: `russell2000_factor_loadings_step4.parquet`

➡️ **Next**: Step 5 - Cross-Sectional Regression
- Use the factor loadings to estimate factor returns
- Run: `factor_model_step5_cross_sectional_regression.ipynb`

---

## Summary

| Step | Notebook | Input | Output | Can Skip? |
|------|----------|-------|--------|-----------|
| 4a | alpha101 | Step 3 data | alpha101_results.parquet | ❌ No |
| 4b | alpha179 | data_panels.parquet | alpha179_results.parquet | ⚠️ Optional* |
| 4c | combine | Both results | Final loadings | ❌ No |

*If you skip 4b, Step 4c will use Alpha 101 factors only (fewer factors for regression)

---

**Created**: 2025-10-12
**Purpose**: Modular workflow to prevent data loss during factor calculation
**Author**: Generated by Claude Code
