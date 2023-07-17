#ifndef INGESTOR_HPP
#define INGESTOR_HPP
#include <memory>
#include "ingestor_impl.hpp"

namespace rtacc::ingestion{

class Ingestor{

public:
    Ingestor() = default;
    virtual ~Ingestor() = default;
    void ingest() {
        m_impl->ingest();
    };
private: 
    std::unique_ptr<IngestorImpl> m_impl;
};

}
#endif
