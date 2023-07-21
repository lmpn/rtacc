#ifndef file_HPP
#define file_HPP
#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>
namespace rtacc::io {

class file
{

public:
  using file_impl = std::FILE;
  using file_impl_ptr = std::FILE *;
  using file_impl_sptr = std::shared_ptr<file_impl>;

  explicit file(const std::filesystem::path &path, const std::string &mode = std::string("r"));
  ~file() = default;

  file(const file &) = delete;
  file &operator=(const file &) = delete;

  file(file &&rhs) noexcept;
  file &operator=(file &&rhs) noexcept;

  [[nodiscard]] int desc() const;

private:
  file_impl_sptr m_file;

  static std::shared_ptr<file_impl> create_file_ptr(const std::filesystem::path &path,
    const std::string &mode = std::string("r"));
};

}// namespace rtacc::io
#endif