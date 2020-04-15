[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://travis-ci.org/bbvch/Coronan.svg?branch=master)](https://travis-ci.org/bbvch/Coronan)
[![codecov](https://codecov.io/gh/bbvch/Coronan/branch/master/graph/badge.svg)](https://codecov.io/gh/bbvch/Coronan)

# Documentation for Co[ro]nan

Co[ro]nan is an example C++17 project using [conan](https://conan.io/) and [CMake](https://cmake.org/).
It reads the current data of Corona (Covid-19) cases for Switzerland from the _The About Corona Covid-19 API_. 

[The About Corona Covid-19](https://about-corona.net/documentation) provides statistics via REST API from The [World Health Organization Situation Reports](https://www.who.int/emergencies/diseases/novel-coronavirus-2019/situation-reports), [Johns Hopkins CSSE](https://coronavirus.jhu.edu/map.html), [The U.S. Department of Health & Human Services](https://www.hhs.gov/), [The National Health Commission of the People’s Republic of China](http://en.nhc.gov.cn/), [The European Centre for Disease Prevention and Control](https://www.ecdc.europa.eu/en), and [China CDC Weekly](http://weekly.chinacdc.cn/news/TrackingtheEpidemic.htm).

![Screenshot of the qt application](docs/images/Screenshot-qt.png)

## Development

To build the project you need python and [conan (version 1.24 or newer)](https://conan.io/):
```bash
#> pip install conan
```
Further a recent version of [CMake](https://cmake.org/) (> 3.10), a modern C++ compiler (C++17) and [Qt](https://www.qt.io/) with [QtCharts](https://doc.qt.io/qt-5/qtcharts-index.html) must be installed.

### How to build

```bash
#> ./build.sh
```

### Source Code formatting
For source code formatting [clang-format](https://clang.llvm.org/docs/ClangFormat.html) for C++ files and [cmake-format](https://pypi.org/project/cmake-format/) for the CMake files are used.
