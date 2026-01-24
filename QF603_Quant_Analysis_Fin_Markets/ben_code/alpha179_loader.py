"""
Alpha 179 (Alpha191) Class Loader

This module loads the Alpha191 class directly from the notebook file.
Use this instead of trying to execute the notebook with IPython.

Usage:
    from alpha179_loader import load_alpha191
    Alpha191 = load_alpha191()
    alpha179 = Alpha191(data)
"""

import json
import sys
import warnings
warnings.filterwarnings('ignore')

# Note: pandas_compat is no longer needed - notebook now uses modern pandas syntax


def load_alpha191():
    """
    Load the Alpha191 class from alpha179_factors.ipynb

    Returns:
        Alpha191: The Alpha191 class (not instantiated)

    Raises:
        FileNotFoundError: If alpha179_factors.ipynb doesn't exist
        RuntimeError: If Alpha191 class cannot be loaded
    """
    print("Loading Alpha191 class from alpha179_factors.ipynb...")

    # Load notebook
    try:
        with open('alpha179_factors.ipynb', 'r', encoding='utf-8') as f:
            nb = json.load(f)
    except FileNotFoundError:
        raise FileNotFoundError(
            "alpha179_factors.ipynb not found. "
            "Make sure you're running from the correct directory."
        )

    # Find and execute cells
    namespace = {}
    cells_executed = 0

    for i, cell in enumerate(nb['cells']):
        if cell['cell_type'] != 'code':
            continue

        # Get source code
        if isinstance(cell['source'], list):
            code = ''.join(cell['source'])
        else:
            code = cell['source']

        # Skip empty cells and examples
        if not code.strip():
            continue
        if 'df_open' in code or 'Example:' in code:
            continue

        # Execute cell
        try:
            exec(code, namespace)
            cells_executed += 1
        except Exception as e:
            # Only fail if it's the Alpha191 class definition (cell 5)
            if i == 5:
                raise RuntimeError(f"Failed to load Alpha191 class: {e}")
            # Otherwise just warn
            print(f"  Warning: Cell {i} had errors (skipped)")

    # Check if Alpha191 is available
    if 'Alpha191' not in namespace:
        raise RuntimeError(
            f"Alpha191 class not found after executing {cells_executed} cells. "
            "The notebook may have structural issues."
        )

    print(f"✓ Alpha191 class loaded successfully")
    print(f"  Executed {cells_executed} cells")

    return namespace['Alpha191']


def get_alpha179_instance(data):
    """
    Convenience function to load Alpha191 and create an instance.

    Args:
        data: Dictionary with keys 'open', 'high', 'low', 'close', 'volume', 'vwap'
              Each value should be a DataFrame (dates × symbols)

    Returns:
        alpha179: Instantiated Alpha191 object
    """
    Alpha191 = load_alpha191()

    print("Instantiating Alpha191...")
    alpha179 = Alpha191(data)

    # Count available methods
    methods = [m for m in dir(alpha179) if m.startswith('alpha_')]
    print(f"✓ Alpha179 instance created")
    print(f"  Available methods: {len(methods)}")
    print(f"  Sample: {methods[:5]}")

    return alpha179


if __name__ == '__main__':
    # Test loading
    print("Testing Alpha191 loader...")
    try:
        Alpha191 = load_alpha191()
        print("\n✅ Success! Alpha191 class is available.")
        print(f"Class: {Alpha191}")
        print(f"Methods: {[m for m in dir(Alpha191) if not m.startswith('_')][:10]}")
    except Exception as e:
        print(f"\n❌ Failed: {e}")
        sys.exit(1)
