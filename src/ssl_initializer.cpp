#include "coronan/ssl_initializer.hpp"

#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/SSLManager.h>

namespace coronan {

using Poco::Net::Context;

SSLInitializer::SSLInitializer(
    Poco::Net::InvalidCertificateHandler* certificate_handler,
    Poco::Net::Context::Ptr context)
    : certificate_handler_ptr{certificate_handler}, context_ptr{context}
{
  Poco::Net::initializeSSL();
}

void SSLInitializer::initialize()
{
  Poco::Net::SSLManager::instance().initializeClient(0, certificate_handler_ptr,
                                                     context_ptr);
}

SSLInitializer::~SSLInitializer() { Poco::Net::uninitializeSSL(); }

std::unique_ptr<SSLInitializer>
SSLInitializer::initialize_with_accept_certificate_handler()
{
  auto ssl_initializer = std::make_unique<SSLInitializer>(
      new Poco::Net::AcceptCertificateHandler{false},
      Context::Ptr{new Context{Context::CLIENT_USE, "", "", "",
                               Context::VERIFY_RELAXED, 9, false,
                               "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"}});

  ssl_initializer->initialize();
  return ssl_initializer;
}

} // namespace coronan