#pragma once

#include <QtWidgets/QWidget>

namespace coronan {
struct CountryData;
}

QT_BEGIN_NAMESPACE
class QTableWidget;
QT_END_NAMESPACE

namespace coronan_gui {

struct CountryOverviewTable
{
  void update(QTableWidget* const table_widget,
              coronan::CountryData const& country_data);
};

} // namespace coronan_gui
