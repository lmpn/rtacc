#pragma once
#include "rtacc/ingest/ingestor.hpp"
#include <filesystem>
#include <ios>
#include <string>
#include <system_error>
#include <utility>
namespace rtacc::ingest {

class ingestor_factory
{
public:
  static ingestor_ptr create_buffered_file_ingestor(const std::filesystem::path &path, std::streamsize size);
};
}// namespace rtacc::ingest
