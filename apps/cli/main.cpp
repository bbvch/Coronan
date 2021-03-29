#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"
#include "coronan/ssl_initializer.hpp"

#include <fmt/core.h>
#include <lyra/lyra.hpp>
#include <sstream>

int main(int argc, char* argv[])
{
  static auto const ssl_initializer_handler =
      coronan::SSLInitializer::initialize_with_accept_certificate_handler();
  try
  {
    std::string country = "ch";
    bool help_request = false;
    auto cli = lyra::cli_parser() | lyra::help(help_request) |
               lyra::opt(country, "country")["-c"]["--country"]("Country Code");

    std::stringstream usage;
    usage << cli;

    auto const result = cli.parse({argc, argv});
    if (!result)
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

    auto const url = "https://corona-api.com/countries/" + country;

    auto const response = coronan::HTTPClient::get(url);

    auto const& data = coronan::api_parser::parse(response.get_response_body());
    fmt::print("\"datetime\", \"confirmed\", \"death\", \"recovered\", "
               "\"active\"\n");

    for (auto const& data_point : data.timeline)
    {
      fmt::print("{date}, {confirmed}, {deaths}, {recovered}, {active}\n",
                 fmt::arg("date", data_point.date),
                 fmt::arg("confirmed", data_point.confirmed),
                 fmt::arg("deaths", data_point.deaths),
                 fmt::arg("recovered", data_point.recovered),
                 fmt::arg("active", data_point.active));
    }
  }
  catch (coronan::SSLException const& ex)
  {

    fmt::print(stderr, "SSL Exception: {}\n", ex.displayText());
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
