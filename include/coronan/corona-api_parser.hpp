#pragma once

#include <string>
#include <vector>

namespace coronan {

struct CountryObject
{
  std::string country_name{};
  std::string country_code{};
  int32_t population{};

  struct today_t
  {
    std::string date{};
    int32_t deaths{};
    int32_t confirmed{};
  };

  today_t today{};

  struct latest_t
  {
    std::string date{};
    int32_t deaths{};
    int32_t confirmed{};
    int32_t recovered{};
    int32_t critical{};
    double death_rate{};
    double recovery_rate{};
    double cases_per_million_population{};
  };

  latest_t latest{};

  struct timeline_t
  {
    std::string date{};
    int32_t deaths{};
    int32_t confirmed{};
    int32_t active{};
    int32_t recovered{};
    int32_t new_deaths{};
    int32_t new_confirmed{};
    int32_t new_recovered{};
  };

  std::vector<timeline_t> timeline;
};

namespace api_parser {
CountryObject parse(std::string const& json);
}

} // namespace coronan