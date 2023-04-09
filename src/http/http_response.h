#pragma once

#include <map>
#include <string>
#include <vector>

#include "http_status.h"

namespace spx {

class HttpResponse {
public:
  explicit HttpResponse(const std::string &body);

  std::vector<char> toRaw();

private:
  HttpStatus status_{HttpStatus::Code::OK};
  int majorVersion_{1};
  int minorVersion_{0};
  std::map<std::string, std::string> headers_{
      {"Server", "ksws"},
  };
  std::vector<char> body_;
};

} // namespace spx
