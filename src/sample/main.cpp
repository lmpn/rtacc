#include <CLI/CLI.hpp>
#include <array>
#include <functional>
#include <iostream>
#include <optional>
#include <spdlog/spdlog.h>


// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `rtacc`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <asio/io_context.hpp>
#include <asio/posix/stream_descriptor.hpp>
#include <asio/stream_file.hpp>
#include <internal_use_only/config.hpp>
#include <stdio.h>


class file
{
private:
  using file_impl = std::FILE *;
  using file_impl_ptr = std::shared_ptr<file_impl>;
  file_impl_ptr m_file;
  file(const file &) = delete;
  file &operator=(const file &) = delete;

  static create_file_ptr(const std::string &path, const std::string &mode = std::string("r"))
  {
    auto fp = std::fopen(path, mode);
    return fp ? std::shared_ptr<std::FILE>(fp, std::fclose) : std::shared_ptr<std::FILE>();
  }

public:
  explicit file(const std::string &path, const std::string &mode = std::string("r"))
    : m_file(create_file_ptr(path, mode))
  {}

  auto desc() const { return m_file->_file; }

  ~file()
  {
    if (m_file) { std::fclose(m_file); }
  }
};


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

    constexpr std::size_t size = 1024;
    asio::io_context ioc{ 1 };
    file fd_ = file(file_path->c_str(), "r");
    auto fd_ptr = std::make_shared<FILE>() asio::posix::stream_descriptor stream_(ioc, fd_);
    std::string buffer_;
    buffer_.resize(size);

    fclose(fd_);

  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
  return 0;
}
