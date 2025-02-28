#include "coronan/corona-api_client.hpp"
#include "coronan/corona-api_datatypes.hpp"
#include "coronan/http_client.hpp"
#include "coronan/ssl_client.hpp"

#include <chrono>
#include <cstdlib>
#include <exception>
#include <fmt/base.h>
#include <lyra/args.hpp>
#include <lyra/cli.hpp>
#include <lyra/cli_parser.hpp>
#include <lyra/help.hpp>
#include <lyra/opt.hpp>
#include <lyra/parser.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>

namespace {
std::variant<std::tuple<std::string, std::optional<std::chrono::year_month_day>>, int>
parse_commandline_arguments(lyra::args const& args);
void print_data(coronan::CountryData const& country_data);
} // namespace

int main(int argc, char* argv[])
{
  try
  {
    auto const arguments_or_exit_code = parse_commandline_arguments({argc, argv});
    if (std::holds_alternative<int>(arguments_or_exit_code))
    {
      return std::get<int>(arguments_or_exit_code);
    }
    auto const& [country, date] =
        std::get<std::tuple<std::string, std::optional<std::chrono::year_month_day>>>(arguments_or_exit_code);
    auto const country_data = coronan::CoronaAPIClient{}.request_country_data(country, date);
    print_data(country_data);
  }
  catch (coronan::SSLException const& ex)
  {
    fmt::print(stderr, "SSL Exception: {}\n", ex.what());
    return EXIT_FAILURE;
  }
  catch (coronan::HTTPClientException const& ex)
  {
    fmt::print(stderr, "HTTP Client Exception: {}\n", ex.what());
    return EXIT_FAILURE;
  }
  catch (std::exception const& ex)
  {
    fmt::print(stderr, "{}\n", ex.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

namespace {
std::variant<std::tuple<std::string, std::optional<std::chrono::year_month_day>>, int>
parse_commandline_arguments(lyra::args const& args)
{
  std::string country = "ch";
  std::string date_string = "";
  bool help_request = false;
  auto command_line_parser = lyra::cli_parser() | lyra::help(help_request) |
                             lyra::opt(country, "country")["-c"]["--country"]("Country Code") |
                             lyra::opt(country, "date_string")["-d"]["--date"]("Date format yyyy-mm-dd");

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

  std::optional<std::chrono::year_month_day> date = std::nullopt;
  if (not date_string.empty())
  {
    std::istringstream iss{date_string};
    iss.imbue(std::locale("en_US.utf-8"));
    std::chrono::year_month_day ymd;
    std::chrono::from_stream(iss, "%Y-%m-%d", ymd);
    if (iss.fail())
    {
      fmt::print(stderr, "Date could not be parsed, please enter in format yyyy-mm-dd.\n");
      fmt::print("{}\n", usage.str());
      return EXIT_FAILURE;
    }
    date = ymd;
  }

  return std::make_tuple(country, date);
}

void print_data(coronan::CountryData const& country_data)
{

  constexpr auto optional_to_string = [](auto const& value) {
    return value.has_value() ? std::to_string(value.value()) : "--";
  };
  fmt::print("datetime, confirmed, death, recovered, active\n");
  for (auto const& data_point : country_data.timeline)
  {
    fmt::print("{}, {}, {}, {}, {}\n", std::format("{:%Y-%m-%d}", data_point.date),
               optional_to_string(data_point.confirmed), optional_to_string(data_point.deaths),
               optional_to_string(data_point.recovered), optional_to_string(data_point.active));
  }
}
} // namespace
