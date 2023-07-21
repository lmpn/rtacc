#ifndef file_ingestor_HPP
#define file_ingestor_HPP
#include <rtacc/io/file.hpp>
namespace rtacc::io {
std::shared_ptr<file::file_impl> file::create_file_ptr(const std::filesystem::path &path, const std::string &mode)
{
  if (not std::filesystem::exists(path)) { throw std::runtime_error("path: " + path.string() + " not found"); }
  return { std::fopen(path.c_str(), mode.c_str()), std::fclose };
}

file::file(const std::filesystem::path &path, const std::string &mode) : m_file(create_file_ptr(path, mode)) {}

file::file(file &&rhs) noexcept : m_file(std::move(rhs.m_file)){};
file &file::operator=(file &&rhs) noexcept
{
  std::swap(m_file, rhs.m_file);
  rhs.m_file.reset();
  return *this;
};

[[nodiscard]] int file::desc() const { return fileno(m_file.get()); }

}// namespace rtacc::io
#endif