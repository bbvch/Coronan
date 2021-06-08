#include "country_data_model.hpp"

namespace {
constexpr auto columns = 5;
}

namespace coronan_ui {

CountryDataModel::CountryDataModel(QObject*)
{
}

void CountryDataModel::populateData(coronan::CountryData const& country_data)
{
  beginResetModel();
  country_name = QString::fromStdString(country_data.info.name);
  confirmed_cases = country_data.latest.confirmed.value_or(0);
  country_timeline_data.clear();

  for (auto const& data_point : country_data.timeline)
  {
    CountryTimelineData timeline_data;
    timeline_data.date = QDateTime::fromString(data_point.date.c_str(), QStringLiteral("yyyy-MM-ddThh:mm:ss.zZ"));
    timeline_data.deaths = data_point.deaths.has_value() ? QVariant{data_point.deaths.value()} : QVariant{};
    timeline_data.confirmed_cases =
        data_point.confirmed.has_value() ? QVariant{data_point.confirmed.value()} : QVariant{};
    timeline_data.active_cases = data_point.active.has_value() ? QVariant{data_point.active.value()} : QVariant{};
    timeline_data.recovered_cases =
        data_point.recovered.has_value() ? QVariant{data_point.recovered.value()} : QVariant{};
    country_timeline_data.push_back(timeline_data);
  }
  endResetModel();
}

int CountryDataModel::rowCount(const QModelIndex&) const
{
  return country_timeline_data.length();
}

int CountryDataModel::columnCount(const QModelIndex&) const
{
  return columns;
}

QVariant CountryDataModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || role != Qt::DisplayRole)
  {
    return QVariant();
  }
  if (index.column() == 0)
  {
    return country_timeline_data.at(index.row()).date;
  }
  else if (index.column() == 1)
  {
    return country_timeline_data.at(index.row()).deaths;
  }
  else if (index.column() == 2)
  {
    return country_timeline_data.at(index.row()).confirmed_cases;
  }
  else if (index.column() == 3)
  {
    return country_timeline_data.at(index.row()).active_cases;
  }
  else if (index.column() == 4)
  {
    return country_timeline_data.at(index.row()).recovered_cases;
  }
  return QVariant();
}

QVariant CountryDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
  {
    if (section == 0)
    {
      return QStringLiteral("Date");
    }
    else if (section == 1)
    {
      return QStringLiteral("Death");
    }
    else if (section == 2)
    {
      return QStringLiteral("Confirmed");
    }
    else if (section == 3)
    {
      return QStringLiteral("Active");
    }
    else if (section == 4)
    {
      return QStringLiteral("Recovered");
    }
  }
  return QVariant();
}

QString CountryDataModel::countryName() const
{
  return country_name;
}

qreal CountryDataModel::casesConfirmed() const
{
  return confirmed_cases;
}

} // namespace coronan_ui
