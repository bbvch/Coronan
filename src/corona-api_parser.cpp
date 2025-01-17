#include "coronan/corona-api_parser.hpp"

#include <cstdint>
#include <optional>
#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/encodings.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/reader.h>
#include <vector>

namespace coronan::api_parser {

namespace {

template <typename Ret_T, typename DOM_T>
std::optional<Ret_T> get_value(DOM_T const& json_dom_object, std::string const& name)
  requires std::is_arithmetic_v<Ret_T>
{
  if (auto member_it = json_dom_object.FindMember(name.c_str()); member_it != json_dom_object.MemberEnd())
  {
    if (auto const& value = member_it->value; value.IsNumber())
    {
      if constexpr (std::is_floating_point_v<Ret_T>)
      {
        if (value.IsDouble())
        {
          return value.GetDouble();
        }
      }
      else if constexpr (std::is_same_v<Ret_T, unsigned>)
      {
        return value.GetUint();
      }
      // cppcheck-suppress identicalConditionAfterEarlyExit
      // cppcheck-suppress multiCondition
      else if constexpr (std::is_same_v<Ret_T, int>)
      {
        return value.GetInt();
      }
      else if constexpr (std::is_same_v<Ret_T, uint64_t>)
      {
        return value.GetUint64();
      }
      else if constexpr (std::is_same_v<Ret_T, int64_t>)
      {
        return value.GetInt64();
      }
    }
  }
  return std::nullopt;
}

template <typename Ret_T, typename DOM_T>
Ret_T get_value(DOM_T const& json_dom_object, std::string const& name)
  requires(std::is_same_v<Ret_T, std::string>)
{
  if (auto member_it = json_dom_object.FindMember(name.c_str()); member_it != json_dom_object.MemberEnd())
  {
    auto const& value = member_it->value;
    if (value.IsString())
    {
      return value.GetString();
    }
    if (value.IsNumber())
    {
      return value.IsDouble() ? std::to_string(value.GetDouble()) : std::to_string(value.GetInt64());
    }
  }
  return "";
}

constexpr auto parse_today_data = [](auto const& json_dom_object) {
  CountryData::TodayData today{};
  if (json_dom_object.HasMember("today"))
  {
    auto const today_object = json_dom_object["today"].GetObject();
    today.deaths = get_value<uint32_t>(today_object, "deaths");
    today.confirmed = get_value<uint32_t>(today_object, "confirmed");
  }
  return today;
};

constexpr auto parse_latest_data = [](auto const& json_dom_object) {
  CountryData::LatestData latest{};
  if (json_dom_object.HasMember("latest_data"))
  {
    auto const latest_data = json_dom_object["latest_data"].GetObject();
    latest.deaths = get_value<uint32_t>(latest_data, "deaths");
    latest.confirmed = get_value<uint32_t>(latest_data, "confirmed");
    latest.recovered = get_value<uint32_t>(latest_data, "recovered");
    latest.critical = get_value<uint32_t>(latest_data, "critical");
    if (latest_data.HasMember("calculated"))
    {
      auto const calculated = latest_data["calculated"].GetObject();
      latest.death_rate = get_value<double>(calculated, "death_rate");
      latest.recovery_rate = get_value<double>(calculated, "recovery_rate");
      latest.recovered_vs_death_ratio = get_value<double>(calculated, "recovered_vs_death_ratio");
      latest.cases_per_million_population = get_value<uint32_t>(calculated, "cases_per_million_population");
    }
  }
  return latest;
};

constexpr auto parse_timeline = [](auto const& json_dom_object) {
  std::vector<CountryData::TimelineData> timeline;
  if (json_dom_object.HasMember("timeline"))
  {
    for (auto const& data_point : json_dom_object["timeline"].GetArray())
    {
      CountryData::TimelineData timepoint;
      timepoint.date = get_value<std::string>(data_point, "updated_at");
      timepoint.deaths = get_value<uint32_t>(data_point, "deaths");
      timepoint.confirmed = get_value<uint32_t>(data_point, "confirmed");
      timepoint.recovered = get_value<uint32_t>(data_point, "recovered");
      timepoint.active = get_value<uint32_t>(data_point, "active");
      timepoint.new_confirmed = get_value<uint32_t>(data_point, "new_confirmed");
      timepoint.new_recovered = get_value<uint32_t>(data_point, "new_recovered");
      timepoint.new_deaths = get_value<uint32_t>(data_point, "new_deaths");
      timeline.emplace_back(timepoint);
    }
  }
  return timeline;
};

} // namespace

// cppcheck-suppress unusedFunction
CountryData parse_country(std::string const& json)
{
  rapidjson::Document document;
  document.Parse<rapidjson::kParseFullPrecisionFlag>(json.c_str());
  auto country_data = CountryData{};
  if (document.HasMember("data"))
  {
    auto const country_data_object = document["data"].GetObject();
    country_data.info.name = get_value<std::string>(country_data_object, "name");
    country_data.info.iso_code = get_value<std::string>(country_data_object, "code");
    country_data.info.population = get_value<uint32_t>(country_data_object, "population");
    country_data.today = parse_today_data(country_data_object);

    auto const current_date = get_value<std::string>(country_data_object, "updated_at");
    country_data.today.date = current_date;
    country_data.latest = parse_latest_data(country_data_object);
    country_data.latest.date = current_date;
    country_data.timeline = parse_timeline(country_data_object);
  }
  return country_data;
}
// cppcheck-suppress unusedFunction
CountryListObject parse_countries(std::string const& json)
{
  rapidjson::Document document;
  document.Parse(json.c_str());
  auto country_list = CountryListObject{};
  for (auto const& country_data : document["data"].GetArray())
  {
    CountryInfo country;
    country.name = get_value<std::string>(country_data, "name");
    country.iso_code = get_value<std::string>(country_data, "code");
    country_list.emplace_back(country);
  }
  return country_list;
}

} // namespace coronan::api_parser
