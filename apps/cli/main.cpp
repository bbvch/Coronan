#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"
#include "coronan/ssl_initializer.hpp"

#include <lyra/lyra.hpp>

#include <iostream>


int main(int argc, char* argv[])
{
  static auto const ssl_initializer_handler = coronan::SSLInitializer::initialize_with_accept_certificate_handler();
  try
  {
    std::string country = "ch";
    bool help_request = false;
    auto cli = lyra::cli_parser() | lyra::help(help_request) |
               lyra::opt(country, "country")["-c"]["--country"]("Country Code");

    auto const result = cli.parse({argc, argv});
    if (!result)
    {
      std::cerr << "Error in comman line: " << result.errorMessage() << "\n";
      exit(EXIT_FAILURE);
    }

    if (help_request)
    {
      std::cout << cli;
      exit(EXIT_SUCCESS);
    }

    auto const url = "https://corona-api.com/countries/" + country;

    auto response = coronan::HTTPClient::get(url);

    auto const& data = coronan::api_parser::parse(response.get_response_body());
    std::cout << "\"datetime\", \"confirmed\", \"death\", \"recovered\", "
                 "\"active\"\n";

    for (auto const& data_points : data.timeline)
    {
      std::cout << data_points.date << ", " << data_points.confirmed << ", "
                << data_points.deaths << ", " << data_points.recovered << ", "
                << data_points.active << "\n";
    }
  }
  catch (coronan::SSLException const& ex)
  {
    std::cerr << "SSL Exception: " << ex.displayText() << "\n";
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}