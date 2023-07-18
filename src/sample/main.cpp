#include <array>
#include <functional>
#include <iostream>
#include <optional>
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>


// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `rtacc`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

int cli_main(int argc, const char **argv)
{
  try {
    CLI::App app{ fmt::format("{} version {}", rtacc::cmake::project_name, rtacc::cmake::project_version) };

    std::optional<std::string> message;
    app.add_option("-m,--message", message, "A message to print back out");
    bool show_version = false;
    app.add_flag("--version", show_version, "Show version information");

   


    CLI11_PARSE(app, argc, argv);

    if (show_version) {
      fmt::print("{}\n", rtacc::cmake::project_version);
      return EXIT_SUCCESS;
    }

  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
  return 0;
}

//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "line_reader.hpp"
#include "sleep.hpp"
#include "stdin_line_reader.hpp"
#include "buffered_file_ingestor.hpp"
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/use_awaitable.hpp>
#include <iostream>

asio::awaitable<void> do_read(line_reader& reader)
{
  for (int i = 0; i < 10; ++i)
  {
    auto value = co_await reader.async_read_line("Enter something: ", asio::use_awaitable);
    std::cout << value ;
    co_await async_sleep(co_await asio::this_coro::executor, std::chrono::seconds(1), asio::use_awaitable);
  }
}

asio::awaitable<void> read_file(buffered_file_ingestor& reader)
{

  std::string buffer; 
  do
  {
    buffer = co_await reader.read(asio::use_awaitable);
    std::cout << buffer;
  } while (true);
  
}

int main()
{
  asio::io_context ctx{1};
  buffered_file_ingestor bfi{ctx.get_executor(), std::string("/Users/mindera/repositories/rtacc/README.md")};
  
  ctx.run();
}