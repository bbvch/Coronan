#include "coronan/corona-api_client.hpp"

#include <cstdlib>
#include <fmt/core.h>
#include <lyra/lyra.hpp>
#include <sstream>

namespace {
std::string parse_commandline_arguments(lyra::args const& args);
void print_data(coronan::CountryData const& country_data);
} // namespace

int main(int argc, char* argv[])
{
  std::string const country_code = parse_commandline_arguments({argc, argv});

  try
  {
    auto const country_data = coronan::CoronaAPIClient{}.request_country_data(country_code);
    print_data(country_data);
  }
  catch (coronan::SSLException const& ex)
  {
    fmt::print(stderr, "SSL Exception: {}\n", ex.what());
    std::exit(EXIT_FAILURE);
  }
  catch (coronan::HTTPClientException const& ex)
  {
    fmt::print(stderr, "HTTP Client Exception: {}\n", ex.what());
    std::exit(EXIT_FAILURE);
  }
  catch (std::exception const& ex)
  {
    fmt::print(stderr, "{}\n", ex.what());
    std::exit(EXIT_FAILURE);
  }
  std::exit(EXIT_SUCCESS);
}

namespace {
std::string parse_commandline_arguments(lyra::args const& args)
{
  std::string country = "ch";
  bool help_request = false;
  auto command_line_parser =
      lyra::cli_parser() | lyra::help(help_request) | lyra::opt(country, "country")["-c"]["--country"]("Country Code");

  std::stringstream usage;
  usage << command_line_parser;

  if (auto const result = command_line_parser.parse(args); !result)
  {
    fmt::print(stderr, "Error in comman line: {}\n", result.errorMessage());
    fmt::print("{}\n", usage.str());
    std::exit(EXIT_FAILURE);
  }

  if (help_request)
  {
    fmt::print("{}\n", usage.str());
    std::exit(EXIT_SUCCESS);
  }
  return country;
}

void print_data(coronan::CountryData const& country_data)
{

  constexpr auto optional_to_string = [](auto const& value) {
    return value.has_value() ? std::to_string(value.value()) : "--";
  };
  fmt::print("datetime, confirmed, death, recovered, active\n");
  for (auto const& data_point : country_data.timeline)
  {
    fmt::print("{}, {}, {}, {}, {}\n", data_point.date, optional_to_string(data_point.confirmed),
               optional_to_string(data_point.deaths), optional_to_string(data_point.recovered),
               optional_to_string(data_point.active));
  }
}
} // namespace
