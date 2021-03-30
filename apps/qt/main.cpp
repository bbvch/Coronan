#include "coronan/ssl_initializer.hpp"
#include "mainwindow.h"

#include <QDebug>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <iostream>

constexpr auto api_url = "https://corona-api.com/countries";

int main(int argc, char* argv[])
{
  static auto const ssl_initializer_handler =
      coronan::SSLInitializer::initialize_with_accept_certificate_handler();
  try
  {

    QApplication app(argc, argv);
    QMainWindow window;
    auto* const widget = new CoronanWidget(api_url);
    window.setCentralWidget(widget);
    auto const window_width = 900;
    auto const window_height = 600;
    window.resize(window_width, window_height);
    window.show();
    return app.exec(); // NOLINT(readability-static-accessed-through-instance)
  }
  catch (coronan::SSLException const& ex)
  {
    qCritical() << "SSL Exception: " << ex.displayText().c_str() << "\n";
    exit(EXIT_FAILURE);
  }
}
