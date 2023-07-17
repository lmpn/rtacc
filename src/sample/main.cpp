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

int main(int argc, const char **argv)
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
}