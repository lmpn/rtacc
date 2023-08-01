#include <any>
#include <memory>

namespace rtacc {
namespace processing {

  class manager;
  class node;

}// namespace processing
using RtaccMessage = std::any;
using RtaccManager = processing::manager;
using RtaccManagerPtr = std::shared_ptr<RtaccManager>;
using RtaccNodeId = std::size_t;
using RtaccNode = processing::node;
}// namespace rtacc
