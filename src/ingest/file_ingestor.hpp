#ifndef FILE_INGESTOR_IMPL_HPP
#define FILE_INGESTOR_IMPL_HPP
#include <memory>
#include "rtacc/ingest/ingestor_impl.hpp"

namespace rtacc::ingestion{

class FileIngestor : IngestorImpl {

public:
    FileIngestor() = default;
    ~FileIngestor() override = default;
    void ingest() override {};
};

}
#endif
