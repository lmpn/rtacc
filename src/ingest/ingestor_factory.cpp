#include "buffered_file_ingestor.hpp"
#include <memory>
#include <rtacc/ingest/ingestor_factory.hpp>
namespace rtacc::ingest {

ingestor_ptr ingestor_factory::create_buffered_file_ingestor(const std::filesystem::path &path, std::streamsize size)
{
  return std::make_shared<buffered_file_ingestor>(path, size);
}

}// namespace rtacc::ingest
