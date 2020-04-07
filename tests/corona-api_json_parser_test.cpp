#include "coronan/corona-api_parser.hpp"

#include <doctest/doctest.h>

namespace {

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
    }, \
    \"_cacheHit\": false \
}";

TEST_SUITE_BEGIN("corona-api parser");

TEST_CASE("The corona-api parser")
{

  auto json_object = coronan::api_parser::parse(test_json);
  SUBCASE("returns the country data")
  {
    REQUIRE(json_object.country_name == "Switzerland");
    REQUIRE(json_object.country_code == "CH");
    REQUIRE(json_object.population == 7581000);
  }

  SUBCASE("returns the today cases")
  {
    REQUIRE(json_object.today.date == "2020-04-03T00:27:34.432Z");
    REQUIRE(json_object.today.deaths == 48);
    REQUIRE(json_object.today.confirmed == 1059);
  }

  SUBCASE("returns the lates data")
  {
    REQUIRE(json_object.latest.date == "2020-04-03T00:27:34.432Z");
    REQUIRE(json_object.latest.deaths == 536);
    REQUIRE(json_object.latest.confirmed == 18827);
    REQUIRE(json_object.latest.recovered == 4013);
    REQUIRE(json_object.latest.critical == 348);
    REQUIRE(json_object.latest.death_rate == 2.8469750889679712);
    REQUIRE(json_object.latest.recovery_rate == 21.315132522441175);
    REQUIRE(json_object.latest.cases_per_million_population == 2175);
  }

  SUBCASE("returns the timeline data ascending")
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

constexpr auto test_country_json = "{ \
    \"data\": [ \
        { \
            \"coordinates\": { \
                \"latitude\": 47.3333, \
                \"longitude\": 13.3333 \
            }, \
            \"name\": \"Austria\", \
            \"code\": \"AT\", \
            \"population\": 8205000, \
            \"updated_at\": \"2020-04-06T22:57:01.350Z\", \
            \"today\": { \
                \"deaths\": 16, \
                \"confirmed\": 246 \
            }, \
            \"latest_data\": { \
                \"deaths\": 220, \
                \"confirmed\": 12297, \
                \"recovered\": 3463, \
                \"critical\": 250, \
                \"calculated\": { \
                    \"death_rate\": 1.7890542408717574, \
                    \"recovery_rate\": 28.161340164267706, \
                    \"recovered_vs_death_ratio\": null, \
                    \"cases_per_million_population\": 1365 \
                } \
            } \
        }, \
        { \
            \"coordinates\": { \
                \"latitude\": 42.8333, \
                \"longitude\": 12.8333 \
            }, \
            \"name\": \"Italy\", \
            \"code\": \"IT\", \
            \"population\": 60340328, \
            \"updated_at\": \"2020-04-06T22:57:01.350Z\", \
            \"today\": { \
                \"deaths\": 636, \
                \"confirmed\": 3599 \
            }, \
            \"latest_data\": { \
                \"deaths\": 16523, \
                \"confirmed\": 132547, \
                \"recovered\": 22837, \
                \"critical\": 3898, \
                \"calculated\": { \
                    \"death_rate\": 12.465766860057187, \
                    \"recovery_rate\": 17.229360151493434, \
                    \"recovered_vs_death_ratio\": null, \
                    \"cases_per_million_population\": 2192 \
                } \
            } \
        }, \
        { \
            \"coordinates\": { \
                \"latitude\": 47, \
                \"longitude\": 8 \
            }, \
            \"name\": \"Switzerland\", \
            \"code\": \"CH\", \
            \"population\": 7581000, \
            \"updated_at\": \"2020-04-06T22:57:01.350Z\", \
            \"today\": { \
                \"deaths\": 50, \
                \"confirmed\": 557 \
            }, \
            \"latest_data\": { \
                \"deaths\": 765, \
                \"confirmed\": 21657, \
                \"recovered\": 8056, \
                \"critical\": 391, \
                \"calculated\": { \
                    \"death_rate\": 3.532345200166228, \
                    \"recovery_rate\": 37.198134552338736, \
                    \"recovered_vs_death_ratio\": null, \
                    \"cases_per_million_population\": 2502 \
                } \
            } \
        } \
    ], \
    \"_cacheHit\": true \
}";

TEST_CASE("The corona-api country parser")
{

  auto json_overview_object = coronan::api_parser::parse_countries(test_country_json);
  SUBCASE("returns the country data")
  {
    REQUIRE(json_overview_object.countries[0].name == "Austria");
    REQUIRE(json_overview_object.countries[0].code == "AT");
    REQUIRE(json_overview_object.countries[1].name == "Italy");
    REQUIRE(json_overview_object.countries[1].code == "IT");
    REQUIRE(json_overview_object.countries[2].name == "Switzerland");
    REQUIRE(json_overview_object.countries[2].code == "CH");
  }
}

TEST_SUITE_END();

} // namespace