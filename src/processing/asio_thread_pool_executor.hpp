#pragma once
#include "rtacc/processing/executor.hpp"
#include <asio/thread_pool.hpp>
#include <functional>
#include <memory>
namespace rtacc::processing {

class asio_thread_pool_executor : public executor
{
private:
  asio::thread_pool m_pool;

public:
  explicit asio_thread_pool_executor(std::size_t num_threads);
  ~asio_thread_pool_executor() override;
  asio_thread_pool_executor(const asio_thread_pool_executor &) = delete;
  asio_thread_pool_executor(asio_thread_pool_executor &&) = delete;
  asio_thread_pool_executor &operator=(const asio_thread_pool_executor &) = delete;
  asio_thread_pool_executor &operator=(asio_thread_pool_executor &&) = delete;
  void post(const task &job) override;
  void wait() override;
};
using asio_executor_ptr = std::shared_ptr<asio_thread_pool_executor>;
}// namespace rtacc::processing
