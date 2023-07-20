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
#include <internal_use_only/config.hpp>
#include <stdio.h>
#include <memory>

class file
{
private:
  using file_impl = std::FILE;
  using file_impl_ptr = std::FILE*;
  using file_impl_sptr = std::shared_ptr<file_impl>;
  file_impl_sptr m_file;
  file(const file &) = delete;
  file &operator=(const file &) = delete;

  static std::shared_ptr<file_impl> create_file_ptr(const std::string &path, const std::string &mode = std::string("r"))
  {
    file_impl_ptr fp = std::fopen(path.c_str(), mode.c_str());
    
    return std::shared_ptr<file_impl>(std::move(fp), std::fclose);
  }

public:
  explicit file(const std::string &path, const std::string &mode = std::string("r"))
    : m_file(create_file_ptr(path, mode))
  {}

  auto desc() const { std::cout << m_file->_file<<"\n"; return fileno(m_file.get())  ; }
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
    asio::posix::stream_descriptor stream_(ioc, fd_.desc());
    std::string buffer_;
    buffer_.resize(size);

    asio::async_read(stream_, asio::buffer(buffer_, size), [&buffer_](std::error_code ec, std::size_t length){
      fmt::print("buffer: \n {}\n", buffer_);
      fmt::print("ec: {} len: {}\n", ec.message(), length);
    });
    ioc.run();
  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
  return 0;
}
