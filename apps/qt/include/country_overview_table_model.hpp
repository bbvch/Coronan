#pragma once

#include "coronan/corona-api_datatypes.hpp"

#include <QAbstractTableModel>
#include <QList>
#include <QPair>
#include <QString>

namespace coronan_ui {

class CountryOverviewTablewModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit CountryOverviewTablewModel(QObject* parent = nullptr);
  void populate_data(coronan::CountryData const& country_data);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
  QList<QPair<QString, QString>> country_overview_data;
};

} // namespace coronan_ui
