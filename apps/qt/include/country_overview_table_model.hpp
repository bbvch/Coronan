#pragma once

#include "coronan/corona-api_datatypes.hpp"

#include <QAbstractTableModel>
#include <QList>
#include <QString>
#include <QPair>

namespace coronan_ui {

class CountryOverviewTablewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CountryOverviewTablewModel(QObject *parent = 0);

    void populateData(coronan::CountryData const& country_data);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<QPair<QString, QString>> country_overview_data;
};

} // namespace coronan_ui
