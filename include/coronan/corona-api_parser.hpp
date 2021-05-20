#pragma once

#include <optional>
#include <string>
#include <vector>

namespace coronan {

struct CountryInfo
{
  std::string name{};                   /**< Country name */
  std::string iso_code{};               /**< ISO 3166-1 alpha-2 Country Code , e.g. ch */
  std::optional<uint32_t> population{}; /**< Country population */
};

/**
 * CountryData hold the covid-19 data of a single country
 */
struct CountryData
{

  CountryInfo info{}; /**< country information (name, code, population) */
  struct TodayData
  {
    std::string date{};                  /**< iso date string */
    std::optional<uint32_t> deaths{};    /**< todays death cased */
    std::optional<uint32_t> confirmed{}; /**< todays confirmed cases */
  };

  TodayData today{}; /**< TodayDatas data when retrieved. New cases of lates date  */

  struct LatestData
  {
    std::string date{};                                     /**< iso date string (last updated) */
    std::optional<uint32_t> deaths{};                       /**< latest number of deaths */
    std::optional<uint32_t> confirmed{};                    /**< latest number of confirmed cases */
    std::optional<uint32_t> recovered{};                    /**< latest number of recovered cases */
    std::optional<uint32_t> critical{};                     /**< latest number of critical cases */
    std::optional<double> death_rate{};                     /**< calculated death rate */
    std::optional<double> recovery_rate{};                  /**< calculated recovery rate */
    std::optional<double> recovered_vs_death_ratio{};       /**< calculated recovered vs death rate */
    std::optional<uint32_t> cases_per_million_population{}; /**< calculated cases
                                           per millions of the population */
  };

  LatestData latest{}; /**< Lates actual cases  */

  struct TimelineData
  {
    std::string date{};                      /**< iso date string  */
    std::optional<uint32_t> deaths{};        /**< number of deaths */
    std::optional<uint32_t> confirmed{};     /**< number of confirmed cases */
    std::optional<uint32_t> active{};        /**< number of current covid-19 cases */
    std::optional<uint32_t> recovered{};     /**< number of recovered cases */
    std::optional<uint32_t> new_deaths{};    /**< new death since last time data */
    std::optional<uint32_t> new_confirmed{}; /**< new confirmed cases since last time data */
    std::optional<uint32_t> new_recovered{}; /**< new recovered cases since last time data */
  };

  std::vector<TimelineData> timeline{}; /**< array of (daily) data */
};

using CountryListObject = std::vector<CountryInfo>;

namespace api_parser {
/**
 * Parse a json string for country data.
 * @param json json string. Must have the format as described at
 * https://about-corona.net/documentation
 * @return Parsed Covid-19 case data
 */
CountryData parse_country(std::string const& json);

/**
 * Parse a json string for a list of country information
 * @param json json string. Must have the format as described at
 * https://about-corona.net/documentation
 * @return Country list parsed
 */
CountryListObject parse_countries(std::string const& json);
} // namespace api_parser

} // namespace coronan
