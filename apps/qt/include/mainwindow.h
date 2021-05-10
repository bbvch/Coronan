#pragma once
#include "coronan/corona-api_parser.hpp"
#include "country_overview_table.hpp"

#include <QtCharts/QChartGlobal>
#include <QtWidgets/QWidget>
#include <string>

QT_BEGIN_NAMESPACE
class Ui_CoronanWidgetForm;
QT_END_NAMESPACE

namespace coronan_gui {
class CountryChartView;

QT_CHARTS_USE_NAMESPACE

class CoronanWidget : public QWidget
{
  Q_OBJECT
public:
  explicit CoronanWidget(QWidget* parent = nullptr);
  virtual ~CoronanWidget();

private Q_SLOTS:
  void update_ui();

private:
  coronan::CountryData get_country_data(std::string_view country_code);
  void populate_country_box();

  CountryChartView* chartView = nullptr;
  Ui_CoronanWidgetForm* ui = nullptr;

  CountryOverviewTable overview_table{};
};

} // namespace coronan_gui
