[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![version](https://img.shields.io/badge/version-v0.2.0-blue)
![GitHub last commit](https://img.shields.io/github/last-commit/bbvch/Coronan)

[![Build Status Travis](https://travis-ci.org/bbvch/Coronan.svg?branch=master)](https://travis-ci.org/bbvch/Coronan)
[![Build status Appveyor](https://ci.appveyor.com/api/projects/status/hd77e7u1s4kboogy/branch/master?svg=true)](https://ci.appveyor.com/project/meshell/coronan/branch/master)
[![CI](https://github.com/bbvch/Coronan/actions/workflows/ci.yml/badge.svg)](https://github.com/bbvch/Coronan/actions/workflows/ci.yml)

[![Codacy Security Scan](https://github.com/bbvch/Coronan/actions/workflows/codacy-analysis.yml/badge.svg)](https://github.com/bbvch/Coronan/actions/workflows/codacy-analysis.yml)
[![CodeQL](https://github.com/bbvch/Coronan/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/bbvch/Coronan/actions/workflows/codeql-analysis.yml)
[![codecov](https://codecov.io/gh/bbvch/Coronan/branch/master/graph/badge.svg)](https://codecov.io/gh/bbvch/Coronan)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/96483b3b9dff405195f7ef0c9c823484)](https://www.codacy.com/gh/bbvch/Coronan/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=bbvch/Coronan&amp;utm_campaign=Badge_Grade)
[![Codacy Badge](https://app.codacy.com/project/badge/Coverage/96483b3b9dff405195f7ef0c9c823484)](https://www.codacy.com/gh/bbvch/Coronan/dashboard?utm_source=github.com&utm_medium=referral&utm_content=bbvch/Coronan&utm_campaign=Badge_Coverage)
[![Coverity Scan](https://img.shields.io/coverity/scan/22882.svg)](https://scan.coverity.com/projects/bbvch-coronan)

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/bbvch/Coronan)

# Documentation for Co\[ro\]nan

Co\[ro\]nan is an example C++17 Qt project using [conan](https://conan.io/) and [CMake](https://cmake.org/).
It reads the current data of Corona (Covid-19) cases for a country from the [The About Corona Covid-19 API](https://about-corona.net/documentation).

_The About Corona Covid-19_ provides statistics via REST API from The [World Health Organization Situation Reports](https://www.who.int/emergencies/diseases/novel-coronavirus-2019/situation-reports), [Johns Hopkins CSSE](https://coronavirus.jhu.edu/map.html), [The U.S. Department of Health & Human Services](https://www.hhs.gov/), [The National Health Commission of the People’s Republic of China](http://en.nhc.gov.cn/), [The European Centre for Disease Prevention and Control](https://www.ecdc.europa.eu/en), and [China CDC Weekly](http://weekly.chinacdc.cn/news/TrackingtheEpidemic.htm).

![Screenshot of the qt application](docs/images/Screenshot-qt.png)

## Development

To build the project you need python and a recent [conan version](https://conan.io/):

```bash
#> pip install conan
```

Further a recent version of [CMake](https://cmake.org/) (> 3.15), a modern C++ compiler (C++17) and [Qt](https://www.qt.io/) with [QtCharts](https://doc.qt.io/qt-5/qtcharts-index.html) must be installed.

### How to build

You have the following options to build the project

__Use build script__:

In a linux shell you can build the project with the provided `build.sh` script. E.g. to build in folder `build` execute:

```bash
#> ./build.sh build
```

See `./build.sh -h` for more options

__CMake__:

You can also build it yourself using CMake:

* cmake < 3.20

  ```bash
  #> mkdir build
  #> (cd build && cmake ..)
  #> cmake --build build
  ```

* cmake >= 3.20

  ```bash
  #> cmake -S . --preset=linux-ninja
  #> cmake --build -S . --preset=linux-build
  ```

  or any other [CMakePresets](CMakePresets.json).

### Source Code formatting

For source code formatting [clang-format](https://clang.llvm.org/docs/ClangFormat.html) for C++ files and [cmake-format](https://pypi.org/project/cmake-format/) for the CMake files are used. Run `format_source_files.sh` to format all C++ and CMake files.

_Note: Delete the build directory before formatting, otherwise all CMake files in the build folder will be formatted as well, what may take a while._

### CMake options

* `ENABLE_TESTING`: Build (and run) unittests. _Default_: `ON`
* `ENABLE_BUILD_WITH_TIME_TRACE`: Enable [Clang Time Trace Feature](https://www.snsystems.com/technology/tech-blog/clang-time-trace-feature). _Default: `OFF`_
* `ENABLE_PCH`: Enable [Precompiled Headers](hhttps://en.wikipedia.org/wiki/Precompiled_header). _Default: `OFF`_
* `ENABLE_CACHE`: Enable caching if available, e.g. [ccache](https://ccache.dev/) or [sccache](https://github.com/mozilla/sccache). _Default: `ON`_
* `ENABLE_COVERAGE`: Enable coverage reporting for gcc/clang. _Default: `OFF`_
* `ENABLE_DOXYGEN`: Enable doxygen documentation build ([doxygen](https://www.doxygen.nl/index.html) with [graphviz dot](https://graphviz.org/) must be installed). _Default: `ON`_
* `ENABLE_SANITIZER_ADDRESS`: Enable [address sanitizer](https://clang.llvm.org/docs/AddressSanitizer.html). _Default: `OFF`_
* `ENABLE_SANITIZER_LEAK`: Enable [leak sanitizer](https://clang.llvm.org/docs/LeakSanitizer.html). _Default: `OFF`_
* `ENABLE_SANITIZER_UNDEFINED_BEHAVIOR`: Enable [undefined behavior sanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html). _Default: `OFF`_
* `ENABLE_SANITIZER_THREAD`: Enable [thread sanitizer](https://clang.llvm.org/docs/ThreadSanitizer.html). _Default: `OFF`_
* `ENABLE_SANITIZER_MEMORY`: Enable [memory sanitizer](https://clang.llvm.org/docs/MemorySanitizer.html). _Default: `OFF`_
* `ENABLE_IPO`: Enable Interprocedural Optimization, aka [Link Time Optimization (LTO)](https://llvm.org/docs/LinkTimeOptimization.html). _Default: `OFF`_
* `ENABLE_CPPCHECK`: Enable static analysis with [cppcheck](http://cppcheck.sourceforge.net/). _Default: `ON` if cppcheck is installed `OFF` otherwise._
* `ENABLE_CLANG_TIDY`: Enable static analysis with [clang-tidy](https://clang.llvm.org/extra/clang-tidy/). _Default: `ON` if clang-tidy is installed `OFF` otherwise._
* `ENABLE_INCLUDE_WHAT_YOU_USE`: Enable static analysis with [include-what-you-use](https://include-what-you-use.org/). _Default: `ON` if include-what-you-use is installed `OFF` otherwise_.

## Pre-Commit Hooks

The following [Pre-commit-hooks](https://github.com/pre-commit/pre-commit-hooks) are used to check and format files before commiting:

* `check-added-large-files`: Prevent giant files (> 500kB) from being committed.
* `fix-byte-order-marker`: Forbid files which have a UTF-8 byte-order marker
* `check-case-conflict`: Check for files with names that would conflict on a case-insensitive filesystem like MacOS HFS+ or Windows FAT.
* `check-merge-conflict`: Check for files that contain merge conflict strings.
* `check-symlinks`: Checks for symlinks which do not point to anything.
* `check-yaml`: Attempts to load all yaml files to verify syntax.
* `check-json`: Attempts to load all json files to verify syntax.
* `check-toml`: Attempts to load all TOML files to verify syntax.
* `check-executables-have-shebangs`: Checks that non-binary executables have a proper shebang.
* `end-of-file-fixer`: Makes sure files end in a newline and only a newline.
* `mixed-line-ending`: Replaces or checks mixed line ending.
* `trailing-whitespace`: Trims trailing whitespace in markdown
* `clang-format`: Format C++ files using clang-format
* `cmake-format`: Format CMake files using cmake-format
* `yamllint`: Linter for yaml files
* `yamlfmt`: Format yaml files

Please install pre-commit hooks before commiting any code.

## Gitpod

To start right away without installing anything on you local machine, you can simply use the <img src="https://gitpod.io/button/open-in-gitpod.svg" height="15"> Button at the top of this README.

## direnv & Nix

If you have [direnv](https://direnv.net/) and [nix](https://nixos.org/) installed everything should be setup to start upon entering the project folder. gcc (gcc10) is set as default C and C++ compiler. If you prefare clang (clang_11) change it in [.envrc](.envrc)

When using nix. The most resent cmake is available and therefore configure and build [CMakePresets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) can be used.
