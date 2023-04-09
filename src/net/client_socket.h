#pragma once

#include <netdb.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../common/exception.h"
#include "tcp_socket.h"

namespace spx {

class ZeroLengthMessageException : public Exception {
public:
  ZeroLengthMessageException() : Exception("Message with length 0") {}
};

class ClientSocket : public TcpSocket {
public:
  explicit ClientSocket(int fd);
  explicit ClientSocket(const addrinfo *info);

  size_t send(const char *data_ptr, size_t data_len) const;
  size_t send(const std::string &data) const;
  size_t send(const std::vector<char> &data) const;

  size_t receive(char *buffer, size_t buffer_len) const;
  size_t receive(std::vector<char> &buffer) const;

  void shutdownWrite();

  void connect(const sockaddr *addr, const socklen_t &addr_len);

private:
};

} // namespace spx
