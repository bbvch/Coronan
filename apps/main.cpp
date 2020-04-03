#include "coronan/http_client.hpp"

#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/SSLException.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/SharedPtr.h>
#include <iostream>

using namespace Poco;
using namespace Poco::Net;

class SSLInitializer
{
public:
  SSLInitializer() { initializeSSL(); }

  ~SSLInitializer() { uninitializeSSL(); }
};

int main(int, char**)
{
  SSLInitializer sslInitializer;

  SharedPtr<InvalidCertificateHandler> ptrCert =
      new AcceptCertificateHandler(false);
  Context::Ptr ptrContext =
      new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9,
                  false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
  SSLManager::instance().initializeClient(0, ptrCert, ptrContext);

  try
  {

    auto const url = "https://corona-api.com/countries/ch";

    auto response = coronan::HTTPClient<>::get(url);

    std::cout << response.get_status() << " " << response.get_reason() << "\n";
    std::cout << response.get_response() << "\n";
  }
  catch (Poco::Net::SSLException const& ex)
  {
    std::cout << "Exception: " << ex.displayText() << "\n";
  }
  exit(EXIT_SUCCESS);
}