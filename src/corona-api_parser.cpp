#include "coronan/corona-api_parser.hpp"

#include <algorithm>
#include <rapidjson/document.h>

namespace coronan::api_parser {

namespace {

template <typename Ret_T, typename DOM_T,
          std::enable_if_t<std::is_arithmetic<Ret_T>::value, bool> = true>
std::optional<Ret_T> get_value(DOM_T const& json_dom_object,
                               std::string const& name)
{
  if (auto member_it = json_dom_object.FindMember(name.c_str());
      member_it != json_dom_object.MemberEnd())
  {
    if (auto const& value = member_it->value; value.IsNumber())
    {
      if constexpr (std::is_floating_point<Ret_T>::value)
      {
        if (value.IsDouble())
        {
          return value.GetDouble();
        }
      }
      else if constexpr (std::is_same<Ret_T, unsigned>::value)
      {
        return value.GetUint();
      }
      else if constexpr (std::is_same<Ret_T, int>::value)
      {
        return value.GetInt();
      }
      else if constexpr (std::is_same<Ret_T, uint64_t>::value)
      {
        return value.GetUint64();
      }
      else if constexpr (std::is_same<Ret_T, int64_t>::value)
      {
        return value.GetInt64();
      }
    }
  }
  return std::nullopt;
}

template <
    typename Ret_T, typename DOM_T,
    std::enable_if_t<std::is_same<Ret_T, std::string>::value, bool> = true>
Ret_T get_value(DOM_T const& json_dom_object, std::string const& name)
{
  if (auto member_it = json_dom_object.FindMember(name.c_str());
      member_it != json_dom_object.MemberEnd())
  {
    auto const& value = member_it->value;
    if (value.IsString())
    {
      return value.GetString();
    }
    if (value.IsNumber())
    {
      return value.IsDouble() ? std::to_string(value.GetDouble())
                              : std::to_string(value.GetInt64());
    }
  }
  return "";
}

constexpr auto parse_today_data = [](auto const& json_dom_object) {
  CountryObject::today_t today{};
  if (json_dom_object.HasMember("today"))
  {
    auto const today_object = json_dom_object["today"].GetObject();
    today.deaths = get_value<uint32_t>(today_object, "deaths");
    today.confirmed = get_value<uint32_t>(today_object, "confirmed");
  }
  return today;
};

constexpr auto parse_latest_data = [](auto const& json_dom_object) {
  CountryObject::latest_t latest{};
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
      latest.recovered_vs_death_ratio =
          get_value<double>(calculated, "recovered_vs_death_ratio");
      latest.cases_per_million_population =
          get_value<uint32_t>(calculated, "cases_per_million_population");
    }
  }
  return latest;
};

constexpr auto parse_timeline = [](auto const& json_dom_object) {
  std::vector<CountryObject::timeline_t> timeline;
  if (json_dom_object.HasMember("timeline"))
  {
    for (auto const& data_point : json_dom_object["timeline"].GetArray())
    {
      CountryObject::timeline_t timepoint;
      timepoint.date = get_value<std::string>(data_point, "updated_at");
      timepoint.deaths = get_value<uint32_t>(data_point, "deaths");
      timepoint.confirmed = get_value<uint32_t>(data_point, "confirmed");
      timepoint.recovered = get_value<uint32_t>(data_point, "recovered");
      timepoint.active = get_value<uint32_t>(data_point, "active");
      timepoint.new_confirmed =
          get_value<uint32_t>(data_point, "new_confirmed");
      timepoint.new_recovered =
          get_value<uint32_t>(data_point, "new_recovered");
      timepoint.new_deaths = get_value<uint32_t>(data_point, "new_deaths");
      timeline.emplace_back(timepoint);
    }
  }
  return timeline;
};

} // namespace

// cppcheck-suppress unusedFunction
// Justification: Is used
CountryObject parse_country(std::string const& json)
{
  rapidjson::Document document;
  document.Parse<rapidjson::kParseFullPrecisionFlag>(json.c_str());
  auto country_object = CountryObject{};
  if (document.HasMember("data"))
  {
    auto const country_data_object = document["data"].GetObject();
    country_object.name = get_value<std::string>(country_data_object, "name");
    country_object.country_code =
        get_value<std::string>(country_data_object, "code");
    country_object.population =
        get_value<uint32_t>(country_data_object, "population");
    country_object.today = parse_today_data(country_data_object);

    auto const current_date =
        get_value<std::string>(country_data_object, "updated_at");
    country_object.today.date = current_date;
    country_object.latest = parse_latest_data(country_data_object);
    country_object.latest.date = current_date;
    country_object.timeline = parse_timeline(country_data_object);
  }
  return country_object;
}
// cppcheck-suppress unusedFunction
// Justification: Is used
OverviewObject parse_countries(std::string const& json)
{
  rapidjson::Document document;
  document.Parse(json.c_str());
  auto overview_object = OverviewObject{};
  for (auto const& country_data : document["data"].GetArray())
  {
    OverviewObject::country_t country;
    country.name = get_value<std::string>(country_data, "name");
    country.code = get_value<std::string>(country_data, "code");
    overview_object.countries.emplace_back(country);
  }
  return overview_object;
}

} // namespace coronan::api_parser
