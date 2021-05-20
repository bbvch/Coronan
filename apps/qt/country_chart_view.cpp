#include "country_chart_view.hpp"

#include "coronan/corona-api_parser.hpp"

#include <QDateTime>
#include <QString>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

namespace {
constexpr auto create_datetime_axis = []() {
  auto* const x_axis = new QtCharts::QDateTimeAxis{};
  x_axis->setFormat("dd/MM  ");
  x_axis->setTitleText("Date");
  return x_axis;
};

constexpr auto create_value_axis = [](auto range_max) {
  auto* const y_axis = new QtCharts::QValueAxis{};
  y_axis->setTitleText("Cases");
  y_axis->setLabelFormat("%i  ");
  y_axis->setRange(0, range_max);

  return y_axis;
};

constexpr auto append_value_to_series = [](auto msecs_since_epoche, auto const& value, auto* const serie) {
  if (value.has_value())
  {
    serie->append(QPointF(msecs_since_epoche, value.value()));
  }
};
} // namespace

namespace coronan_ui {

CountryChartView::CountryChartView(coronan::CountryData const& country_data, QWidget* parent) : QChartView{parent}
{
  auto* const chart = new QChart{};

  chart->setTitle(QString{"Corona (Covid-19) Cases in "}.append(country_data.info.name.c_str()));

  auto* const death_series = new QLineSeries{};
  death_series->setName("Death");
  auto* const confirmed_series = new QLineSeries{};
  confirmed_series->setName("Confirmed");
  auto* const active_series = new QLineSeries{};
  active_series->setName("Active");
  auto* const recovered_series = new QLineSeries{};
  recovered_series->setName("Recovered");

  for (auto const& data_point : country_data.timeline)
  {
    auto const msecs_since_epoche = static_cast<double>(
        QDateTime::fromString(data_point.date.c_str(), "yyyy-MM-ddThh:mm:ss.zZ").toMSecsSinceEpoch());

    append_value_to_series(msecs_since_epoche, data_point.deaths, death_series);
    append_value_to_series(msecs_since_epoche, data_point.confirmed, confirmed_series);
    append_value_to_series(msecs_since_epoche, data_point.active, active_series);
    append_value_to_series(msecs_since_epoche, data_point.recovered, recovered_series);
  }

  auto* const x_axis = create_datetime_axis();
  auto* const y_axis = create_value_axis(country_data.latest.confirmed.value_or(0));
  chart->addAxis(x_axis, Qt::AlignBottom);
  chart->addAxis(y_axis, Qt::AlignLeft);

  for (auto* const series : std::vector<QLineSeries*>{death_series, confirmed_series, active_series, recovered_series})
  {
    chart->addSeries(series);
    series->attachAxis(x_axis);
    series->attachAxis(y_axis);
  }

  chart->setTheme(QChart::ChartThemeDark);
  chart->legend()->setAlignment(Qt::AlignTop);
  chart->legend()->show();

  this->setChart(chart);
  this->setRenderHint(QPainter::Antialiasing, true);
}

} // namespace coronan_ui
