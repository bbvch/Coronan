#include "coronan/corona-api_client.hpp"

#include <fmt/core.h>
#include <lyra/lyra.hpp>
#include <sstream>

int main(int argc, char* argv[])
{
  try
  {
    std::string country = "ch";
    bool help_request = false;
    auto cli = lyra::cli_parser() | lyra::help(help_request) |
               lyra::opt(country, "country")["-c"]["--country"]("Country Code");

    std::stringstream usage;
    usage << cli;

    if (auto const result = cli.parse({argc, argv}); !result)
    {
      fmt::print(stderr, "Error in comman line: {}\n", result.errorMessage());
      fmt::print("{}\n", usage.str());
      exit(EXIT_FAILURE);
    }

    if (help_request)
    {
      fmt::print("{}\n", usage.str());
      exit(EXIT_SUCCESS);
    }

    auto const& country_data = coronan::CoronaAPIClient{}.get_country_data(country);
    fmt::print("\"datetime\", \"confirmed\", \"death\", \"recovered\", "
               "\"active\"\n");

    constexpr auto optional_to_string = [](auto const& value) {
      return value.has_value() ? std::to_string(value.value()) : "--";
    };

    for (auto const& data_point : country_data.timeline)
    {
      fmt::print("{}, {}, {}, {}, {}\n", data_point.date, optional_to_string(data_point.confirmed),
                 optional_to_string(data_point.deaths), optional_to_string(data_point.recovered),
                 optional_to_string(data_point.active));
    }
  }
  catch (coronan::SSLException const& ex)
  {
    fmt::print(stderr, "SSL Exception: {}\n", ex.what());
    exit(EXIT_FAILURE);
  }
  catch (coronan::HTTPClientException const& ex)
  {
    fmt::print(stderr, "HTTP Client Exception: {}\n", ex.what());
    exit(EXIT_FAILURE);
  }
  catch (std::exception const& ex)
  {
    fmt::print(stderr, "{}\n", ex.what());
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
