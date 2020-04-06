# Documentation for Co[ro]nan

Co[ro]nan is an example C++17 project using [conan](https://conan.io/) and [CMake](https://cmake.org/).
It reads the current data of Corona (Covid-19) cases for Switzerland from the [The About Corona Covid-19 API](https://about-corona.net/documentation). The About Corona Covid-19 provides statistics via REST API from The World Health Organization Situation Reports, Johns Hopkins CSSE, The U.S. Department of Health & Human Services, The National Health Commission of the People’s Republic of China, The European Centre for Disease Prevention and Control, and China CDC Weekly.

## Development

### How to build

```bash
#> mkdir build
#> (cd build && cmake ..)
#> cmake --build build
```

of simply call the `build.sh` script

```bash
./build.sh
```