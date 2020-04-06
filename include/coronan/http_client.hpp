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

class HTTPResponse
{
public:
  explicit HTTPResponse(Poco::Net::HTTPResponse const& response,
                        std::string const& response_str)
      : response_{response}, response_str_{response_str}
  {
  }

  Poco::Net::HTTPResponse::HTTPStatus get_status() const
  {
    return response_.getStatus();
  }

  std::string get_reason() const { return response_.getReason(); }

  std::string get_response() const { return response_str_; }

private:
  Poco::Net::HTTPResponse response_;
  std::string response_str_;
};

template <typename SessionT = Poco::Net::HTTPSClientSession,
          typename HTTPRequestT = Poco::Net::HTTPRequest>
struct HTTPClientT
{
  static HTTPResponse get(std::string const& uri_str)
  {

    Poco::URI uri{uri_str};
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