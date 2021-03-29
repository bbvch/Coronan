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
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <algorithm>
#include <array>
#include <string>
#include <variant>

namespace {

template <class... Ts> struct overloaded : Ts...
{
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>; // not needed as of C++20

constexpr auto update_country_overview_table = [](auto* table,
                                                  auto const& country_data) {
  auto const label_col_index = 0;
  auto const value_col_index = 1;
  auto row_index = 0;
  constexpr auto no_table_entries = 7;
  std::array<
      std::pair<char const*, std::variant<uint32_t, std::optional<double>>>,
      no_table_entries> const overview_table_entries = {
      {std::make_pair("Population:", country_data.population),
       std::make_pair("Confirmed:", country_data.latest.confirmed),
       std::make_pair("Death:", country_data.latest.deaths),
       std::make_pair("Recovered:", country_data.latest.recovered),
       std::make_pair("Critical:", country_data.latest.critical),
       std::make_pair("Death rate:", country_data.latest.death_rate),
       std::make_pair("Recovery rate::", country_data.latest.recovery_rate)}};

  table->setRowCount(no_table_entries);
  for (auto const& pair : overview_table_entries)
  {
    auto* const label_widget = new QTableWidgetItem{pair.first};
    table->setItem(row_index, label_col_index, label_widget);
    auto const value_str =
        std::visit(overloaded{
                       [](uint32_t arg) { return QString::number(arg); },
                       [](std::optional<double> arg) {
                         return arg.has_value() ? QString::number(arg.value())
                                                : QString{"--"};
                       },
                   },
                   pair.second);

    auto* const value_widget = new QTableWidgetItem{value_str};
    table->setItem(row_index, value_col_index, value_widget);

    row_index++;
  }
};

constexpr auto create_line_chart =
    [](coronan::CountryObject const& country_data) {
      auto* const chart = new QChart{};

      chart->setTitle(QString{"Corona (Covid-19) Cases in "}.append(
          country_data.name.c_str()));

      auto const confirmed_serie_name = std::string{"Confirmed"};
      auto const death_serie_name = std::string{"Death"};
      auto const recovered_serie_name = std::string{"Recovered"};

      auto* const death_serie = new QLineSeries{};
      death_serie->setName(death_serie_name.c_str());
      auto* const confirmed_serie = new QLineSeries{};
      confirmed_serie->setName(confirmed_serie_name.c_str());
      auto* const active_serie = new QLineSeries{};
      active_serie->setName("Active");
      auto* const recovered_serie = new QLineSeries{};
      recovered_serie->setName(recovered_serie_name.c_str());

      std::array<QLineSeries*, 4> series = {
          {death_serie, confirmed_serie, active_serie, recovered_serie}};

      for (auto const& data_point : country_data.timeline)
      {
        QDateTime date = QDateTime::fromString(data_point.date.c_str(),
                                               "yyyy-MM-ddThh:mm:ss.zZ");
        auto const msecs_since_epoche =
            static_cast<double>(date.toMSecsSinceEpoch());
        death_serie->append(QPointF(msecs_since_epoche, data_point.deaths));
        confirmed_serie->append(
            QPointF(msecs_since_epoche, data_point.confirmed));
        active_serie->append(QPointF(msecs_since_epoche, data_point.active));
        recovered_serie->append(
            QPointF(msecs_since_epoche, data_point.recovered));
      }

      for (auto* serie : series)
      {
        chart->addSeries(serie);
      }

      auto* const axisX = new QDateTimeAxis{};
      axisX->setFormat("dd/MM  ");
      axisX->setTitleText("Date");
      chart->addAxis(axisX, Qt::AlignBottom);

      auto* const axisY = new QValueAxis{};
      axisY->setTitleText("Cases");
      axisY->setLabelFormat("%i  ");

      auto const max_cases = country_data.latest.confirmed;
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
    };

constexpr auto create_chart_view = [](auto const& country_data) {
  auto* const chartView = new QChartView{create_line_chart(country_data)};
  chartView->setRenderHint(QPainter::Antialiasing, true);
  return chartView;
};

} // namespace

CoronanWidget::CoronanWidget(std::string&& api_url, QWidget* parent)
    : QWidget{parent}, m_ui{new Ui_CoronanWidgetForm}, m_url{std::move(api_url)}
{
  m_ui->setupUi(this);

  populate_country_box();

  auto country_code =
      m_ui->countryComboBox->itemData(m_ui->countryComboBox->currentIndex())
          .toString();
  auto const country_data = get_country_data(country_code.toStdString());

  m_chartView = create_chart_view(country_data);
  m_ui->gridLayout->addWidget(m_chartView, 2, 1);

  m_ui->overviewTable->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::ResizeToContents);

  update_country_overview_table(m_ui->overviewTable, country_data);
}

CoronanWidget::~CoronanWidget() { delete m_ui; }

void CoronanWidget::populate_country_box()
{
  auto const http_response = coronan::HTTPClient::get(m_url);
  auto const json_object =
      coronan::api_parser::parse_countries(http_response.get_response_body());

  auto* countryComboBox = m_ui->countryComboBox;
  auto countries = json_object.countries;

  std::sort(std::begin(countries), std::end(countries),
            [](auto const& a, auto const& b) { return a.name < b.name; });

  for (auto const& country : countries)
  {
    countryComboBox->addItem(country.name.c_str(), country.code.c_str());
  }

  int index = countryComboBox->findData("CH");
  if (index != -1)
  { // -1 for not found
    countryComboBox->setCurrentIndex(index);
  }
}

coronan::CountryObject
CoronanWidget::get_country_data(std::string const& country_code) const
{
  auto const http_response =
      coronan::HTTPClient::get(m_url + std::string{"/"} + country_code);
  return coronan::api_parser::parse(http_response.get_response_body());
}

void CoronanWidget::update_ui()
{
  auto country_code =
      m_ui->countryComboBox->itemData(m_ui->countryComboBox->currentIndex())
          .toString();
  auto const country_data = get_country_data(country_code.toStdString());
  auto* const new_chartView = create_chart_view(country_data);
  auto* old_layout =
      m_ui->gridLayout->replaceWidget(m_chartView, new_chartView);
  delete old_layout;
  m_chartView = new_chartView;
  update_country_overview_table(m_ui->overviewTable, country_data);
}
