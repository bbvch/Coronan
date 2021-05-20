#pragma once

#include <QtCharts/QChartView>

namespace coronan {
struct CountryData;
}

namespace coronan_ui {

QT_CHARTS_USE_NAMESPACE

class CountryChartView : public QChartView
{
  Q_OBJECT
public:
  explicit CountryChartView(coronan::CountryData const& country_data, QWidget* parent = nullptr);

  virtual ~CountryChartView() = default;
};

} // namespace coronan_ui
