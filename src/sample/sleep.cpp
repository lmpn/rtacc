#include "sleep.hpp"
#include <asio/consign.hpp>
#include <asio/steady_timer.hpp>
#include <memory>

void async_sleep_impl(
    asio::any_completion_handler<void(std::error_code)> handler,
    asio::any_io_executor ex, std::chrono::nanoseconds duration)
{
  auto timer = std::make_shared<asio::steady_timer>(ex, duration);
  timer->async_wait(asio::consign(std::move(handler), timer));
}