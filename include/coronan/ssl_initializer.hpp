#pragma once

#include <Poco/Net/Context.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/SSLException.h>
#include <Poco/SharedPtr.h>
#include <functional>
#include <memory>

namespace coronan {

using SSLException = Poco::Net::SSLException;

/**
 * An RAII wrapper to initialize & uninitialize the POCO::Net SSL stuff
 * (Poco::Net::initializeSSL())
 */
class SSLClient
{
public:
  using SSLClientPtr =
      std::unique_ptr<SSLClient, std::function<void(SSLClient*)>>;

  [[nodiscard]] static SSLClientPtr create_with_accept_certificate_handler();

private:
  explicit SSLClient(
      Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> cert_handler,
      Poco::Net::Context::Ptr net_context);

  void initialize();
  Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> certificate_handler{};
  Poco::Net::Context::Ptr context;
};

} // namespace coronan
