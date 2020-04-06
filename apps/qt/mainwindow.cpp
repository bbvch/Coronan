#include "mainwindow.h"

#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QLabel>
#include <QString>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtGlobal>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <array>
#include <string>

CoronanWidget::CoronanWidget(std::string const& api_url, QWidget* parent)
    : QWidget{parent}, m_ui{new Ui_CoronanWidgetForm}, m_url{api_url}
{
  m_ui->setupUi(this);

  // create chart
  auto* chartView = new QChartView{createLineChart()};
  chartView->setRenderHint(QPainter::Antialiasing, true);
  m_ui->gridLayout->addWidget(chartView, 1, 2);
  m_charts << chartView;

  // Set the colors from the light theme as default ones
  auto pal = qApp->palette();
  pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
  pal.setColor(QPalette::WindowText, QRgb(0x404044));
  qApp->setPalette(pal);
}

CoronanWidget::~CoronanWidget() { delete m_ui; }

QChart* CoronanWidget::createLineChart() const
{
  auto const http_response = coronan::HTTPClient::get(m_url);
  auto const json_object =
      coronan::api_parser::parse(http_response.get_response_body());

  auto* chart = new QChart{};
  chart->setTitle(QString{"Corona (Covid-19) Cases in "}.append(
      json_object.country_name.c_str()));

  auto const confirmed_serie_name =
      std::string{"Confirmed ("} +
      std::to_string(json_object.latest.confirmed) + std::string{")"};
  auto const death_serie_name = std::string{"Death ("} +
                                std::to_string(json_object.latest.deaths) +
                                std::string{")"};
  auto const recovered_serie_name =
      std::string{"Recovered ("} +
      std::to_string(json_object.latest.recovered) + std::string{")"};

  auto* death_serie = new QLineSeries{};
  death_serie->setName(death_serie_name.c_str());
  auto* confirmed_serie = new QLineSeries{};
  confirmed_serie->setName(confirmed_serie_name.c_str());
  auto* active_serie = new QLineSeries{};
  active_serie->setName("Active");
  auto* recovered_serie = new QLineSeries{};
  recovered_serie->setName(recovered_serie_name.c_str());

  std::array<QLineSeries*, 4> series = {
      {death_serie, confirmed_serie, active_serie, recovered_serie}};

  auto const max_cases = json_object.latest.confirmed;

  for (auto const& data_point : json_object.timeline)
  {
    QDateTime date = QDateTime::fromString(data_point.date.c_str(),
                                           "yyyy-MM-ddThh:mm:ss.zZ");
    auto const msecs_since_epoche = date.toMSecsSinceEpoch();
    death_serie->append(QPointF(msecs_since_epoche, data_point.deaths));
    confirmed_serie->append(QPointF(msecs_since_epoche, data_point.confirmed));
    active_serie->append(QPointF(msecs_since_epoche, data_point.active));
    recovered_serie->append(QPointF(msecs_since_epoche, data_point.recovered));
  }

  chart->addSeries(death_serie);
  chart->addSeries(confirmed_serie);
  chart->addSeries(active_serie);
  chart->addSeries(recovered_serie);

  auto* axisX = new QDateTimeAxis;
  axisX->setFormat("dd/MM  ");
  axisX->setTitleText("Date");
  chart->addAxis(axisX, Qt::AlignBottom);

  auto* axisY = new QValueAxis();
  axisY->setTitleText("Cases");
  axisY->setLabelFormat("%i  ");
  axisY->setRange(0, max_cases);
  axisY->setLinePenColor(confirmed_serie->pen().color());
  axisY->setLabelsColor(confirmed_serie->pen().color());
  axisY->setGridLineColor(confirmed_serie->pen().color());
  chart->addAxis(axisY, Qt::AlignLeft);

  for (auto* serie : series)
  {
    serie->attachAxis(axisX);
    serie->attachAxis(axisY);
  }
  chart->setTheme(QChart::ChartThemeDark);
  chart->legend()->setAlignment(Qt::AlignTop);
  chart->legend()->show();

  return chart;
}
