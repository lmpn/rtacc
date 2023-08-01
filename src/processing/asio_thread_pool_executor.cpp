#include "asio_thread_pool_executor.hpp"
#include <asio/post.hpp>

namespace rtacc::processing {

asio_thread_pool_executor::asio_thread_pool_executor(std::size_t num_threads) : m_pool(num_threads) {}

void asio_thread_pool_executor::post(const executor::task &job)
{
  asio::post(m_pool, [job]() { job(); });
}

asio_thread_pool_executor::~asio_thread_pool_executor() { m_pool.executor(); }
void asio_thread_pool_executor::wait() { m_pool.wait(); }

}// namespace rtacc::processing