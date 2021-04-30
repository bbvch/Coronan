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

    auto const base_url = std::string{"https://corona-api.com/countries"};
    auto const url = base_url + std::string{"/"} + country;

    if (auto const response = coronan::HTTPClient::get(url);
        response.get_status() == Poco::Net::HTTPResponse::HTTP_OK)
    {

      auto const& data =
          coronan::api_parser::parse_country(response.get_response_body());
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
    else
    {
      fmt::print(
          stderr,
          "Error fetching {url}. Response Status: {reason} ({status_code}).\n",
          fmt::arg("url", url), fmt::arg("reason", response.get_reason()),
          fmt::arg("status_code", response.get_status()));
    }
  }
  catch (coronan::SSLException const& ex)
  {

    fmt::print(stderr, "SSL Exception: {}\n", ex.displayText());
    exit(EXIT_FAILURE);
  }
  catch (std::exception const& ex)
  {
    fmt::print(stderr, "{}\n", ex.what());
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
