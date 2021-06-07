#pragma once

#include "coronan/corona-api_datatypes.hpp"

#include <QAbstractTableModel>
#include <QVariant>
#include <QDateTime>
#include <QList>


namespace coronan_ui {

class CountryDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CountryDataModel(QObject *parent = 0);

    void populateData(coronan::CountryData const& country_data);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;

    QString countryName() const;

    qreal casesConfirmed() const;


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