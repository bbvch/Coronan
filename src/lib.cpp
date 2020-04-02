#include "covidnan/lib.hpp"

namespace covidnan {


int factorial(int number) 
{ 
    return number <= 1 ? number : factorial(number-1) * number; 
}

} // namespace covidnan