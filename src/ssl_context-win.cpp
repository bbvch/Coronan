#include "coronan/ssl_context.h"

namespace coronan::ssl_context {

Poco::Net::Context::Ptr create_ssl_context()
{
  using Poco::Net::Context;
  static constexpr auto ca_location = "";
  static constexpr auto verification_mode = Context::VERIFY_RELAXED;
  return Context::Ptr{new Context{Context::TLS_CLIENT_USE, ca_location, verification_mode}};
}

} // namespace coronan::ssl_context
