#include <any>
#include <iostream>
#include <rtacc/processing/manager.hpp>
#include <rtacc/processing/node.hpp>
namespace rtacc::processing {

void node::finish(const std::any &msg)
{
  std::for_each(m_connection_ids.begin(), m_connection_ids.end(), [this, msg](const auto &identifier) {
    m_executor->send(identifier, msg);
  });
};

void node::connect(const connection_id &identifier) { m_connection_ids.emplace(identifier); };
node::node_id node::get_id() const { return m_id; };

}// namespace rtacc::processing