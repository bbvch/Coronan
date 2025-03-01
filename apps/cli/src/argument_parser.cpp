#include "argument_parser.hpp"

#include <fmt/base.h>
#include <fmt/chrono.h>
#include <iomanip>
#include <lyra/cli.hpp>
#include <lyra/cli_parser.hpp>
#include <lyra/help.hpp>
#include <lyra/opt.hpp>
#include <lyra/parser.hpp>
#include <sstream>

namespace {
std::chrono::year_month_day parse_date(std::string const& date_string)
{
  std::tm date_time = {};
  std::istringstream date_iss{date_string};
  date_iss >> std::get_time(&date_time, "%Y-%m-%d");

  if (date_iss.fail())
  {
    return {};
  }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wuseless-cast"
  // Ugly but necessary, because std::parse is not available on compilers with only partial c++20 support
  return std::chrono::year_month_day{std::chrono::year{date_time.tm_year + 1900},
                                     std::chrono::month{static_cast<unsigned>(date_time.tm_mon + 1U)},
                                     std::chrono::day{static_cast<unsigned>(date_time.tm_mday)}};
#pragma GCC diagnostic pop
}

} // namespace

namespace coronan_cli {

std::variant<
    std::tuple<std::string, std::optional<std::chrono::year_month_day>, std::optional<std::chrono::year_month_day>>,
    int>
parse_commandline_arguments(lyra::args const& args)
{
  std::string country = "CHE";
  std::string start_date_string;
  std::string end_date_string;
  bool help_request = false;
  const auto command_line_parser =
      lyra::cli_parser() | lyra::help(help_request) | lyra::opt(country, "country")["-c"]["--country"]("Country Code") |
      lyra::opt(start_date_string, "start date")["-s"]["--start_date"]("Date format yyyy-mm-dd") |
      lyra::opt(end_date_string, "end date")["-e"]["--end_date"]("Date format yyyy-mm-dd");

  std::stringstream usage;
  usage << command_line_parser;

  if (auto const result = command_line_parser.parse(args); !result)
  {
    fmt::print(stderr, "Error in comman line: {}\n", result.message());
    fmt::print("{}\n", usage.str());
    return EXIT_FAILURE;
  }

  if (help_request)
  {
    fmt::print("{}\n", usage.str());
    return EXIT_SUCCESS;
  }

  std::optional<std::chrono::year_month_day> start_date = std::nullopt;
  if (not start_date_string.empty())
  {
    if (auto const ymd = parse_date(start_date_string); ymd.ok())
    {
      start_date = ymd;
    }
    else
    {
      fmt::print(stderr, "Start date could not be parsed, please enter in format yyyy-mm-dd.\n");
      fmt::print("{}\n", usage.str());
      return EXIT_FAILURE;
    }
  }

  std::optional<std::chrono::year_month_day> end_date = std::nullopt;
  if (not end_date_string.empty())
  {
    if (auto const ymd = parse_date(end_date_string); ymd.ok())
    {
      end_date = ymd;
    }
    else
    {
      fmt::print(stderr, "End date could not be parsed, please enter in format yyyy-mm-dd.\n");
      fmt::print("{}\n", usage.str());
      return EXIT_FAILURE;
    }
  }

  return std::make_tuple(country, start_date, end_date);
}

} // namespace coronan_cli
