#include "buffered_file_ingestor.hpp"
#include <fstream>
#include <spdlog/spdlog.h>
namespace rtacc::ingest {

buffered_file_ingestor::buffered_file_ingestor(const asio::any_io_executor &ex,
  const std::filesystem::path &path,
  std::size_t size)
  : m_file(rtacc::io::file(path)), m_stream(ex, m_file.desc()), m_size{ size }
{
  m_buffer.resize(m_size);
}

void buffered_file_ingestor::read_impl(ingestor::read_cb handler)
{
  auto read_handler = [self = shared_from_this(), handler = std::move(handler)](std::error_code code, std::size_t length) {
    auto payload = self->m_buffer.substr(0, length);
    handler(code, payload);
  };

  asio::async_read(m_stream, asio::buffer(m_buffer, m_size), read_handler);
}
}// namespace rtacc::ingest