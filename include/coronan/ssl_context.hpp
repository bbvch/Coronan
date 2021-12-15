#pragma once

#include <Poco/Net/Context.h>

namespace coronan {
namespace ssl_context {

Poco::Net::Context::Ptr create_ssl_context();

} // namespace ssl_context
} // namespace coronan
