//
// buffered_file_ingestor.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef buffered_file_ingestor_HPP
#define buffered_file_ingestor_HPP

#include "file_ingestor.hpp"
#include <asio/posix/stream_descriptor.hpp>
#include <fstream>

class buffered_file_ingestor : public file_ingestor
{
public:
  explicit buffered_file_ingestor(asio::any_io_executor ex, std::string path);
  ~buffered_file_ingestor() override;
private:
  void read_impl(ReadCb handler) override;

  std::int32_t fd_;
  asio::posix::stream_descriptor stream_;
  const std::size_t size_{1024};
  std::string buffer_;
};

#endif