#include <iostream>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <boost/asio.hpp>
using namespace boost;

int main() {
  
  system::error_code ec;

  // Context Platform Specific Interface
  asio::io_context io_context;

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
    std::string s_request =
      "GET /index.html HTTP/1.1\r\n"
      "HOST: example.com\r\n"
      "Connection: close\r\n\r\n";

    socket.write_some(asio::buffer(s_request.data(), s_request.size()), ec);

    socket.wait(socket.wait_read);

    size_t bytes = socket.available();
    std::cout << "Bytes Available: " << bytes << std::endl;

    if (bytes > 0) {
      std::vector<char> v_buffer(bytes);
      socket.read_some(asio::buffer(v_buffer.data(), v_buffer.size()), ec);

      for (auto c : v_buffer) {
        std::cout << c;
      }
    }
  }
  return 0;
}