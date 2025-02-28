#include "country_overview_table_model.hpp"

#include <array>
#include <variant>

namespace {

template <class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};

inline auto constexpr row_count = 6u;
inline auto constexpr column_count = 2u;
} // namespace

namespace coronan_ui {

CountryOverviewTablewModel::CountryOverviewTablewModel(QObject* parent) : QAbstractTableModel(parent)
{
}

void CountryOverviewTablewModel::populate_data(coronan::CountryData const& country_data)
{

  using VariantT = std::variant<std::string, std::optional<uint32_t>, std::optional<double>>;
  using CaptionValuePair = std::pair<QString, VariantT>;
  std::array<CaptionValuePair, row_count> const overview_table_entries = {
      {std::make_pair("Date:", std::format("{:%Y-%m-%d}", country_data.latest.date)),
       std::make_pair("Confirmed:", country_data.latest.confirmed),
       std::make_pair("Death:", country_data.latest.deaths),
       std::make_pair("Recovered:", country_data.latest.recovered),
       std::make_pair("Active:", country_data.latest.active),
       std::make_pair("Death rate:", country_data.latest.fatality_rate)}};
  beginResetModel();
  country_overview_data.clear();
  for (auto const& pair : overview_table_entries)
  {
    auto const label = QString{pair.first};
    auto const value = std::visit(
        overloaded{[](std::string const& arg) { return QString{arg.c_str()}; },
                   [](auto const& arg) { return arg.has_value() ? QString::number(arg.value()) : QString{"--"}; }},
        pair.second);

    country_overview_data.push_back(qMakePair(label, value));
  }
  endResetModel();
}

int CountryOverviewTablewModel::rowCount(QModelIndex const&) const
{
  return row_count;
}

int CountryOverviewTablewModel::columnCount(QModelIndex const&) const
{
  return column_count;
}

QVariant CountryOverviewTablewModel::data(QModelIndex const& index, int role) const
{
  if (!index.isValid() || role != Qt::DisplayRole)
  {
    return QVariant();
  }
  if (index.column() == 0)
  {
    return country_overview_data.at(index.row()).first;
  }
  else if (index.column() == 1)
  {
    return country_overview_data.at(index.row()).second;
  }
  return QVariant();
}

} // namespace coronan_ui
