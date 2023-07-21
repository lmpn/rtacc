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
  auto cb = [this, handler = std::move(handler)](std::error_code ec, std::size_t length) {
    fmt::print("ec: {} len: {}\n", ec.message(), length);
    auto payload = m_buffer.substr(0, length);
    m_buffer.erase(0, length);
    handler(payload);
  };

  asio::async_read(m_stream, asio::buffer(m_buffer, m_size), cb);
}
}// namespace rtacc::ingest