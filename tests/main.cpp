#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "covidnan/lib.hpp"

TEST_CASE("testing the factorial function")
{
    CHECK(covidnan::factorial(1) == 1);
    CHECK(covidnan::factorial(2) == 2);
    CHECK(covidnan::factorial(3) == 6);
    CHECK(covidnan::factorial(10) == 3628800);

}