
#include "coronan/ssl_initializer.hpp"

#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/SSLManager.h>

namespace {
using Poco::Net::Context;

constexpr auto ca_location = "";
constexpr auto verification_mode = Context::VERIFY_RELAXED;

#ifdef WIN32
constexpr auto create_NetSSL_context = []() {
  return Context::Ptr{
      new Context{Context::TLS_CLIENT_USE, ca_location, verification_mode}};
};
#else
constexpr auto private_key_file = "";
constexpr auto certificate_file = "";
constexpr auto verification_depth = 9;
constexpr auto load_default_cas = false;
constexpr auto cipher_list = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH";

constexpr auto create_NetSSL_context = []() {
  return Context::Ptr{new Context{
      Context::TLS_CLIENT_USE, private_key_file, certificate_file, ca_location,
      verification_mode, verification_depth, load_default_cas, cipher_list}};
};
#endif // WIN32
} // namespace

namespace coronan {

using Poco::Net::Context;

SSLClient::SSLClient(
    Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> cert_handler,
    Poco::Net::Context::Ptr net_context)
    : certificate_handler{std::move(cert_handler)}, context{
                                                        std::move(net_context)}
{
  Poco::Net::initializeSSL();
}

void SSLClient::initialize()
{
  Poco::Net::SSLManager::instance().initializeClient(
      nullptr, certificate_handler, context);
}

SSLClient::SSLClientPtr
// cppcheck-suppress unusedFunction
SSLClient::create_with_accept_certificate_handler()
{
  constexpr auto handle_errors_on_server_side = false;

  Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> cert_handler =
      new Poco::Net::AcceptCertificateHandler{handle_errors_on_server_side};

  // Using `new` to access a non-public constructor.
  auto ssl_client = SSLClientPtr{
      new SSLClient{std::move(cert_handler), create_NetSSL_context()},
      [](SSLClient*) { Poco::Net::uninitializeSSL(); }};

  ssl_client->initialize();
  return ssl_client;
}

} // namespace coronan
