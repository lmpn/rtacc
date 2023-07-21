//
// buffered_ingestor.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ingestor_HPP
#define ingestor_HPP
#include "asio.hpp"
#include <string>
namespace rtacc::ingest {

class ingestor
{
public:
  using read_cb = std::function<void(std::string)>;
  explicit ingestor() = default;
  virtual ~ingestor() = default;
  void read(read_cb handler) { read_impl(handler); }

private:
  virtual void read_impl(read_cb handler) = 0;
};
}// namespace rtacc::ingest

#endif