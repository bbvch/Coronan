#include "mainwindow.h"

#include "coronan/corona-api_client.hpp"
#include "country_chart_view.hpp"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>
#include <algorithm>

namespace coronan_ui {

CoronanWidget::CoronanWidget(QWidget* parent) : QWidget{parent}, ui{new Ui_CoronanWidgetForm}
{
  ui->setupUi(this);

  ui->overviewTable->horizontalHeader()->setVisible(false);
  ui->overviewTable->setModel(&overview_model);
  
  populate_country_box();
  update_ui();

  QObject::connect(ui->countryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update_ui()));
}

CoronanWidget::~CoronanWidget()
{
  delete ui;
}

void CoronanWidget::populate_country_box()
{
  auto* country_combo = ui->countryComboBox;
  auto countries = coronan::CoronaAPIClient{}.request_countries();

  std::sort(begin(countries), end(countries), [](auto const& a, auto const& b) { return a.name < b.name; });

  for (auto const& country : countries)
  {
    country_combo->addItem(country.name.c_str(), country.iso_code.c_str());
  }
  if (int const index = country_combo->findData("CH"); index != -1)
  { // -1 for not found
    country_combo->setCurrentIndex(index);
  }
}

coronan::CountryData CoronanWidget::get_country_data(std::string_view country_code)
{
  try
  {
    return coronan::CoronaAPIClient{}.request_country_data(country_code);
  }
  catch (coronan::SSLException const& ex)
  {
    qWarning() << ex.what();
    QMessageBox::warning(this, "SSL Exception", QString{ex.what()});
  }
  catch (coronan::HTTPClientException const& ex)
  {
    qWarning() << ex.what();
    QMessageBox::warning(this, "HTTP Client Exception", QString{ex.what()});
  }
  catch (std::exception const& ex)
  {
    qWarning() << ex.what();
    QMessageBox::warning(this, "Exception", QString{ex.what()});
  }
  return {};
}

void CoronanWidget::update_ui()
{
  auto country_code = ui->countryComboBox->itemData(ui->countryComboBox->currentIndex()).toString();
  auto const country_data = get_country_data(country_code.toStdString());
  overview_model.populateData(country_data);
  country_data_model.populateData(country_data);
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
