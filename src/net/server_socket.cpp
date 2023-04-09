#include "server_socket.h"

#include <algorithm>
#include <netdb.h>

#include <cstdint>
#include <iostream>
#include <memory>
#include <sys/socket.h>

#include "client_socket.h"
#include "tcp_socket.h"

namespace spx {

ServerSocket::ServerSocket(uint16_t port) : TcpSocket(), port_(port) {
  int enable_socket_reuse = 1;
  if (setsockopt(getFileDescriptor(), SOL_SOCKET, SO_REUSEADDR,
                 &enable_socket_reuse, sizeof(enable_socket_reuse)) < 0) {
    throw Exception("setsockopt(SO_REUSEADDR) failed");
  }
  if (setsockopt(getFileDescriptor(), SOL_SOCKET, SO_REUSEPORT,
                 &enable_socket_reuse, sizeof(enable_socket_reuse)) < 0) {
    throw Exception("setsockopt(SO_REUSEPORT) failed");
  }

  struct addrinfo hints {
  }, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = PF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo(nullptr, std::to_string(port_).c_str(), &hints, &res) != 0) {
    throw Exception("Failed to get addrinfo for server socket");
  }

  if (bind(getFileDescriptor(), res->ai_addr, res->ai_addrlen) != 0) {
    throw Exception("Failed to bind server socket");
  }

  freeaddrinfo(res);
}

void ServerSocket::listen() {
  if (::listen(getFileDescriptor(), SOMAXCONN) != 0) {
    throw Exception("error on listen");
  }
  std::cout << "Listening on 0.0.0.0:" << getPort() << std::endl;
}

ClientSocket ServerSocket::accept() {
  int conn_fd = ::accept(getFileDescriptor(), nullptr, nullptr);
  if (conn_fd == INVALID_SOCKET_FD) {
    throw Exception("error on accepting connection");
  }

  return ClientSocket(conn_fd);
}

uint16_t ServerSocket::getPort() const { return port_; }
} // namespace spx
