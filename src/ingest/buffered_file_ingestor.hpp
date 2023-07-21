#ifndef buffered_file_ingestor_HPP
#define buffered_file_ingestor_HPP

#include <asio.hpp>
#include <asio/posix/stream_descriptor.hpp>
#include <rtacc/ingest/ingestor.hpp>
#include <rtacc/io/file.hpp>

namespace rtacc::ingest {

class buffered_file_ingestor : public ingestor
{
public:
  explicit buffered_file_ingestor(const asio::any_io_executor &ex, const std::filesystem::path &path, std::size_t size);
  ~buffered_file_ingestor() override = default;

private:
  void read_impl(ingestor::read_cb handler) override;

  io::file m_file;
  asio::posix::stream_descriptor m_stream;
  std::size_t m_size;
  std::string m_buffer;
};

}// namespace rtacc::ingest

#endif