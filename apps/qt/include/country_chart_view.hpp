#pragma once

#include <QtCharts/QChartView>
#include <QVXYModelMapper>

namespace coronan {
struct CountryData;
}

namespace coronan_ui {
class CountryDataModel;

QT_CHARTS_USE_NAMESPACE

class CountryChartView : public QChartView
{
  Q_OBJECT
public:
  explicit CountryChartView(CountryDataModel * const data_model, QWidget* parent = nullptr);

  virtual ~CountryChartView() = default;
  void update_ui(CountryDataModel const & data_model);
private:
  QVXYModelMapper death_series_model_mapper{};
  QVXYModelMapper confirmed_series_model_mapper{};
  QVXYModelMapper active_series_model_mapper{};
  QVXYModelMapper recovered_series_model_mapper{};

};

} // namespace coronan_ui
