#pragma once

#include "Poco/StreamCopier.h"

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <string>

namespace coronan {

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
                        std::string const& response_body)
      : response_{response}, response_body_{response_body}
  {
  }

  /**
   * Return the HTTP status code
   */ 
  Poco::Net::HTTPResponse::HTTPStatus get_status() const
  {
    return response_.getStatus();
  }

   /**
   * Return the HTTP reason phrase
   */ 
  std::string get_reason() const { return response_.getReason(); }

  /**
   * Return the HTTP response body
   */ 
  std::string get_response_body() const { return response_body_; }

private:
  Poco::Net::HTTPResponse response_{};
  std::string response_body_{};
};

/**
 * Simple Stateless HTTP Client using by default a HTTPS Session
 */ 
template <typename SessionT = Poco::Net::HTTPSClientSession,
          typename HTTPRequestT = Poco::Net::HTTPRequest>
struct HTTPClientT
{
  /**
   * Execute a HTTP GET
   * @param url GET url
   */
  static HTTPResponse get(std::string const& url)
  {

    Poco::URI uri{url};
    SessionT session(uri.getHost(), uri.getPort());

    auto const path = [uri]() {
      auto const path_ = uri.getPathAndQuery();
      return path_.empty() ? "/" : path_;
    }();

    HTTPRequestT request{Poco::Net::HTTPRequest::HTTP_GET, path,
                         Poco::Net::HTTPMessage::HTTP_1_1};

    Poco::Net::HTTPResponse response;
    session.sendRequest(request);
    auto& response_stream = session.receiveResponse(response);

    std::string const response_content = [&response_stream]() {
      std::string content;
      Poco::StreamCopier::copyToString(response_stream, content);
      return content;
    }();

    return HTTPResponse{response, response_content};
  }
};

using HTTPClient = HTTPClientT<>;

} // namespace coronan