#ifndef buffered_file_ingestor_HPP
#define buffered_file_ingestor_HPP

#include <asio.hpp>
#include <asio/posix/stream_descriptor.hpp>
#include <memory>
#include <rtacc/ingest/ingestor.hpp>
#include <rtacc/io/file.hpp>

namespace rtacc::ingest {

class buffered_file_ingestor : public ingestor, public std::enable_shared_from_this<buffered_file_ingestor>
{
public:
  buffered_file_ingestor(const buffered_file_ingestor &) = delete;
  buffered_file_ingestor(buffered_file_ingestor &&) = delete;
  buffered_file_ingestor &operator=(const buffered_file_ingestor &) = delete;
  buffered_file_ingestor &operator=(buffered_file_ingestor &&) = delete;
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