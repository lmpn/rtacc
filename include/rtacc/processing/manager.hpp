#pragma once
#include "rtacc/processing/executor.hpp"
#include "rtacc/processing/node.hpp"
#include <any>
#include <cstdio>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
namespace rtacc::processing {
class manager
{
private:
  using node_id = std::size_t;
  using task = std::function<void()>;
  executor_ptr m_executor;
  std::unordered_map<node_id, node_ptr> m_nodes;

public:
  explicit manager(executor_ptr executor);
  void add_node(const node_ptr &node);
  void remove_node(node_id idx);
  void send(node_id idx, const std::any &msg);
  void wait() { m_executor->wait(); };
};

}// namespace rtacc::processing