#include "country_overview_table.hpp"

#include "coronan/corona-api_parser.hpp"

#include <QtWidgets/QTableWidget>
#include <array>
#include <variant>

namespace {

template <class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>; // not needed as of C++20

} // namespace

namespace coronan_ui {

void CountryOverviewTable::update(QTableWidget* const table_widget, coronan::CountryData const& country_data)
{
  if (table_widget != nullptr)
  {
    auto const label_col_index = 0;
    auto const value_col_index = 1;

    using VariantT = std::variant<std::optional<uint32_t>, std::optional<double>>;
    using CaptionValuePair = std::pair<QString, VariantT>;
    constexpr auto no_table_entries = 7;
    std::array<CaptionValuePair, no_table_entries> const overview_table_entries = {
        {std::make_pair("Population:", country_data.info.population),
         std::make_pair("Confirmed:", country_data.latest.confirmed),
         std::make_pair("Death:", country_data.latest.deaths),
         std::make_pair("Recovered:", country_data.latest.recovered),
         std::make_pair("Critical:", country_data.latest.critical),
         std::make_pair("Death rate:", country_data.latest.death_rate),
         std::make_pair("Recovery rate::", country_data.latest.recovery_rate)}};

    table_widget->setRowCount(no_table_entries);

    auto row_index = 0;
    for (auto const& pair : overview_table_entries)
    {
      auto* const label_widget = new QTableWidgetItem{pair.first};
      table_widget->setItem(row_index, label_col_index, label_widget);
      // Clean Code Note: variant with visit functions allows to handle the
      // empty (optional) type without if statements
      auto const value_str = std::visit(
          overloaded{[](auto const& arg) { return arg.has_value() ? QString::number(arg.value()) : QString{"--"}; }},
          pair.second);

      auto* const value_widget = new QTableWidgetItem{value_str};
      table_widget->setItem(row_index, value_col_index, value_widget);

      row_index++;
    }
  }
}

} // namespace coronan_ui
