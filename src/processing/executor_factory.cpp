#include "rtacc/processing/executor_factory.hpp"
#include "asio_thread_pool_executor.hpp"
#include <memory>
namespace rtacc::processing {

executor_ptr executor_factory::create_asio_thread_pool_executor(std::size_t threads)
{
  return std::make_shared<rtacc::processing::asio_thread_pool_executor>(threads);
}

}// namespace rtacc::processing
