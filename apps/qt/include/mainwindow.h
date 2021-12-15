#pragma once
#include "coronan/corona-api_datatypes.hpp"
#include "country_data_model.hpp"
#include "country_overview_table_model.hpp"

#include <QTableView>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QWidget>
#include <string>

QT_BEGIN_NAMESPACE
class Ui_CoronanWidgetForm;
QT_END_NAMESPACE

namespace coronan_ui {
class CountryChartView;
class CountryOverviewTablewModel;

QT_CHARTS_USE_NAMESPACE

class CoronanWidget : public QWidget
{
  Q_OBJECT
public:
  explicit CoronanWidget(QWidget* parent = nullptr);
  virtual ~CoronanWidget();
  CoronanWidget(CoronanWidget const&) = delete;
  CoronanWidget& operator=(CoronanWidget const&) = delete;
  CoronanWidget(CoronanWidget&&) = delete;
  CoronanWidget& operator=(CoronanWidget&&) = delete;

private Q_SLOTS:
  void update_ui();

private:
  coronan::CountryData get_country_data(std::string_view country_code);
  void populate_country_box();

  CountryChartView* chartView = nullptr;
  Ui_CoronanWidgetForm* ui = nullptr;

  CountryOverviewTablewModel overview_model{};
  CountryDataModel country_data_model{};
};

} // namespace coronan_ui
