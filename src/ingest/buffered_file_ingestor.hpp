#ifndef buffered_file_ingestor_HPP
#define buffered_file_ingestor_HPP

#include <asio.hpp>
#include <asio/posix/stream_descriptor.hpp>
#include <fstream>
#include <ios>
#include <memory>
#include <rtacc/ingest/ingestor.hpp>

namespace rtacc::ingest {

class buffered_file_ingestor
  : public ingestor
  , public std::enable_shared_from_this<buffered_file_ingestor>
{
public:
  explicit buffered_file_ingestor(const std::filesystem::path &path, std::streamsize size);
  buffered_file_ingestor(const buffered_file_ingestor &) = delete;
  buffered_file_ingestor(buffered_file_ingestor &&) = delete;
  buffered_file_ingestor &operator=(const buffered_file_ingestor &) = delete;
  buffered_file_ingestor &operator=(buffered_file_ingestor &&) = delete;
  ~buffered_file_ingestor() override = default;

private:
  void read_impl(ingestor::read_cb handler) override;

  std::streamsize m_size;
  std::string m_buffer;
  std::ifstream m_istream;
};

}// namespace rtacc::ingest

#endif