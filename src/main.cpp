#include "common/exception.h"
#include "common/thread_pool.h"
#include "http/http_response.h"
#include "net/client_socket.h"
#include "net/server_socket.h"
#include <functional>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <sstream>
#include <sys/_endian.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

const int BUFFER_SIZE = 4096; // 4kb
const int PORT = 9977;

int main() {
  spx::ServerSocket server_socket(PORT);
  server_socket.listen();

  spx::ThreadPool thread_pool;
  int req_cnt = 0;
  while (true) {
    auto conn_ptr = std::make_shared<spx::ClientSocket>(server_socket.accept());
    req_cnt++;

    thread_pool.submit([=]() {
      std::vector<char> buffer(BUFFER_SIZE);
      try {
        conn_ptr->receive(buffer);
      } catch (const spx::ZeroLengthMessageException &) {
        std::cout << "zero length message, closing connection" << std::endl;
        return;
      }

      std::stringstream resp_body;
      resp_body << "<h1>Hello, your request is #" << req_cnt << "!</h1>";

      conn_ptr->send(spx::HttpResponse(resp_body.str()).toRaw());
    });
  }

  return 0;
}
