#pragma once

#include "coronan/corona-api_datatypes.hpp"

#include <string>

namespace coronan {

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
