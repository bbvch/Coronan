#include "country_chart_view.hpp"

#include "coronan/corona-api_parser.hpp"
#include "country_data_model.hpp"

#include <QDateTime>
#include <QLatin1String>
#include <QString>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

namespace {
constexpr auto create_datetime_axis = []() {
  auto* const x_axis = new QtCharts::QDateTimeAxis{};
  x_axis->setFormat(QStringLiteral("dd/MM  "));
  x_axis->setTitleText(QStringLiteral("Date"));
  return x_axis;
};

constexpr auto create_value_axis = []() {
  auto* const y_axis = new QtCharts::QValueAxis{};
  y_axis->setTitleText(QStringLiteral("Cases"));
  y_axis->setLabelFormat(QStringLiteral("%i  "));
  return y_axis;
};

} // namespace

namespace coronan_ui {

CountryChartView::CountryChartView(CountryDataModel* const data_model, QWidget* parent) : QChartView{parent}
{
  auto* const chart = new QChart{};
  chart->setTitle(QStringLiteral("Corona (Covid-19) Cases in ").append(data_model->countryName()));
  auto* const x_axis = create_datetime_axis();
  auto* const y_axis = create_value_axis();
  y_axis->setRange(0, data_model->casesConfirmed());
  chart->addAxis(x_axis, Qt::AlignBottom);
  chart->addAxis(y_axis, Qt::AlignLeft);

  auto* const death_series = new QLineSeries{};
  death_series->setName(data_model->headerData(1).toString());
  death_series_model_mapper.setXColumn(0);
  death_series_model_mapper.setYColumn(1);
  death_series_model_mapper.setSeries(death_series);
  death_series_model_mapper.setModel(data_model);
  chart->addSeries(death_series);
  death_series->attachAxis(x_axis);
  death_series->attachAxis(y_axis);

  auto* const confirmed_series = new QLineSeries{};
  confirmed_series->setName(data_model->headerData(2).toString());
  confirmed_series_model_mapper.setXColumn(0);
  confirmed_series_model_mapper.setYColumn(2);
  confirmed_series_model_mapper.setSeries(confirmed_series);
  confirmed_series_model_mapper.setModel(data_model);
  chart->addSeries(confirmed_series);
  confirmed_series->attachAxis(x_axis);
  confirmed_series->attachAxis(y_axis);

  auto* const active_series = new QLineSeries{};
  active_series->setName(data_model->headerData(3).toString());
  active_series_model_mapper.setXColumn(0);
  active_series_model_mapper.setYColumn(3);
  active_series_model_mapper.setSeries(active_series);
  active_series_model_mapper.setModel(data_model);
  chart->addSeries(active_series);
  active_series->attachAxis(x_axis);
  active_series->attachAxis(y_axis);

  auto* const recovered_series = new QLineSeries{};
  recovered_series->setName(data_model->headerData(4).toString());
  recovered_series_model_mapper.setXColumn(0);
  recovered_series_model_mapper.setYColumn(4);
  recovered_series_model_mapper.setSeries(recovered_series);
  recovered_series_model_mapper.setModel(data_model);
  chart->addSeries(recovered_series);
  recovered_series->attachAxis(x_axis);
  recovered_series->attachAxis(y_axis);

  chart->setTheme(QChart::ChartThemeDark);
  chart->legend()->setAlignment(Qt::AlignTop);
  chart->legend()->show();

  this->setChart(chart);
  this->setRenderHint(QPainter::Antialiasing, true);
}

void CountryChartView::update_ui(CountryDataModel const& data_model)
{
  this->chart()->setTitle(QStringLiteral("Corona (Covid-19) Cases in ").append(data_model.countryName()));
  this->chart()->axes(Qt::Vertical).at(0)->setMax(data_model.casesConfirmed());
}

} // namespace coronan_ui
