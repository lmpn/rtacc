//
// buffered_file_ingestor.cpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "buffered_file_ingestor.hpp"
#include <asio/deferred.hpp>
#include <asio/read.hpp>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

buffered_file_ingestor::buffered_file_ingestor(asio::any_io_executor ex, std::string path)
  : fd_(open(path.c_str(), O_RDONLY)), stream_(ex, fd_)
{

  buffer_.resize(size_);
}
buffered_file_ingestor::~buffered_file_ingestor()
{
  close(fd_);
}

void buffered_file_ingestor::read_impl(asio::any_completion_handler<void(asio::error_code, std::string)> handler)
{
  auto cb = asio::deferred([this](asio::error_code ec, std::size_t n) {
    std::string result = buffer_.substr(0, n);
    buffer_.erase(0, n);
    std::cout << result;
    return asio::deferred.values(ec, std::move(result));
  });

  asio::async_read(stream_, asio::buffer(buffer_, size_), std::move(cb))(std::move(handler));
}
