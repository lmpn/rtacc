#include "rtacc/processing/manager.hpp"
#include <iostream>
namespace rtacc::processing {
manager::manager(executor_ptr executor) : m_executor(std::move(executor)), m_nodes({}){};
void manager::add_node(const node_ptr &node) { m_nodes.emplace(node->get_id(), node); };
void manager::remove_node(node_id idx) { m_nodes.erase(idx); };
void manager::send(node_id idx, const std::any &msg)
{
  auto node = m_nodes.at(idx);
  m_executor->post([node, msg]() { node->start(msg); });
};
}// namespace rtacc::processing