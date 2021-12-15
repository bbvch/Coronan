#pragma once

#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>
#include <functional>
#include <stdexcept>
#include <string>

namespace coronan {

/**
 * A HTTPClientException including Http get and Net exceptions.
 */
class HTTPClientException : public std::exception
{
public:
  /**
   *  Constructor
   * @param exception_msg exception message
   */
  explicit HTTPClientException(std::string exception_msg);
  HTTPClientException(HTTPClientException const&) = delete;
  /**
   *  Return the exception message
   */
  char const* what() const noexcept override;

private:
  std::string const msg{};
};

/**
 * A HTTPResponse containing response status and payload
 */
class HTTPResponse
{
public:
  /**
   *  Constructor
   * @param response http response
   * @param response_body http response body
   */
  explicit HTTPResponse(Poco::Net::HTTPResponse const& response, std::string response_body);

  /**
   * Return the HTTP status code
   */
  Poco::Net::HTTPResponse::HTTPStatus status() const noexcept;

  /**
   * Return the HTTP reason phrase
   */
  std::string reason() const noexcept;

  /**
   * Return the HTTP response body
   */
  std::string response_body() const noexcept;

private:
  Poco::Net::HTTPResponse response_{};
  std::string response_body_{};
};

/**
 * Simple Stateless HTTP Client
 */
template <typename SessionType, typename HTTPRequestType, typename HTTPResponseType>
struct HTTPClientType
{
  /**
   * Execute a HTTP GET
   * @param url GET url
   */
  static HTTPResponse get(std::string const& url);
};

template <typename SessionType, typename HTTPRequestType, typename HTTPResponseType>
HTTPResponse HTTPClientType<SessionType, HTTPRequestType, HTTPResponseType>::get(std::string const& url)
{
  try
  {
    Poco::URI const uri{url};
    SessionType session(uri.getHost(), uri.getPort());

    auto const path = std::invoke([uri]() {
      auto const path_ = uri.getPathAndQuery();
      return path_.empty() ? "/" : path_;
    });

    HTTPRequestType request{"GET", path, "HTTP/1.1"};

    HTTPResponseType response;
    session.sendRequest(request);
    auto& response_stream = session.receiveResponse(response);

    std::string const response_content = std::invoke([&response_stream]() {
      std::string content;
      Poco::StreamCopier::copyToString(response_stream, content);
      return content;
    });

    return HTTPResponse{response, response_content};
  }
  catch (std::exception const& ex)
  {
    auto const exception_msg =
        std::string{"Error fetching url \""} + url + std::string{"\".\n\n Exception occurred: "} + ex.what();
    throw HTTPClientException{exception_msg};
  }
}

} // namespace coronan
