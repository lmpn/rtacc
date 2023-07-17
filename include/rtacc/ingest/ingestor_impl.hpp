#ifndef INGESTOR_IMPL_HPP
#define INGESTOR_IMPL_HPP
#include <memory>

namespace rtacc::ingestion{

class IngestorImpl{

public:
    IngestorImpl() = default;
    virtual ~IngestorImpl() = default;
    virtual void ingest() = 0;
};

}
#endif
