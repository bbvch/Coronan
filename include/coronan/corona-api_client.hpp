#pragma once

#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"
#include "coronan/ssl_initializer.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace coronan {

struct Country
{
  std::string name{}; /**< Country name */
  std::string code{}; /**< Country Code , e.g. ch */
};

template <typename HTTPClientT = HTTPClient> class CoronaAPIClient
{
public:
  explicit CoronaAPIClient(std::string url)
      : api_url{std::move(url)},
        ssl_initializer_handler{
            coronan::SSLInitializer::
                initialize_with_accept_certificate_handler()}
  {
  }

  std::vector<Country> get_countries()
  {
    auto const countries_url = api_url + std::string{"/countries"};
    if (auto const http_response = HTTPClientT::get(countries_url);
        http_response.get_status() == Poco::Net::HTTPResponse::HTTP_OK)
    {
      auto const country_list = coronan::api_parser::parse_countries(
                                    http_response.get_response_body())
                                    .countries;
      std::vector<Country> countries;

      std::transform(begin(country_list), end(country_list),
                     std::back_inserter(countries), [](auto const& country) {
                       return Country{country.name, country.code};
                     });

      return countries;
    }
    else
    {
      auto const exception_msg =
          std::string{"Error fetching data from url \""} + countries_url +
          std::string{"\".\n\n Response status: "} +
          http_response.get_reason() + std::string{" ("} +
          std::to_string(http_response.get_status()) + std::string{")."};
      throw HTTPClientException{exception_msg};
    }
    return {};
  }

  coronan::CountryObject get_country_data(std::string const& country_code)
  {
    auto const countries_url =
        api_url + std::string{"/countries/"} + country_code;
    if (auto const http_response = HTTPClientT::get(countries_url);
        http_response.get_status() == Poco::Net::HTTPResponse::HTTP_OK)

    {
      return coronan::api_parser::parse_country(
          http_response.get_response_body());
    }
    else
    {
      auto const exception_msg =
          std::string{"Error fetching data from url \""} + countries_url +
          std::string{"\".\n\n Response status: "} +
          http_response.get_reason() + std::string{" ("} +
          std::to_string(http_response.get_status()) + std::string{")."};
      throw HTTPClientException{exception_msg};
    }
    return {};
  }

private:
  std::string const api_url;
  std::unique_ptr<SSLInitializer> ssl_initializer_handler;
};

} // namespace coronan
