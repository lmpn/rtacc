#include <CLI/CLI.hpp>
#include <array>
#include <functional>
#include <iostream>
#include <optional>
#include <spdlog/spdlog.h>


// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `rtacc`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include "asio.hpp"
#include "buffered_file_ingestor.hpp"
#include <cstdio>
#include <internal_use_only/config.hpp>
#include <memory>
#include <rtacc/io/file.hpp>


int main(int argc, const char **argv)
{
  try {
    CLI::App app{ fmt::format("{} version {}", rtacc::cmake::project_name, rtacc::cmake::project_version) };

    std::optional<std::string> file_path;
    app.add_option("-f,--file", file_path, "An input file");
    bool show_version = false;
    app.add_flag("--version", show_version, "Show version information");


    CLI11_PARSE(app, argc, argv);

    if (show_version) {
      fmt::print("{}\n", rtacc::cmake::project_version);
      return EXIT_SUCCESS;
    }

    if (not file_path.has_value()) {
      fmt::print("no file\n");
      return EXIT_SUCCESS;
    }

    constexpr std::size_t size = 128;
    asio::io_context ioc{ 1 };
    rtacc::ingest::buffered_file_ingestor bfi{ ioc.get_executor(), file_path.value(), size };

    bfi.read([](std::string buffer) { fmt::print("{}", buffer); });

    ioc.run();
  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
  return 0;
}
