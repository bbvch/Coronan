#include "coronan/http_client.hpp"

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <doctest/doctest.h>
#include <iostream>
#include <sstream>

using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

namespace {

struct TestHTTPRequest
{
  TestHTTPRequest(std::string const& request, std::string const& path,
                  std::string const& type)
  {
    TestHTTPRequest::request_ = request;
    TestHTTPRequest::type_ = type;
    TestHTTPRequest::path_ = path;
  }

  inline static std::string request_{};
  inline static std::string type_{};
  inline static std::string path_{};
};

struct TestHTTPSession
{

  TestHTTPSession(std::string const& host, std::uint16_t port)
  {
    TestHTTPSession::port_ = port;
    TestHTTPSession::host_ = host;
  }

  std::ostream& sendRequest(TestHTTPRequest&) { return std::cout; }

  std::istream& receiveResponse(HTTPResponse& response)
  {
    response.setStatusAndReason(TestHTTPSession::response_status_,
                                TestHTTPSession::response_reason_);
    return TestHTTPSession::response_;
  }

  static void set_response_status(HTTPResponse::HTTPStatus const& status)
  {
    TestHTTPSession::response_status_ = status;
  }

  static void set_response_reason(std::string const& reason)
  {
    TestHTTPSession::response_reason_ = reason;
  }

  static void set_response(std::string const& response)
  {
    TestHTTPSession::response_ = std::istringstream{response};
  }

  inline static std::uint16_t port_{};
  inline static std::string host_{};
  inline static HTTPResponse::HTTPStatus response_status_{
      HTTPResponse::HTTP_OK};
  inline static std::string response_reason_{};
  inline static std::istringstream response_{""};
};

TEST_CASE("HTTPClient get")
{
  SUBCASE("Initializes a session")
  {
    auto uri = "http://server.com:80/";
    auto resonse =
        coronan::HTTPClientT<TestHTTPSession, TestHTTPRequest>::get(uri);

    REQUIRE(TestHTTPSession::host_ == "server.com");
    REQUIRE(TestHTTPSession::port_ == 80);
  }

  SUBCASE("Creates a request")
  {
    auto uri = "http://server.com:80/test";
    auto resonse =
        coronan::HTTPClientT<TestHTTPSession, TestHTTPRequest>::get(uri);
    REQUIRE(TestHTTPRequest::request_ == HTTPRequest::HTTP_GET);
    REQUIRE(TestHTTPRequest::type_ == HTTPMessage::HTTP_1_1);
    REQUIRE(TestHTTPRequest::path_ == "/test");
  }

  SUBCASE("Returns status, reason and response")
  {
    auto const expected_status = HTTPResponse::HTTP_FOUND;
    auto const expected_reason = "All ok";
    auto const expected_response = "Test";

    TestHTTPSession::set_response_status(expected_status);
    TestHTTPSession::set_response_reason(expected_reason);
    TestHTTPSession::set_response(expected_response);

    auto uri = "http://server.com:80/test";
    auto resonse =
        coronan::HTTPClientT<TestHTTPSession, TestHTTPRequest>::get(uri);

    REQUIRE(resonse.get_status() == expected_status);
    REQUIRE(resonse.get_reason() == expected_reason);
    REQUIRE(resonse.get_response_body() == expected_response);
  }
}

} // namespace