#include "mainwindow.h"

#include "coronan/ssl_initializer.hpp"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <QDebug>

#include <iostream>

constexpr auto api_url = "https://corona-api.com/countries";

int main(int argc, char* argv[])
{
  static auto const ssl_initializer_handler = coronan::SSLInitializer::initialize_with_accept_certificate_handler();
  try
  {
 
    QApplication a(argc, argv);
    QMainWindow window;
    CoronanWidget* widget = new CoronanWidget(api_url);
    window.setCentralWidget(widget);
    window.resize(900, 600);
    window.show();
    return a.exec();
  }
  catch (coronan::SSLException const& ex)
  {
    qCritical() << "SSL Exception: " << ex.displayText().c_str() << "\n";
    exit(EXIT_FAILURE);
  }
}
