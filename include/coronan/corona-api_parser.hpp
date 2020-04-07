#pragma once

#include <string>
#include <vector>

#include <optional>

namespace coronan {

/**
 * CountryObject hold the covid-19 data of a single country
 */
struct CountryObject
{
  std::string country_name{}; /**< Country name */
  std::string country_code{}; /**< Country Code , e.g. ch */
  int32_t population{};       /**< Country population */

  struct today_t
  {
    std::string date{};  /**< iso date string */
    int32_t deaths{};    /**< todays death cased */
    int32_t confirmed{}; /**< todays confirmed cases */
  };

  today_t today{}; /**< Todays data when retrieved. New cases of lates date  */

  struct latest_t
  {
    std::string date{};     /**< iso date string (last updated) */
    int32_t deaths{};       /**< latest number of deaths */
    int32_t confirmed{};    /**< latest number of confirmed cases */
    int32_t recovered{};    /**< latest number of recovered cases */
    int32_t critical{};     /**< latest number of critical cases */
    std::optional<double> death_rate{};    /**< calculated death rate */
    std::optional<double> recovery_rate{}; /**< calculated recovery rate */
    std::optional<double> recovered_vs_death_ratio{};   /**< calculated recovered vs death rate */
    std::optional<double> cases_per_million_population{}; /**< calculated cases per millions of
                                              the population */
  };

  latest_t latest{}; /**< Lates actual cases  */

  struct timeline_t
  {
    std::string date{};      /**< iso date string  */
    int32_t deaths{};        /**< number of deaths */
    int32_t confirmed{};     /**< number of confirmed cases */
    int32_t active{};        /**< number of current covid-19 cases */
    int32_t recovered{};     /**< number of recovered cases */
    int32_t new_deaths{};    /**< new death since last time data */
    int32_t new_confirmed{}; /**< new confirmed cases since last time data */
    int32_t new_recovered{}; /**< new recovered cases since last time data */
  };

  std::vector<timeline_t> timeline; /**< array of (daily) data */
};

struct OverviewObject
{
    struct country_t
    {
        std::string name{}; /**< Country name */
        std::string code{}; /**< Country Code , e.g. ch */
    };

    std::vector<country_t> countries; /**< array of available countries */
};


namespace api_parser {
/**
 * Parse a json.
 * @note Must have the format as described at
 * https://about-corona.net/documentation
 */
CountryObject parse(std::string const& json);

OverviewObject parse_countries(std::string const& json);
} // namespace api_parser

} // namespace coronan