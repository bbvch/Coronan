#pragma once

#include "coronan/corona-api_datatypes.hpp"

#include <QAbstractTableModel>
#include <QDateTime>
#include <QList>
#include <QVariant>
#include <cstdint>
#include <qglobal.h>

namespace coronan_ui {

class CountryDataModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit CountryDataModel(QObject* parent = nullptr);
  ~CountryDataModel() override = default;

  void populate_data(coronan::CountryData const& country_data);

  int rowCount(QModelIndex const& parent = QModelIndex()) const override;
  int columnCount(QModelIndex const& parent = QModelIndex()) const override;

  QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal,
                      int role = Qt::DisplayRole) const override;

  QString country() const;

  uint32_t max_value() const
  {
    return max;
  }

  uint32_t min_value() const
  {
    return min;
  }

  QDateTime max_date() const
  {
    return end_date;
  }

  QDateTime min_date() const
  {
    return start_date;
  }

  static auto constexpr date_column_index = 0;
  static auto constexpr deaths_column_index = 1;
  static auto constexpr confirmed_column_index = 2;
  static auto constexpr active_column_index = 3;
  static auto constexpr recovered_column_index = 4;

private:
  QString country_name{};
  uint32_t max{};
  uint32_t min{};
  QDateTime start_date{QDateTime::currentDateTime()};
  QDateTime end_date{QDateTime::currentDateTime()};
  struct CountryTimelineData
  {
    QDateTime date;
    QVariant deaths;
    QVariant confirmed_cases;
    QVariant active_cases;
    QVariant recovered_cases;
  };

  QList<CountryTimelineData> country_timeline_data;
};

} // namespace coronan_ui
