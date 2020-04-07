#pragma once

#include <Poco/Net/Context.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/SSLException.h>
#include <Poco/SharedPtr.h>
#include <memory>

using Poco::Net::InvalidCertificateHandler;

namespace coronan {

using SSLException = Poco::Net::SSLException;

class SSLInitializer
{
public:
  explicit SSLInitializer(InvalidCertificateHandler* certificate_handler,
                          Poco::Net::Context::Ptr context);
  void initialize();

  ~SSLInitializer();

  [[nodiscard]] static std::unique_ptr<SSLInitializer> initialize_with_accept_certificate_handler();

private:
  Poco::SharedPtr<InvalidCertificateHandler> certificate_handler_ptr;
  Poco::Net::Context::Ptr context_ptr;
};

} // namespace coronan