#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace coronan {

/**
 * Holds general country information
 */
struct CountryInfo
{
  std::string name{};                   /**< Country name */
  std::string iso_code{};               /**< ISO 3166-1 alpha-2 Country Code , e.g. ch */
  std::optional<uint32_t> population{}; /**< Country population */
};

/**
 * Holds the covid-19 case data of a single country
 */
struct CountryData
{

  CountryInfo info{}; /**< country information (name, code, population) */
  /**
   * Holds the available covid-19 case data of the country for today
   */
  struct TodayData
  {
    std::string date{};                  /**< iso date string */
    std::optional<uint32_t> deaths{};    /**< todays death cased */
    std::optional<uint32_t> confirmed{}; /**< todays confirmed cases */
  };

  TodayData today{}; /**< Today data when retrieved. New cases of latest date  */

  /**
   * Holds the latest available covid-19 case data of the country
   */
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
    std::optional<uint32_t> cases_per_million_population{}; /**< calculated cases per millions of the population */
  };

  LatestData latest{}; /**< Lates actual cases  */

  /**
   * Holds the the covid-19 case data of the country for a specific date/time
   */
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

  std::vector<TimelineData> timeline{}; /**< Timeline data (list of daily data) */
};

using CountryListObject = std::vector<CountryInfo>;

} // namespace coronan
