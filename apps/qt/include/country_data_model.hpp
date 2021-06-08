#pragma once

#include "coronan/corona-api_datatypes.hpp"

#include <QAbstractTableModel>
#include <QDateTime>
#include <QList>
#include <QVariant>

namespace coronan_ui {

class CountryDataModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit CountryDataModel(QObject* parent = nullptr);

  void populate_data(coronan::CountryData const& country_data);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal,
                      int role = Qt::DisplayRole) const override;

  QString country() const;

  qreal cases_confirmed() const;

  static constexpr auto date_column_index = 0;
  static constexpr auto deaths_column_index = 1;
  static constexpr auto confirmed_column_index = 2;
  static constexpr auto active_column_index = 3;
  static constexpr auto recovered_column_index = 4;

private:
  QString country_name{};
  qreal confirmed_cases{};
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
