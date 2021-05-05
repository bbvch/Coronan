#pragma once

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/NetException.h>
#include <Poco/Path.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>
#include <stdexcept>
#include <string>

namespace coronan {

class HTTPClientException : public std::exception
{
public:
  explicit HTTPClientException(std::string exception_msg);
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
   *  Consructor
   * @param response http response status
   * @param response_body http response body
   */
  explicit HTTPResponse(Poco::Net::HTTPResponse const& response,
                        std::string response_body);

  /**
   * Return the HTTP status code
   */
  Poco::Net::HTTPResponse::HTTPStatus get_status() const;

  /**
   * Return the HTTP reason phrase
   */
  std::string get_reason() const;

  /**
   * Return the HTTP response body
   */
  std::string get_response_body() const;

private:
  Poco::Net::HTTPResponse response_{};
  std::string response_body_{};
};

/**
 * Simple Stateless HTTP Client
 */
template <typename SessionT, typename HTTPRequestT, typename HTTPResponseT>
struct HTTPClientT
{
  /**
   * Execute a HTTP GET
   * @param url GET url
   */
  static HTTPResponse get(std::string const& url);
};

template <typename SessionT, typename HTTPRequestT, typename HTTPResponseT>
HTTPResponse
HTTPClientT<SessionT, HTTPRequestT, HTTPResponseT>::get(std::string const& url)
{
  try
  {
    Poco::URI uri{url};
    SessionT session(uri.getHost(), uri.getPort());

    auto const path = [uri]() {
      auto const path_ = uri.getPathAndQuery();
      return path_.empty() ? "/" : path_;
    }();

    HTTPRequestT request{"GET", path, "HTTP/1.1"};

    HTTPResponseT response;
    session.sendRequest(request);
    auto& response_stream = session.receiveResponse(response);

    std::string const response_content = [&response_stream]() {
      std::string content;
      Poco::StreamCopier::copyToString(response_stream, content);
      return content;
    }();

    return HTTPResponse{response, response_content};
  }
  catch (std::exception const& ex)
  {
    auto const exception_msg = std::string{"Error fetching url \""} + url +
                               std::string{"\".\n\n Exception occured: "} +
                               ex.what();
    throw HTTPClientException{exception_msg};
  }
}

} // namespace coronan
