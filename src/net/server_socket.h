#pragma once

#include <_types/_uint16_t.h>
#include <cstdint>
#include <memory>

#include "client_socket.h"
#include "tcp_socket.h"

namespace spx {

class ServerSocket : public TcpSocket {
public:
  explicit ServerSocket(uint16_t port);

  void listen();

  ClientSocket accept();

  uint16_t getPort() const;

private:
  uint16_t port_;
};

} // namespace spx
