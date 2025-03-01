#pragma once

#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"
#include "coronan/ssl_client.hpp"

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <chrono>
#include <fmt/base.h>
#include <future>
#include <string>
#include <variant>
#include <vector>

namespace coronan {

namespace {
inline constexpr auto corona_api_url = "https://covid-api.com/api/";

template <class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};

// The number of conrurrent (asynch) http get calls
// with a too large number you might get HTTP 502 errors
inline constexpr auto number_of_parallel_get = 40;

} // namespace

using HTTPClient = HTTPClientType<Poco::Net::HTTPSClientSession, Poco::Net::HTTPRequest, Poco::Net::HTTPResponse>;

/**
 * A Client for retrieving data from https://covid-api.com.
 */
template <typename ClientType>
class CoronaAPIClientType
{
public:
  /**
   *  Get the list of available countries
   *  @return List of available countries with Covid-19 case data
   */
  [[nodiscard]] std::vector<RegionInfo> request_regions() const;

  /**
   *  Get the list of available provinces of a region
   *  @param iso_code ISO Country Code of the region
   *  @return List of available provinces with Covid-19 case data
   */
  [[nodiscard]] std::vector<ProvinceInfo> request_provinces(std::string_view iso_code) const;

  /**
   *  Get the covid-19 case data for a country latest or for a specific date
   * @param country_code Country Code
   * @param date date, optional, if empty the latest data is returned
   * @return Covid-19 case data for country @p country_code on date @p date
   */
  [[nodiscard]] CountryData request_country_data(std::string_view country_code,
                                                 std::optional<std::chrono::year_month_day> const& date) const;

  /**
   *  Get the covid-19 case data for a country latest or for a specific date
   * @param country_code Country Code
   * @param start_date start_date
   * @param end_date end date
   * @return Covid-19 case data from @p start_date to @p end_date for country @p country_code
   */
  [[nodiscard]] CountryData request_country_data(std::string_view country_code,
                                                 std::chrono::year_month_day const& start_date,
                                                 std::chrono::year_month_day const& end_date) const;

private:
  std::unique_ptr<SSLClient> ssl_client = SSLClient::create_with_accept_certificate_handler();
};

using CoronaAPIClient = CoronaAPIClientType<HTTPClient>;

namespace {
constexpr auto create_exception_msg = [](auto const& url, auto const& response) {
  return std::string{"Error fetching data from url \""} + url + std::string{"\".\n\n Response status: "} +
         response.reason() + std::string{" ("} + std::to_string(response.status()) + std::string{")."};
};
} // namespace

template <typename ClientType>
std::vector<RegionInfo> CoronaAPIClientType<ClientType>::request_regions() const
{
  auto const regions_url = corona_api_url + std::string{"regions"};
  auto const http_response = ClientType::get(regions_url);
  if (http_response.status() == Poco::Net::HTTPResponse::HTTP_OK)
  {
    return coronan::api_parser::parse_regions(http_response.response_body());
  }
  else
  {
    auto const exception_msg = create_exception_msg(regions_url, http_response);
    throw HTTPClientException{exception_msg};
  }
}
template <typename ClientType>
std::vector<ProvinceInfo> CoronaAPIClientType<ClientType>::request_provinces(std::string_view iso_code) const
{
  auto const provinces_url = corona_api_url + std::string{"provinces/"} + std::string{iso_code};
  auto const http_response = ClientType::get(provinces_url);
  if (http_response.status() == Poco::Net::HTTPResponse::HTTP_OK)
  {
    return coronan::api_parser::parse_provinces(http_response.response_body());
  }
  else
  {
    auto const exception_msg = create_exception_msg(provinces_url, http_response);
    throw HTTPClientException{exception_msg};
  }
}

template <typename ClientType>
CountryData
CoronaAPIClientType<ClientType>::request_country_data(std::string_view country_code,
                                                      std::optional<std::chrono::year_month_day> const& date) const
{
  auto const date_query_string = date.has_value() ? std::format("date={:%Y-%m-%d}&", date.value()) : std::string{""};
  auto const region_report_url = corona_api_url + std::string{"reports/total?"} + date_query_string +
                                 std::string{"iso="} + std::string{country_code};
  auto const http_response = ClientType::get(region_report_url);
  if (http_response.status() == Poco::Net::HTTPResponse::HTTP_OK)
  {
    auto country_data = CountryData{};
    country_data.info.iso_code = country_code;
    if (auto const covid_data = coronan::api_parser::parse_region_total(http_response.response_body());
        covid_data.has_value())
    {
      if (date.has_value())
      {
        country_data.timeline.emplace_back(covid_data.value());
      }
      else
      {
        country_data.latest = covid_data.value();
      }
    }
    return country_data;
  }
  else
  {
    auto const exception_msg = create_exception_msg(region_report_url, http_response);
    throw HTTPClientException{exception_msg};
  }
}

template <typename ClientType>
CountryData CoronaAPIClientType<ClientType>::request_country_data(std::string_view country_code,
                                                                  std::chrono::year_month_day const& start_date,
                                                                  std::chrono::year_month_day const& end_date) const
{
  auto country_data = CountryData{};
  country_data.info.iso_code = country_code;
  auto start = std::chrono::sys_days{start_date};
  auto end = std::chrono::sys_days{end_date};

  std::vector<std::vector<std::chrono::sys_days>> dates_to_parse;

  std::vector<std::chrono::sys_days> days{};

  for (auto day = start; day <= end; day += std::chrono::days{1})
  {
    days.emplace_back(day);
    if (days.size() == number_of_parallel_get)
    {
      dates_to_parse.emplace_back(days);
      days = std::vector<std::chrono::sys_days>{};
    }
  }

  if (not days.empty())
  {
    dates_to_parse.emplace_back(days);
  }

  std::ranges::for_each(dates_to_parse, [&](const auto& days) {
    using GetDataResult = std::variant<std::optional<CovidData> const, std::string const>;
    std::vector<std::future<GetDataResult>> response_results;

    for (const auto& day : days)
    {
      const auto get_covid_data = [day, country_code]() -> GetDataResult {
        auto const date = std::chrono::year_month_day{day};
        auto const date_query_string = std::format("date={:%Y-%m-%d}&", date);
        auto const region_report_url = corona_api_url + std::string{"reports/total?"} + date_query_string +
                                       std::string{"iso="} + std::string{country_code};

        if (auto const http_response = ClientType::get(region_report_url);
            http_response.status() == Poco::Net::HTTPResponse::HTTP_OK)
        {
          return coronan::api_parser::parse_region_total(http_response.response_body());
        }
        else
        {
          return create_exception_msg(region_report_url, http_response);
        }
      };
      response_results.emplace_back(std::async(std::launch::async, get_covid_data));
    };

    for (auto const& result : response_results)
    {
      result.wait();
    }

    for (auto& result : response_results)
    {
      auto const http_response = result.get();

      std::visit(overloaded{[&](std::optional<CovidData> const& covid_data) {
                              if (covid_data.has_value())
                              {
                                country_data.timeline.emplace_back(covid_data.value());
                              }
                            },
                            [](std::string const& exception_msg) {
                              fmt::print(stderr, "HTTPClientException {}\n", exception_msg);
                            }},
                 http_response);
    }
  });
  std::ranges::sort(country_data.timeline, [](auto const& lhs, auto const& rhs) { return lhs.date < rhs.date; });

  return country_data;
}
} // namespace coronan
