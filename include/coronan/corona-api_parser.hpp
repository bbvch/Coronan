#pragma once

#include "coronan/corona-api_datatypes.hpp"

#include <optional>
#include <string>

namespace coronan {

namespace api_parser {
/**
 * Parse a json string for region total data
 * @param json json string. Must have the format as described at
 * https://covid-api.com/api/
 * @return Parsed Covid-19 case data
 */
std::optional<CovidData> parse_region_total(std::string const& json);

/**
 * Parse a json string for a list of region information
 * @param json json string. Must have the format as described at
 * https://covid-api.com/api/
 * @return Region list parsed
 */
RegionListObject parse_regions(std::string const& json);

/**
 * Parse a json string for a list of province information
 * @param json json string. Must have the format as described at
 * https://covid-api.com/api/
 * @return Province list parsed
 */
ProvinceListObject parse_provinces(std::string const& json);

} // namespace api_parser

} // namespace coronan
