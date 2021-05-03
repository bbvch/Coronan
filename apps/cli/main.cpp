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

    auto const& data =
        coronan::CoronaAPIClient{"https://corona-api.com"}.get_country_data(
            country);
    fmt::print("\"datetime\", \"confirmed\", \"death\", \"recovered\", "
               "\"active\"\n");

    for (auto const& data_point : data.timeline)
    {
      fmt::print("{date}, {confirmed}, {deaths}, {recovered}, {active}\n",
                 fmt::arg("date", data_point.date),
                 fmt::arg("confirmed", data_point.confirmed.value_or(0)),
                 fmt::arg("deaths", data_point.deaths.value_or(0)),
                 fmt::arg("recovered", data_point.recovered.value_or(0)),
                 fmt::arg("active", data_point.active.value_or(0)));
    }
  }
  catch (coronan::SSLException const& ex)
  {

    fmt::print(stderr, "SSL Exception: {}\n", ex.displayText());
    exit(EXIT_FAILURE);
  }
  catch (coronan::HTTPClientException const& ex)
  {
    fmt::print(stderr, "HTTPClientException: {}\n", ex.what());
    exit(EXIT_FAILURE);
  }
  catch (std::exception const& ex)
  {
    fmt::print(stderr, "{}\n", ex.what());
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
