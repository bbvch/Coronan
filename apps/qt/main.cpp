#include "coronan/corona-api_client.hpp"
#include "coronan/http_client.hpp"
#include "coronan/ssl_initializer.hpp"
#include "mainwindow.h"

#include <QDebug>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <iostream>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QMainWindow window;
  try
  {
    window.show();
    window.setWindowTitle("Co[ro]nan");
    auto const window_width = 900;
    auto const window_height = 600;
    window.resize(window_width, window_height);
    auto* const widget = new CoronanWidget();
    window.setCentralWidget(widget);
    return app.exec(); // NOLINT(readability-static-accessed-through-instance)
  }
  catch (coronan::HTTPClientException const& ex)
  {
    qCritical() << ex.what();
    QMessageBox::critical(&window, "Http Exception", QString{ex.what()});
    app.exit(EXIT_FAILURE);
  }
  catch (coronan::SSLException const& ex)
  {
    qCritical() << ex.what();
    QMessageBox::critical(&window, "SSL Exception", QString{ex.what()});
    app.exit(EXIT_FAILURE);
  }
  catch (std::exception const& ex)
  {
    qCritical() << ex.what();
    QMessageBox::critical(&window, "Exception", QString{ex.what()});
    app.exit(EXIT_FAILURE);
  }
}
