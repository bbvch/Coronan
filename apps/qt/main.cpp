#include "mainwindow.h"

#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/SSLException.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/SharedPtr.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

constexpr auto api_url = "https://corona-api.com/countries/ch";

using namespace Poco;
using namespace Poco::Net;

class SSLInitializer
{
public:
  SSLInitializer() { initializeSSL(); }

  ~SSLInitializer() { uninitializeSSL(); }
};

int main(int argc, char* argv[])
{
  SSLInitializer sslInitializer;

  SharedPtr<InvalidCertificateHandler> ptrCert =
      new AcceptCertificateHandler(false);
  Context::Ptr ptrContext =
      new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9,
                  false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
  SSLManager::instance().initializeClient(0, ptrCert, ptrContext);

  QApplication a(argc, argv);
  QMainWindow window;
  CoronanWidget* widget = new CoronanWidget(api_url);
  window.setCentralWidget(widget);
  window.resize(900, 600);
  window.show();
  return a.exec();
}
