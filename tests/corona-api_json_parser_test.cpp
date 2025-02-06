#include "coronan/corona-api_parser.hpp"

#include <catch2/catch_test_macros.hpp>

namespace {

TEST_CASE("The corona-api parser parsing a full json", "[corona-api parser]")
{
  constexpr auto test_json = "{ \
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

  auto json_object = coronan::api_parser::parse_country(test_json);
  SECTION("returns the country data")
  {
    REQUIRE(json_object.info.name == "Switzerland");
    REQUIRE(json_object.info.iso_code == "CH");
    REQUIRE(json_object.info.population == 7581000);
  }

  SECTION("returns the today cases")
  {
    REQUIRE(json_object.today.date == "2020-04-03T00:27:34.432Z");
    REQUIRE(json_object.today.deaths == 48);
    REQUIRE(json_object.today.confirmed == 1059);
  }

  SECTION("returns the lates data")
  {
    REQUIRE(json_object.latest.date == "2020-04-03T00:27:34.432Z");
    REQUIRE(json_object.latest.deaths == 536);
    REQUIRE(json_object.latest.confirmed == 18827);
    REQUIRE(json_object.latest.recovered == 4013);
    REQUIRE(json_object.latest.critical == 348);
    REQUIRE(json_object.latest.death_rate == 2.8469750889679712);
    REQUIRE(json_object.latest.recovery_rate == 21.315132522441175);
    REQUIRE_FALSE(json_object.latest.recovered_vs_death_ratio.has_value());
    REQUIRE(json_object.latest.cases_per_million_population == 2175);
  }

  SECTION("returns the timeline data ascending")
  {
    REQUIRE(json_object.timeline[0].date == "2020-04-03T00:20:32.326Z");
    REQUIRE(json_object.timeline[0].deaths == 536);
    REQUIRE(json_object.timeline[0].confirmed == 18827);
    REQUIRE(json_object.timeline[0].active == 14278);
    REQUIRE(json_object.timeline[0].recovered == 4013);
    REQUIRE(json_object.timeline[0].new_confirmed == 1059);
    REQUIRE(json_object.timeline[0].new_recovered == 1046);
    REQUIRE(json_object.timeline[0].new_deaths == 48);
    REQUIRE(json_object.timeline[1].date == "2020-04-01T19:58:34.000Z");
    REQUIRE(json_object.timeline[1].deaths == 488);
    REQUIRE(json_object.timeline[1].confirmed == 17768);
    REQUIRE(json_object.timeline[1].active == 14313);
    REQUIRE(json_object.timeline[1].recovered == 2967);
    REQUIRE(json_object.timeline[1].new_confirmed == 1163);
    REQUIRE(json_object.timeline[1].new_recovered == 1144);
    REQUIRE(json_object.timeline[1].new_deaths == 55);
  }
}

TEST_CASE("The corona-api parser parsing a partial json", "[corona-api parser]")
{
  SECTION("with missing population returns no value for population")
  {
    constexpr auto test_json = "{ \
        \"data\": { \
            \"coordinates\": { \
                \"latitude\": 47, \
                \"longitude\": 8 \
            }, \
            \"name\": \"Switzerland\", \
            \"code\": \"CH\", \
            \"timeline\": [ ] \
        } \
    }";

    auto json_object = coronan::api_parser::parse_country(test_json);

    REQUIRE_FALSE(json_object.info.population.has_value());
  }

  SECTION("with missing today cases return no today cases")
  {
    constexpr auto test_json = "{ \
        \"data\": { \
            \"coordinates\": { \
                \"latitude\": 47, \
                \"longitude\": 8 \
            }, \
            \"name\": \"Switzerland\", \
            \"code\": \"CH\", \
            \"population\": 7581000, \
            \"updated_at\": \"2020-04-03T00:27:34.432Z\", \
            \"timeline\": [ ] \
        } \
    }";

    auto json_object = coronan::api_parser::parse_country(test_json);

    REQUIRE(json_object.today.date == "2020-04-03T00:27:34.432Z");
    REQUIRE_FALSE(json_object.today.deaths.has_value());
    REQUIRE_FALSE(json_object.today.confirmed.has_value());
  }

  SECTION("with missing latest_data return no latest_data")
  {
    constexpr auto test_json = "{ \
            \"data\": { \
                \"coordinates\": { \
                    \"latitude\": 47, \
                    \"longitude\": 8 \
                }, \
                \"name\": \"Switzerland\", \
                \"code\": \"CH\", \
                \"population\": 7581000, \
                \"updated_at\": \"2020-04-03T00:27:34.432Z\", \
                \"timeline\": [ ] \
            } \
        }";

    auto json_object = coronan::api_parser::parse_country(test_json);

    REQUIRE(json_object.latest.date == "2020-04-03T00:27:34.432Z");
    REQUIRE_FALSE(json_object.today.confirmed.has_value());
    REQUIRE_FALSE(json_object.latest.deaths.has_value());
    REQUIRE_FALSE(json_object.latest.confirmed.has_value());
    REQUIRE_FALSE(json_object.latest.recovered.has_value());
    REQUIRE_FALSE(json_object.latest.critical.has_value());
    REQUIRE_FALSE(json_object.latest.death_rate.has_value());
    REQUIRE_FALSE(json_object.latest.recovery_rate.has_value());
    REQUIRE_FALSE(json_object.latest.recovered_vs_death_ratio.has_value());
    REQUIRE_FALSE(json_object.latest.cases_per_million_population.has_value());
  }

  SECTION("with missing updated_at and latest_data return empty latest_data date")
  {
    constexpr auto test_json = "{ \
            \"data\": { \
                \"coordinates\": { \
                    \"latitude\": 47, \
                    \"longitude\": 8 \
                }, \
                \"name\": \"Switzerland\", \
                \"code\": \"CH\", \
                \"population\": 7581000, \
                \"timeline\": [ ] \
            } \
        }";

    auto json_object = coronan::api_parser::parse_country(test_json);

    REQUIRE(json_object.latest.date == "");
    REQUIRE_FALSE(json_object.today.confirmed.has_value());
    REQUIRE_FALSE(json_object.latest.deaths.has_value());
    REQUIRE_FALSE(json_object.latest.confirmed.has_value());
    REQUIRE_FALSE(json_object.latest.recovered.has_value());
    REQUIRE_FALSE(json_object.latest.critical.has_value());
    REQUIRE_FALSE(json_object.latest.death_rate.has_value());
    REQUIRE_FALSE(json_object.latest.recovery_rate.has_value());
    REQUIRE_FALSE(json_object.latest.recovered_vs_death_ratio.has_value());
    REQUIRE_FALSE(json_object.latest.cases_per_million_population.has_value());
  }

  SECTION("with missing timeline data return empty timeline vector")
  {
    constexpr auto test_json = "{ \
        \"data\": { \
            \"coordinates\": { \
                \"latitude\": 47, \
                \"longitude\": 8 \
            }, \
            \"name\": \"Switzerland\", \
            \"code\": \"CH\", \
            \"population\": 7581000, \
            \"updated_at\": \"2020-04-03T00:27:34.432Z\" \
        } \
    }";

    auto json_object = coronan::api_parser::parse_country(test_json);
    REQUIRE(json_object.timeline.size() == 0);
  }
}

TEST_CASE("The corona-api country parser parsing a country list", "[corona-api parser]")
{
  constexpr auto test_country_json = "{ \
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

  auto countries = coronan::api_parser::parse_countries(test_country_json);
  SECTION("returns the country data")
  {
    REQUIRE(countries[0].name == "Austria");
    REQUIRE(countries[0].iso_code == "AT");
    REQUIRE(countries[1].name == "Italy");
    REQUIRE(countries[1].iso_code == "IT");
    REQUIRE(countries[2].name == "Switzerland");
    REQUIRE(countries[2].iso_code == "CH");
  }
}

} // namespace
