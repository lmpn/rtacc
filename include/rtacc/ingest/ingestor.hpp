#pragma once
#include <memory>
#include <string>
#include <system_error>
#include <utility>
namespace rtacc::ingest {

class ingestor
{
public:
  ingestor() = default;
  ingestor(const ingestor &) = default;
  ingestor(ingestor &&) = delete;
  ingestor &operator=(const ingestor &) = default;
  ingestor &operator=(ingestor &&) = delete;
  using read_cb = std::function<void(std::error_code, const std::string &)>;
  virtual ~ingestor() = default;
  void read(read_cb handler) { read_impl(std::move(handler)); }

private:
  virtual void read_impl(read_cb handler) = 0;
};
using ingestor_ptr = std::shared_ptr<ingestor>;
}// namespace rtacc::ingest