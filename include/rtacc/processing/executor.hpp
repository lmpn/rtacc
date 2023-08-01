#pragma once
#include <functional>
#include <memory>
namespace rtacc::processing {

class executor
{
public:
  using task = std::function<void()>;
  explicit executor() = default;
  executor(const executor &) = delete;
  executor(executor &&) = default;
  executor &operator=(const executor &) = delete;
  executor &operator=(executor &&) = default;
  virtual ~executor() = default;
  virtual void post(const task &job) = 0;
  virtual void wait() = 0;
};
using executor_ptr = std::shared_ptr<executor>;
}// namespace rtacc::processing
