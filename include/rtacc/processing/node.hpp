#pragma once
#include <any>
#include <cstddef>
#include <memory>
#include <unordered_set>
namespace rtacc::processing {
class manager;

using manager_ptr = std::shared_ptr<manager>;

class node
{
private:
  using connection_id = std::size_t;
  using node_id = std::size_t;
  std::unordered_set<connection_id> m_connection_ids;
  node_id m_id;

protected:
  void finish(const std::any &msg);
  manager_ptr m_executor;

public:
  explicit node(node_id identifier, manager_ptr executor) : m_id(identifier), m_executor(std::move(executor)){};
  node(const node &rhs) = delete;
  node(node &&rhs) = delete;
  node &operator=(const node &rhs) = delete;
  node &operator=(node &&rhs) = delete;

  virtual ~node() = default;
  virtual void start(const std::any &msg) = 0;
  void connect(const connection_id &identifier);
  [[nodiscard]] node_id get_id() const;
};

using node_ptr = std::shared_ptr<node>;

}// namespace rtacc::processing