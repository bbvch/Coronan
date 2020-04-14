#pragma once

#include "coronan/corona-api_parser.hpp"

#include <QtCharts/QChartGlobal>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include <string>

QT_BEGIN_NAMESPACE
class Ui_CoronanWidgetForm;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

class CoronanWidget : public QWidget
{
  Q_OBJECT
public:
  explicit CoronanWidget(std::string const& api_url, QWidget* parent = 0);
  ~CoronanWidget();

private Q_SLOTS:
  void update_ui();

private:
  coronan::CountryObject
  get_country_data(std::string const& country_code) const;
  void populate_country_box();

private:
  QChartView* m_chartView = nullptr;
  Ui_CoronanWidgetForm* m_ui = nullptr;
  std::string m_url{};
};
