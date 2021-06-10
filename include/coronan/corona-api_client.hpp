#pragma once

#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"
#include "coronan/ssl_client.hpp"

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <string>
#include <vector>

namespace coronan {

namespace {
constexpr auto corona_api_url = "https://corona-api.com";
}

using HTTPClient = HTTPClientType<Poco::Net::HTTPSClientSession, Poco::Net::HTTPRequest, Poco::Net::HTTPResponse>;

/**
 * A Client for retrieving data from https://corona-api.com.
 */
template <typename ClientType>
class CoronaAPIClientType
{
public:
  /**
   *  Get the list of available countries
   *  @return List of available countries with Covid-19 case data
   */
  std::vector<CountryInfo> request_countries() const;
  /**
   *  Get the covid-19 case data for a country
   * @param country_code ISO 3166-1 alpha-2 Country Code
   * @return Covid-19 case data for country <country_code>
   */
  CountryData request_country_data(std::string_view country_code) const;

private:
  std::string const api_url = corona_api_url;
  std::unique_ptr<SSLClient> ssl_client = SSLClient::create_with_accept_certificate_handler();
};

using CoronaAPIClient = CoronaAPIClientType<HTTPClient>;

namespace {
constexpr auto create_exception_msg = [](auto const& url, auto const& response) {
  return std::string{"Error fetching data from url \""} + url + std::string{"\".\n\n Response status: "} +
         response.reason() + std::string{" ("} + std::to_string(response.status()) + std::string{")."};
};
}

template <typename ClientType>
std::vector<CountryInfo> CoronaAPIClientType<ClientType>::request_countries() const
{
  auto const countries_url = api_url + std::string{"/countries"};
  auto const http_response = ClientType::get(countries_url);
  if (http_response.status() == Poco::Net::HTTPResponse::HTTP_OK)
  {
    return coronan::api_parser::parse_countries(http_response.response_body());
  }
  else
  {
    auto const exception_msg = create_exception_msg(countries_url, http_response);
    throw HTTPClientException{exception_msg};
  }
}

template <typename ClientType>
CountryData CoronaAPIClientType<ClientType>::request_country_data(std::string_view country_code) const
{
  auto const country_url = api_url + std::string{"/countries/"} + std::string{country_code};
  auto const http_response = ClientType::get(country_url);
  if (http_response.status() == Poco::Net::HTTPResponse::HTTP_OK)

  {
    return coronan::api_parser::parse_country(http_response.response_body());
  }
  else
  {
    auto const exception_msg = create_exception_msg(country_url, http_response);
    throw HTTPClientException{exception_msg};
  }
}
} // namespace coronan
