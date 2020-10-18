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

  // Perform IO Operations
  asio::ip::tcp::socket socket(io_context);

  // IP endpoint & Make Address so ASIO can recognize
  asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);
  return 0;
}