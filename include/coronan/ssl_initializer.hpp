#pragma once

#include <Poco/Net/Context.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/SSLException.h>
#include <Poco/SharedPtr.h>
#include <memory>

using Poco::Net::InvalidCertificateHandler;

namespace coronan {

using SSLException = Poco::Net::SSLException;

/**
 * An RAII wrapper to initialize & uninitializeSSL the POCO::Net SSL stuff
 * (Poco::Net::initializeSSL())
 */
class SSLInitializer
{
public:
  explicit SSLInitializer(
      Poco::SharedPtr<InvalidCertificateHandler> certificate_handler,
      Poco::Net::Context::Ptr context);
  void initialize();

  ~SSLInitializer();

  [[nodiscard]] static std::unique_ptr<SSLInitializer>
  initialize_with_accept_certificate_handler();

private:
  Poco::SharedPtr<InvalidCertificateHandler> certificate_handler_ptr{};
  Poco::Net::Context::Ptr context_ptr;
};

} // namespace coronan
