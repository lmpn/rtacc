#ifndef ingestor_HPP
#define ingestor_HPP
#include "asio.hpp"
#include <string>
#include <system_error>
#include <utility>
namespace rtacc::ingest {

class ingestor
{
public:
  using read_cb = std::function<void(std::error_code, std::string)>;
  virtual ~ingestor() = default;
  void read(read_cb handler) { read_impl(std::move(handler)); }

private:
  virtual void read_impl(read_cb handler) = 0;
};
}// namespace rtacc::ingest

#endif