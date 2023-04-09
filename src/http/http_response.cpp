#include "http_response.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace spx {

const std::string CRLF = "\r\n";

std::string getDate() {
  std::stringstream ss;

  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  auto tm = gmtime(&time);
  ss << std::put_time(tm, "%a, %d %b %Y %H:%M:%S GMT");

  return ss.str();
}

HttpResponse::HttpResponse(const std::string &body)
    : body_(body.begin(), body.end()) {}

std::vector<char> HttpResponse::toRaw() {
  headers_.emplace("Content-Type", "text/plain");
  headers_.emplace("Content-Length", std::to_string(body_.size()));
  headers_.emplace("Date", getDate());

  std::stringstream ss;
  ss << "HTTP/" << majorVersion_ << "." << minorVersion_ << " "
     << status_.toInt() << " " << status_.reasonPhrase() << CRLF;
  for (const auto &header : headers_) {
    ss << header.first << ": " << header.second << CRLF;
  }
  ss << CRLF;

  std::string prebody = ss.str();
  std::vector<char> raw(prebody.begin(), prebody.end());
  raw.insert(raw.end(), body_.begin(), body_.end());

  return raw;
}

} // namespace spx
