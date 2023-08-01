#include "buffered_file_ingestor.hpp"
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <spdlog/spdlog.h>
#include <system_error>
namespace rtacc::ingest {

buffered_file_ingestor::buffered_file_ingestor(const std::filesystem::path &path, std::streamsize size)
  : m_size{ size }, m_istream(path, std::ios::binary)
{
  m_buffer.resize(static_cast<std::size_t>(m_size));
}

void buffered_file_ingestor::read_impl(ingestor::read_cb handler)
{


  m_istream.read(m_buffer.data(), m_size);
  auto bytes = m_istream.gcount();
  std::size_t real_bytes = 0;
  if (bytes > 0) { real_bytes = static_cast<std::size_t>(bytes); }
  auto buffer = m_buffer.substr(0, real_bytes);
  handler(std::error_code(), buffer);
}
}// namespace rtacc::ingest