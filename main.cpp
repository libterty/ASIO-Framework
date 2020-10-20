#include <iostream>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <boost/asio.hpp>
using namespace boost;

std::vector<char> v_buffer(20 * 1024);

void bufferHandler(asio::ip::socket& socket) {
  socket.async_read_some(asio::buffer(v_buffer.data(), v_buffer.size()),
    [&](std::error_code ec, std::size_t length) {
      if (!ec) {
        std::cout << "\n\nRead" << length << "\nbytes\n\n";

        for (int i = 0; i < length; i++) {
          std::cout << v_buffer[i];
        }

        bufferHandler(socket);
      }
    }
  );
}

int main() {
  
  system::error_code ec;

  // Context Platform Specific Interface
  asio::io_context io_context;

  asio::io_context::work idle_work(io_context);

  // Run Context in own thread dont block main thread
  std::thread thread_context = std::thread([&])() {
    io_context.run();
  }

  // IP endpoint & Make Address so ASIO can recognize
  asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);

    // Perform IO Operations
  asio::ip::tcp::socket socket(io_context);

  if (!ec) {
    std::cout << "Connect Success!" << std::endl;
  } else {
    std::cout << "Failed to Connect to Address:\n" << ec.message() << std::endl;
  }

  if (socket.is_open()) {
    bufferHandler(socket);

    std::string s_request =
      "GET /index.html HTTP/1.1\r\n"
      "HOST: example.com\r\n"
      "Connection: close\r\n\r\n";

    socket.write_some(asio::buffer(s_request.data(), s_request.size()), ec);
  }
  return 0;
}