#pragma once

#include "coronan/corona-api_client.hpp"

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

using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;
using DataTable = QList<DataList>;

QT_CHARTS_USE_NAMESPACE

class CoronanWidget : public QWidget
{
  Q_OBJECT
public:
  explicit CoronanWidget(std::string&& api_url, QWidget* parent = nullptr);
  virtual ~CoronanWidget();

private Q_SLOTS:
  void update_ui();

private:
  coronan::CountryObject get_country_data(std::string const& country_code);
  void populate_country_box();

  QChartView* m_chartView = nullptr;
  Ui_CoronanWidgetForm* m_ui = nullptr;
  coronan::CoronaAPIClient<> m_api_client;
};
