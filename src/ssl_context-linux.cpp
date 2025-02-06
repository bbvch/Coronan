#include "coronan/ssl_context.hpp"

#include <Poco/Net/Context.h>

namespace coronan::ssl_context {

Poco::Net::Context::Ptr create_ssl_context()
{
  using Poco::Net::Context;
  constexpr auto ca_location = "";
  constexpr auto private_key_file = "";
  constexpr auto certificate_file = "";
  constexpr auto verification_depth = 9;
  constexpr auto load_default_cas = false;
  constexpr auto cipher_list = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH";
  constexpr auto verification_mode = Context::VERIFY_RELAXED;
  return Context::Ptr{new Context{Context::TLS_CLIENT_USE, private_key_file, certificate_file, ca_location,
                                  verification_mode, verification_depth, load_default_cas, cipher_list}};
}

} // namespace coronan::ssl_context
