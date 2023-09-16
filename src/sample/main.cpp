#include "rtacc/ingest/ingestor.hpp"
#include "rtacc/ingest/ingestor_factory.hpp"
#include "rtacc/processing/definitions.hpp"
#include "rtacc/processing/executor_factory.hpp"
#include "rtacc/processing/node.hpp"
#include <AudioFile.h>
#include <CLI/CLI.hpp>
#include <algorithm>
#include <any>
#include <array>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <internal_use_only/config.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <ranges>
#include <rtacc/processing/definitions.hpp>
#include <rtacc/processing/manager.hpp>
#include <spdlog/spdlog.h>
#include <thread>
#include <typeindex>
#include <unordered_set>
#include <utility>
#include <variant>
class nil
{
};

class bfi_msg
{
};


// class node;


class bfi_node : public rtacc::RtaccNode
{
private:
  std::shared_ptr<rtacc::ingest::ingestor> m_ingestor;

protected:
  void start(const rtacc::RtaccMessage &message) override;

public:
  bfi_node(rtacc::RtaccNodeId identifier,
    const rtacc::RtaccManagerPtr &manager,
    std::shared_ptr<rtacc::ingest::ingestor> ingestor);
};

bfi_node::bfi_node(rtacc::RtaccNodeId identifier,
  const rtacc::RtaccManagerPtr &manager,
  std::shared_ptr<rtacc::ingest::ingestor> ingestor)
  : rtacc::RtaccNode(identifier, manager), m_ingestor(std::move(ingestor))
{}
void bfi_node::start(const rtacc::RtaccMessage &message)
{

  if (typeid(bfi_msg) != message.type()) { return; }
  m_ingestor->read([this](std::error_code ecode, const std::string &buffer) {
    if (!buffer.empty()) {
      const rtacc::RtaccMessage msg = buffer;
      m_executor->send(0, bfi_msg{});
      finish(msg);
    }

    if (ecode.value() != 0) { return; }
  });
}

class count_node : public rtacc::RtaccNode
{
private:
protected:
  void start(const rtacc::RtaccMessage &message) override;

public:
  explicit count_node(rtacc::RtaccNodeId identifier, rtacc::RtaccManagerPtr manager);
};

count_node::count_node(rtacc::RtaccNodeId identifier, rtacc::RtaccManagerPtr manager)
  : rtacc::RtaccNode(identifier, std::move(manager))
{}
void count_node::start(const rtacc::RtaccMessage &message)
{
  if (typeid(std::string) != message.type()) { return; }
  const rtacc::RtaccMessage msg = std::any_cast<std::string>(message).length();
  finish(msg);
}

class log_node : public rtacc::RtaccNode
{
private:
  std::size_t ctr{ 0 };

protected:
  void start(const rtacc::RtaccMessage &message) override;

public:
  explicit log_node(rtacc::RtaccNodeId identifier, rtacc::RtaccManagerPtr manager);
};

log_node::log_node(rtacc::RtaccNodeId identifier, rtacc::RtaccManagerPtr manager)
  : rtacc::RtaccNode(identifier, std::move(manager))
{}
void log_node::start(const rtacc::RtaccMessage &message)
{
  if (typeid(std::size_t) != message.type()) { return; }
  auto value = std::any_cast<std::size_t>(message);
  ctr += value;
  spdlog::info("{}", ctr);
  std::cout << value << std::endl;
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

    constexpr size_t size = 128;
    auto executor_ptr = rtacc::processing::executor_factory::create_asio_thread_pool_executor(2);
    const rtacc::RtaccManagerPtr manager = std::make_shared<rtacc::RtaccManager>(executor_ptr);
    auto bfi_ptr = rtacc::ingest::ingestor_factory::create_buffered_file_ingestor(file_path.value(), size);
    auto nbfi_ptr = std::make_shared<bfi_node>(0, manager, bfi_ptr);
    manager->add_node(nbfi_ptr);

    auto count_ptr = std::make_shared<count_node>(1, manager);
    manager->add_node(count_ptr);

    auto log_ptr = std::make_shared<log_node>(2, manager);
    manager->add_node(log_ptr);

    nbfi_ptr->connect(1);
    count_ptr->connect(2);
    manager->send(0, bfi_msg{});

    manager->wait();
    AudioFile<double> audioFile;
  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
  return 0;
}
