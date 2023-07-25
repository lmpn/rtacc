#include <CLI/CLI.hpp>
#include <array>
#include <asio/io_context.hpp>
#include <cstddef>
#include <functional>
#include <iostream>
#include <optional>
#include <spdlog/spdlog.h>
#include <unordered_set>


// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `rtacc`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include "asio.hpp"
#include "buffered_file_ingestor.hpp"
#include "rtacc/ingest/ingestor.hpp"
#include <cstdio>
#include <internal_use_only/config.hpp>
#include <memory>
#include <rtacc/io/file.hpp>

#include <algorithm>
#include <ranges>
#include <utility>
#include <variant>

class nil
{
};

class bfi_msg
{
};

using Message = std::variant<nil, std::string, std::size_t, bfi_msg>;


class node;
using node_sptr = std::shared_ptr<node>;

class manager
{
private:
  using node_id = std::size_t;
  std::unordered_map<node_id, node_sptr> m_nodes;
  std::shared_ptr<asio::io_context> m_ioc;
  using index = std::vector<node_sptr>::difference_type;

public:
  explicit manager(std::shared_ptr<asio::io_context> ioc) : m_ioc(std::move(ioc)){};
  void add_node(const node_id idx, const node_sptr &node);
  void remove_node(node_id idx);
  void send(node_id idx, const Message &message);
  void post(const std::function<void()> &job);
};
class node
{
private:
  using connection_id = std::size_t;
  std::unordered_set<connection_id> m_connection_ids;
  std::shared_ptr<manager> m_executor;

protected:
  virtual void start(const Message &message) = 0;
  void finish(const Message &message);


public:
  explicit node(std::shared_ptr<manager> executor) : m_executor(std::move(executor)){};
  node(const node &rhs) = delete;
  node(node &&rhs) = delete;
  node &operator=(const node &rhs) = delete;
  node &operator=(node &&rhs) = delete;

  virtual ~node() = default;
  void connect(const connection_id &identifier);
  void run(const Message &message);
};


void manager::add_node(manager::node_id idx, const node_sptr &node) { m_nodes.emplace(idx, node); }
void manager::remove_node(manager::node_id idx) { m_nodes.erase(idx); }
void manager::send(manager::node_id idx, const Message &message) { m_nodes.at(idx)->run(message); }
void manager::post(const std::function<void()> &job)
{
  asio::post(*m_ioc, [job]() { job(); });
}

void node::finish(const Message &message)
{
  std::for_each(m_connection_ids.begin(), m_connection_ids.end(), [this, message](const auto &identifier) {
    m_executor->send(identifier, message);
  });
};

void node::connect(const connection_id &identifier) { m_connection_ids.emplace(identifier); };
void node::run(const Message &message)
{
  m_executor->post([this, message]() { start(message); });
};

class bfi_node : public node
{
private:
  std::shared_ptr<rtacc::ingest::ingestor> m_ingestor;

protected:
  void start(const Message &message) override;

public:
  bfi_node(std::shared_ptr<manager> manager, std::shared_ptr<rtacc::ingest::ingestor> ingestor);
};

bfi_node::bfi_node(std::shared_ptr<manager> manager, std::shared_ptr<rtacc::ingest::ingestor> ingestor)
  : node(std::move(manager)), m_ingestor(std::move(ingestor))
{}
void bfi_node::start(const Message &message)
{
  if (!std::holds_alternative<bfi_msg>(message)) { return; }
  m_ingestor->read([this](std::string buffer) {
    Message msg = buffer;
    finish(msg);
  });
}

class count_node : public node
{
private:
protected:
  void start(const Message &message) override;

public:
  explicit count_node(std::shared_ptr<manager> manager);
};

count_node::count_node(std::shared_ptr<manager> manager) : node(std::move(manager)) {}
void count_node::start(const Message &message)
{
  if (!std::holds_alternative<std::string>(message)) { return; }
  Message msg = std::get<std::string>(message).length();
  finish(msg);
}

class log_node : public node
{
private:
protected:
  void start(const Message &message) override;

public:
  explicit log_node(std::shared_ptr<manager> manager);
};

log_node::log_node(std::shared_ptr<manager> manager) : node(std::move(manager)) {}
void log_node::start(const Message &message)
{
  if (!std::holds_alternative<std::size_t>(message)) { return; }
  spdlog::info("{}", std::get<std::size_t>(message));
}

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
    auto ioc_ptr = std::make_shared<asio::io_context>(1);
    auto m_ptr = std::make_shared<manager>(ioc_ptr);
    auto bfi_ptr =
      std::make_shared<rtacc::ingest::buffered_file_ingestor>(ioc_ptr->get_executor(), file_path.value(), size);
    auto nbfi_ptr = std::make_shared<bfi_node>(m_ptr, bfi_ptr);
    m_ptr->add_node(0, nbfi_ptr);

    auto count_ptr = std::make_shared<count_node>(m_ptr);
    m_ptr->add_node(1, count_ptr);

    auto log_ptr = std::make_shared<log_node>(m_ptr);
    m_ptr->add_node(2, log_ptr);

    nbfi_ptr->connect(1);
    count_ptr->connect(2);
    m_ptr->send(0, bfi_msg{});
    ioc_ptr->run();
  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
  return 0;
}
