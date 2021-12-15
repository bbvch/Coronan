#include "coronan/corona-api_client.hpp"

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <catch2/catch.hpp>
#include <iostream>
#include <sstream>

namespace {

using Catch::Matchers::Equals;

class TestHTTPClient
{
public:
  static coronan::HTTPResponse get(std::string_view url)
  {
    get_called = true;
    get_url = url;
    return coronan::HTTPResponse{Poco::Net::HTTPResponse{response_status}, response_payload};
  }

  static bool get_was_called_with(std::string_view url)
  {
    return (get_called && (get_url == url));
  }

  static bool get_called;
  static std::string get_url;
  static Poco::Net::HTTPResponse::HTTPStatus response_status;
  static std::string response_payload;
};

bool TestHTTPClient::get_called = false;
std::string TestHTTPClient::get_url = "";
Poco::Net::HTTPResponse::HTTPStatus TestHTTPClient::response_status = Poco::Net::HTTPResponse::HTTP_CONTINUE;
std::string TestHTTPClient::response_payload = "";

SCENARIO("CoronaAPIClient retrieves country list", "[CoronaAPIClient]")
{
  GIVEN("A corona-api client")
  {
    TestHTTPClient::get_called = false;
    TestHTTPClient::get_url = "";
    auto testee = coronan::CoronaAPIClientType<TestHTTPClient>{};

    WHEN("the http client returns an not OK response status")
    {

      TestHTTPClient::response_status = Poco::Net::HTTPResponse::HTTP_NOT_FOUND;
      THEN("an exception is thrown")
      {
        CHECK_THROWS(testee.request_countries());
      }
    }

    WHEN("the http client returns an OK response status and a country list "
         "in the payload")
    {
      TestHTTPClient::response_status = Poco::Net::HTTPResponse::HTTP_OK;
      TestHTTPClient::response_payload = "{ \
              \"data\": [ \
                  { \
                      \"name\": \"Austria\", \
                      \"code\": \"AT\", \
                      \"population\": 8205000 \
                  }, \
                  { \
                      \"name\": \"Italy\", \
                      \"code\": \"IT\", \
                      \"population\": 60340328 \
                  }, \
                  { \
                      \"name\": \"Switzerland\", \
                      \"code\": \"CH\", \
                      \"population\": 7581000 \
                  } \
              ] \
          }";

      auto const countries = testee.request_countries();

      REQUIRE(TestHTTPClient::get_was_called_with("https://corona-api.com/countries"));

      THEN("a list of country names and iso country codes is returned.")
      {
        REQUIRE(countries.size() == 3);
        REQUIRE_THAT(countries[0].name, Equals("Austria"));
        REQUIRE_THAT(countries[0].iso_code, Equals("AT"));
        REQUIRE_THAT(countries[1].name, Equals("Italy"));
        REQUIRE_THAT(countries[1].iso_code, Equals("IT"));
        REQUIRE_THAT(countries[2].name, Equals("Switzerland"));
        REQUIRE_THAT(countries[2].iso_code, Equals("CH"));
      }
    }
  }
}

SCENARIO("CoronaAPIClient retrieves country data for Switzerland", "[CoronaAPIClient]")
{
  GIVEN("A corona-api client")
  {
    TestHTTPClient::get_called = false;
    TestHTTPClient::get_url = "";
    auto testee = coronan::CoronaAPIClientType<TestHTTPClient>{};

    WHEN("the http client returns an not OK response status")
    {

      TestHTTPClient::response_status = Poco::Net::HTTPResponse::HTTP_NOT_FOUND;
      THEN("an exception is thrown")
      {
        CHECK_THROWS(testee.request_country_data("CH"));
      }
    }

    WHEN("the http client returns an OK response status and a payload with "
         "data for Switzerland")
    {
      TestHTTPClient::response_status = Poco::Net::HTTPResponse::HTTP_OK;
      TestHTTPClient::response_payload = "{ \
          \"data\": { \
              \"coordinates\": { \
                  \"latitude\": 47, \
                  \"longitude\": 8 \
              }, \
              \"name\": \"Switzerland\", \
              \"code\": \"CH\", \
              \"population\": 7581000, \
              \"updated_at\": \"2020-04-03T00:27:34.432Z\", \
              \"today\": { \
                  \"deaths\": 48, \
                  \"confirmed\": 1059 \
              }, \
              \"latest_data\": { \
                  \"deaths\": 536, \
                  \"confirmed\": 18827, \
                  \"recovered\": 4013, \
                  \"critical\": 348, \
                  \"calculated\": { \
                      \"death_rate\": 2.8469750889679712, \
                      \"recovery_rate\": 21.315132522441175, \
                      \"recovered_vs_death_ratio\": null, \
                      \"cases_per_million_population\": 2175 \
                  } \
              }, \
              \"timeline\": [ \
                  { \
                      \"updated_at\": \"2020-04-03T00:20:32.326Z\", \
                      \"date\": \"2020-04-03\", \
                      \"deaths\": 536, \
                      \"confirmed\": 18827, \
                      \"active\": 14278, \
                      \"recovered\": 4013, \
                      \"new_confirmed\": 1059, \
                      \"new_recovered\": 1046, \
                      \"new_deaths\": 48, \
                      \"is_in_progress\": true \
                  }, \
                  { \
                      \"updated_at\": \"2020-04-01T19:58:34.000Z\", \
                      \"date\": \"2020-04-01\", \
                      \"deaths\": 488, \
                      \"confirmed\": 17768, \
                      \"recovered\": 2967, \
                      \"new_confirmed\": 1163, \
                      \"new_recovered\": 1144, \
                      \"new_deaths\": 55, \
                      \"active\": 14313 \
                  } \
              ] \
          } \
      }";

      auto const country_data = testee.request_country_data("CH");
      REQUIRE(TestHTTPClient::get_was_called_with("https://corona-api.com/countries/CH"));

      THEN("the country data is returned.")
      {
        REQUIRE(country_data.info.name == "Switzerland");
        REQUIRE(country_data.info.iso_code == "CH");
        REQUIRE(country_data.info.population == 7581000);
        REQUIRE(country_data.today.confirmed == 1059);
      }
    }
  }
}

} // namespace
