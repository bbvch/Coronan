#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"

#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/SSLException.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/SharedPtr.h>
#include <iostream>
#include <lyra/lyra.hpp>

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

  try
  {
    std::string country = "ch";
    bool help_request = false;
    auto cli = lyra::cli_parser() | lyra::help(help_request) |
               lyra::opt(country, "country")["-c"]["--country"]("Country Code");

    auto const result = cli.parse({argc, argv});
    if (!result)
    {
      std::cerr << "Error in comman line: " << result.errorMessage() << "\n";
      exit(EXIT_FAILURE);
    }

    if (help_request)
    {
      std::cout << cli;
      exit(EXIT_SUCCESS);
    }

    auto const url = "https://corona-api.com/countries/" + country;

    auto response = coronan::HTTPClient::get(url);

    auto const& data = coronan::api_parser::parse(response.get_response_body());
    std::cout << "\"datetime\", \"confirmed\", \"death\", \"recovered\", "
                 "\"active\"\n";

    for (auto const& data_points : data.timeline)
    {
      std::cout << data_points.date << ", " << data_points.confirmed << ", "
                << data_points.deaths << ", " << data_points.recovered << ", "
                << data_points.active << "\n";
    }
  }
  catch (Poco::Net::SSLException const& ex)
  {
    std::cout << "Exception: " << ex.displayText() << "\n";
  }
  exit(EXIT_SUCCESS);
}