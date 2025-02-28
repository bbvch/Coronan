#include "mainwindow.h"

#include "coronan/corona-api_client.hpp"
#include "country_chart_view.hpp"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>
#include <algorithm>
#include <chrono>
#include <optional>

namespace coronan_ui {

inline constexpr auto default_country_code = "CHE";

CoronanWidget::CoronanWidget(QWidget* parent) : QWidget(parent), ui{new Ui_CoronanWidgetForm}
{
  ui->setupUi(this);

  ui->overviewTable->horizontalHeader()->setVisible(false);
  ui->overviewTable->setModel(&overview_model);

  populate_country_box();
  populate_date_boxes();

  update_ui();

  QObject::connect(ui->updateButton, qOverload<>(&QPushButton::pressed), [this]() { this->update_ui(); });
}

CoronanWidget::~CoronanWidget()
{
  delete ui;
}

void CoronanWidget::populate_country_box()
{
  auto* country_combo = ui->countryComboBox;
  auto countries = coronan::CoronaAPIClient{}.request_regions();

  std::sort(begin(countries), end(countries), [](auto const& a, auto const& b) { return a.name < b.name; });

  std::for_each(cbegin(countries), cend(countries),
                [=](auto const& country) { country_combo->addItem(country.name.c_str(), country.iso_code.c_str()); });

  if (int const index = country_combo->findData(default_country_code); index != -1)
  { // -1 for not found
    country_combo->setCurrentIndex(index);
  }
}

void CoronanWidget::populate_date_boxes()
{
  auto const latest_country_data = coronan::CoronaAPIClient{}.request_country_data(default_country_code, std::nullopt);
  auto const latest_date = latest_country_data.latest.date;
  auto const start_date = latest_date - std::chrono::months{1};
  ui->startDate->setDate(QDate{start_date});
  ui->startDate->setMaximumDate(QDate{latest_date});
  ui->endDate->setDate(QDate{latest_date});
  ui->endDate->setMaximumDate(QDate{latest_date});
}

coronan::CountryData CoronanWidget::get_country_data(std::string_view country_code,
                                                     std::chrono::year_month_day const& start_date,
                                                     std::chrono::year_month_day const& end_date)
{
  try
  {
    return coronan::CoronaAPIClient{}.request_country_data(country_code, start_date, end_date);
  }
  catch (coronan::SSLException const& ex)
  {
    qWarning() << ex.what();
    QMessageBox::warning(this, QStringLiteral("SSL Exception"), QString{ex.what()});
  }
  catch (coronan::HTTPClientException const& ex)
  {
    qWarning() << ex.what();
    QMessageBox::warning(this, QStringLiteral("HTTP Client Exception"), QString{ex.what()});
  }
  catch (std::exception const& ex)
  {
    qWarning() << ex.what();
    QMessageBox::warning(this, QStringLiteral("Exception"), QString{ex.what()});
  }
  return {};
}

void CoronanWidget::update_ui()
{
  auto const country_code = ui->countryComboBox->itemData(ui->countryComboBox->currentIndex()).toString();
  auto const country_name = ui->countryComboBox->itemText(ui->countryComboBox->currentIndex());
  std::chrono::year_month_day const start_date{ui->startDate->date().toStdSysDays()};
  std::chrono::year_month_day const end_date{ui->endDate->date().toStdSysDays()};
  auto const latest_country_data = coronan::CoronaAPIClient{}.request_country_data(default_country_code, std::nullopt);
  overview_model.populate_data(latest_country_data);
  auto country_data = get_country_data(country_code.toStdString(), start_date, end_date);
  country_data.latest = latest_country_data.latest;
  country_data.info.name = country_name.toStdString();
  country_data_model.populate_data(country_data);
  ui->overviewTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

  if (chartView == nullptr)
  {
    chartView = new coronan_ui::CountryChartView{&country_data_model};
    ui->gridLayout->addWidget(chartView, 2, 1);
  }
  else
  {
    chartView->update_ui(country_data_model);
  }
}

} // namespace coronan_ui
