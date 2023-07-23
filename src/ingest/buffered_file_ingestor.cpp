//
// buffered_file_ingestor.cpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "buffered_file_ingestor.hpp"
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
  auto read_handler = [this, handler = std::move(handler)](std::error_code code, std::size_t length) {
    if (code.value() != 0) {
      spdlog::error("Error({}): {} \n", code.value(), code.message());
      return;
    }
    auto payload = m_buffer.substr(0, length);
    m_buffer.erase(0, length);
    handler(payload);
  };

  asio::async_read(m_stream, asio::buffer(m_buffer, m_size), read_handler);
}
}// namespace rtacc::ingest