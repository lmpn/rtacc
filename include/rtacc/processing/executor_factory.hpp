#pragma once
#include "rtacc/processing/executor.hpp"
#include <functional>
#include <memory>
namespace rtacc::processing {

class executor_factory
{
public:
  static executor_ptr create_asio_thread_pool_executor(std::size_t threads);
};
}// namespace rtacc::processing
