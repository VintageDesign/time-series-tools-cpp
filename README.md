#Time Series Tools

A collection of time series analysis tools written in C++ with bindings to python.

## Current Tools:
- [Mann Kendall Test for Monotonic
  Trends](https://vsp.pnnl.gov/help/vsample/design_trend_mann_kendall.htm)

## How to Build:
Building the Time Series Tools lib requires cmake
```
# Configure Project:
cmake -B build .

# Build c++ lib and tests
cmake --build build
```

### Building Python Bindings:
```
# Configure Project:
cmake -B build . -DFLAG_BUILD_BINDINGS=1

# Build c++ lib and tests
cmake --build build
```

The bindings can then be used by running the following:
```python
>>> import time_series_tools
```

