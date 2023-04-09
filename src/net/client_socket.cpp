#include "client_socket.h"

#include <netdb.h>

#include <iostream>
#include <memory>
#include <string>

#include "tcp_socket.h"

namespace spx {

ClientSocket::ClientSocket(int fd) : TcpSocket(fd) {}

ClientSocket::ClientSocket(const addrinfo *const info) : TcpSocket(info) {}

size_t ClientSocket::send(const char *data_ptr, size_t data_len) const {
  ssize_t res = ::send(getFileDescriptor(), data_ptr, data_len, 0);
  if (res == -1) {
    std::stringstream ss;
    ss << "Error on send(): " << std::strerror(errno);
    throw Exception(ss.str());
  }

  return static_cast<size_t>(res);
}

size_t ClientSocket::send(const std::string &data) const {
  const char *data_ptr = data.c_str();
  size_t data_len = std::char_traits<char>::length(data_ptr);

  return send(data_ptr, data_len);
}

size_t ClientSocket::send(const std::vector<char> &data) const {
  return send(data.data(), data.size());
}

size_t ClientSocket::receive(char *buffer, size_t buffer_len) const {
  ssize_t res = recv(getFileDescriptor(), buffer, buffer_len, 0);
  if (res == -1) {
    std::stringstream ss;
    ss << "Error on recv(): " << std::strerror(errno);
    throw Exception(ss.str());
  }

  if (res == 0) {
    throw ZeroLengthMessageException();
  }

  return static_cast<size_t>(res);
}

size_t ClientSocket::receive(std::vector<char> &buffer) const {
  return receive(buffer.data(), buffer.capacity());
}

void ClientSocket::shutdownWrite() {
  if (shutdown(getFileDescriptor(), SHUT_WR) == -1) {
    std::stringstream ss;
    ss << "Error on shutdown(write): " << std::strerror(errno);
    throw Exception(ss.str());
  }
}

void ClientSocket::connect(const sockaddr *const addr,
                           const socklen_t &addr_len) {
  if (::connect(getFileDescriptor(), addr, addr_len) == -1) {
    throw Exception("Error on connect()");
  }
}

} // namespace spx
