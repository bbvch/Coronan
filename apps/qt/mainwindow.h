#pragma once

#include <QtCharts/QChartGlobal>
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

private:
  QChart* createLineChart() const;

private:
  QList<QChartView*> m_charts;
  Ui_CoronanWidgetForm* m_ui;
  std::string m_url{};
};
