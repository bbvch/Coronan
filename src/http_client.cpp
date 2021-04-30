#include "coronan/http_client.hpp"

namespace coronan {

HTTPClientException::HTTPClientException(std::string exception_msg)
    : msg{std::move(exception_msg)}
{
}

char const* HTTPClientException::what() const noexcept { return msg.c_str(); }

HTTPResponse::HTTPResponse(Poco::Net::HTTPResponse const& response,
                           std::string response_body)
    : response_{response}, response_body_{std::move(response_body)}
{
}

Poco::Net::HTTPResponse::HTTPStatus
// cppcheck-suppress unusedFunction
HTTPResponse::get_status() const
{
  return response_.getStatus();
}
// cppcheck-suppress unusedFunction
std::string HTTPResponse::get_reason() const { return response_.getReason(); }
// cppcheck-suppress unusedFunction
std::string HTTPResponse::get_response_body() const { return response_body_; }

} // namespace coronan
