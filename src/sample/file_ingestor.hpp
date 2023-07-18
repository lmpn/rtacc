//
// FILE_INGESTOR.hpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef FILE_INGESTOR_HPP
#define FILE_INGESTOR_HPP

#include <asio/any_completion_handler.hpp>
#include <asio/async_result.hpp>
#include <asio/error.hpp>
#include <string>

class file_ingestor 
{
public:
  virtual ~file_ingestor() {}

  template <typename CompletionToken>
  auto read(CompletionToken&& token)
  {
    return asio::async_initiate<CompletionToken, void(asio::error_code, std::string)>(
        [](auto handler, file_ingestor* self)
        {
          self->read_impl(std::move(handler));
        }, token, this);
  }

private:
  virtual void read_impl(asio::any_completion_handler<void(asio::error_code, std::string)> handler) = 0;
};

#endif // FILE_INGESTOR_HPP