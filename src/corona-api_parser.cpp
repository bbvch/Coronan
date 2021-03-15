#include "coronan/corona-api_parser.hpp"

#include "rapidjson/document.h"

#include <algorithm>

namespace coronan::api_parser {

namespace {

constexpr auto parse_today_data = [](auto const& json_dom_object) {
  CountryObject::today_t today{};
  today.deaths = json_dom_object["deaths"].GetInt();
  today.confirmed = json_dom_object["confirmed"].GetInt();
  return today;
};

constexpr auto parse_lates_data = [](auto const& json_dom_object) {
  CountryObject::latest_t latest{};
  latest.deaths = json_dom_object["deaths"].GetInt();
  latest.confirmed = json_dom_object["confirmed"].GetInt();
  latest.recovered = json_dom_object["recovered"].GetInt();
  latest.critical = json_dom_object["critical"].GetInt();
  auto const calculated = json_dom_object["calculated"].GetObject();
  if (calculated["death_rate"].IsNumber())
  {
    latest.death_rate = calculated["death_rate"].GetDouble();
  }
  if (calculated["recovery_rate"].IsNumber())
  {
    latest.recovery_rate = calculated["recovery_rate"].GetDouble();
  }
  if (calculated["recovered_vs_death_ratio"].IsNumber())
  {
    latest.recovered_vs_death_ratio =
        calculated["recovered_vs_death_ratio"].GetDouble();
  }
  if (calculated["cases_per_million_population"].IsNumber())
  {
    latest.cases_per_million_population =
        calculated["cases_per_million_population"].GetDouble();
  }
  return latest;
};

constexpr auto parse_timeline = [](auto const& json_dom_array) {
  std::vector<CountryObject::timeline_t> timeline;
  for (auto const& data_point : json_dom_array)
  {
    CountryObject::timeline_t timepoint;
    timepoint.date = data_point["updated_at"].GetString();
    timepoint.deaths = data_point["deaths"].GetInt();
    timepoint.confirmed = data_point["confirmed"].GetInt();
    timepoint.recovered = data_point["recovered"].GetInt();
    timepoint.active = data_point["active"].GetInt();
    timepoint.new_confirmed = data_point["new_confirmed"].GetInt();
    timepoint.new_recovered = data_point["new_recovered"].GetInt();
    timepoint.new_deaths = data_point["new_deaths"].GetInt();
    timeline.emplace_back(timepoint);
  }
  return timeline;
};

} // namespace

CountryObject parse(std::string const& json)
{
  rapidjson::Document document{};
  document.Parse<rapidjson::kParseFullPrecisionFlag>(json.c_str());
  auto country_object = CountryObject{};
  auto const country_data_object = document["data"].GetObject();
  country_object.name = country_data_object["name"].GetString();
  country_object.country_code = country_data_object["code"].GetString();
  country_object.population = country_data_object["population"].GetInt();
  country_object.today =
      parse_today_data(country_data_object["today"].GetObject());
  auto const* const current_date =
      country_data_object["updated_at"].GetString();
  country_object.today.date = current_date;
  country_object.latest =
      parse_lates_data(country_data_object["latest_data"].GetObject());
  country_object.latest.date = current_date;
  country_object.timeline =
      parse_timeline(country_data_object["timeline"].GetArray());
  return country_object;
}

OverviewObject parse_countries(std::string const& json)
{
  rapidjson::Document document{};
  document.Parse(json.c_str());
  auto overview_object = OverviewObject{};
  for (auto const& country_data : document["data"].GetArray())
  {
    OverviewObject::country_t country;
    country.name = country_data["name"].GetString();
    country.code = country_data["code"].GetString();
    overview_object.countries.emplace_back(country);
  }
  return overview_object;
}

} // namespace coronan::api_parser
