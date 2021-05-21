#include "country_overview_table_model.hpp"
#include <variant>
#include <array>


namespace {

template <class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>; // not needed as of C++20

auto const row_count = 7u;
auto const column_count = 2u;
} // namespace

namespace coronan_ui {

CountryOverviewTablewModel::CountryOverviewTablewModel(QObject *)
{

}

void CountryOverviewTablewModel::populateData(coronan::CountryData const& country_data)
{

    using VariantT = std::variant<std::optional<uint32_t>, std::optional<double>>;
    using CaptionValuePair = std::pair<QString, VariantT>;
    std::array<CaptionValuePair, row_count> const overview_table_entries = {
        {std::make_pair("Population:", country_data.info.population),
         std::make_pair("Confirmed:", country_data.latest.confirmed),
         std::make_pair("Death:", country_data.latest.deaths),
         std::make_pair("Recovered:", country_data.latest.recovered),
         std::make_pair("Critical:", country_data.latest.critical),
         std::make_pair("Death rate:", country_data.latest.death_rate),
         std::make_pair("Recovery rate:", country_data.latest.recovery_rate)}};
    beginResetModel();
    country_overview_data.clear();
    for (auto const& pair : overview_table_entries)
    {
      auto const label = QString{pair.first};
      auto const value = std::visit(
          overloaded{[](auto const& arg) { return arg.has_value() ? QString::number(arg.value()) : QString{"--"}; }},
          pair.second);

      country_overview_data.push_back(qMakePair(label, value));
    }
    endResetModel();
}

int CountryOverviewTablewModel::rowCount(const QModelIndex &) const 
{
    return row_count;
}

int CountryOverviewTablewModel::columnCount(const QModelIndex &) const
{
    return column_count;
}

QVariant CountryOverviewTablewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if (index.column() == 0) {
        return country_overview_data.at(index.row()).first;
    } else if (index.column() == 1) {
        return country_overview_data.at(index.row()).second;
    }
    return QVariant();
}



} // namespace coronan_ui
