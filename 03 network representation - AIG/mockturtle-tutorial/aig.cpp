#include <mockturtle/mockturtle.hpp>

#include <mockturtle/io/write_aiger.hpp>
#include <mockturtle/io/write_dot.hpp>
#include <mockturtle/networks/aig.hpp>
#include <mockturtle/views/topo_view.hpp>

using namespace mockturtle;
// using namespace std;

int main() {
  aig_network aig;
  auto const x1 = aig.create_pi();
  auto const x2 = aig.create_pi();
  auto const x3 = aig.create_pi();

  auto const n4 = aig.create_and(!x1, x2);
  auto const n5 = aig.create_and(x1, n4);
  auto const n6 = aig.create_and(x3, n5);
  auto const n7 = aig.create_and(n4, x2);
  auto const n8 = aig.create_and(!n5, !n7);
  auto const n9 = aig.create_and(!n8, n4);

  aig.create_po(n6);
  aig.create_po(n9);

  std::cout << aig.get_node(x1) << std::endl;
  std::cout << aig.get_node(x2) << std::endl;
  std::cout << aig.get_node(x3) << std::endl;
  std::cout << aig.get_node(n4) << std::endl;
  std::cout << aig.get_node(n5) << std::endl;
  std::cout << aig.get_node(n6) << std::endl;
  std::cout << aig.get_node(n7) << std::endl;
  std::cout << aig.get_node(n8) << std::endl;
  std::cout << aig.get_node(n9) << std::endl;

  write_aiger(aig, "tiny.aiger");
  write_dot(aig, "tiny.dot");

  /* ---------- Duplicate node Test ----------- */
  std::cout << "before dup node 7: node number = " << aig.size() << "\n";
  auto const n7_dup = aig.create_and(n4, x2);
  std::cout << "after dup node 7: node number = " << aig.size() << "\n";
  std::cout << "---------------------\n\n";

  assert(n7_dup == n7);
  assert(aig.get_node(n7_dup) == aig.get_node(n7));
  write_dot(aig, "dup.dot");
  /* --------------------- */

  /* ---------- Access all nodes in AIG ----------- */
  std::cout << "foreach node:\n";
  aig.foreach_node([&](node<aig_network> node, auto idx) {
    std::cout << "Node = " << node << ", index = " << idx
              << ", node_to_index = " << aig.node_to_index(node) << std::endl;
  });
  std::cout << "---------------------\n\n";
  /* --------------------- */

  /* ---------- Access all gates in AIG ----------- */
  std::cout << "foreach gate:\n";
  aig.foreach_gate([&](node<aig_network> node, auto idx) {
    std::cout << "Node = " << node << ", gate_index = " << idx
              << ", node_to_index = " << aig.node_to_index(node) << std::endl;
  });
  std::cout << "---------------------\n\n";
  /* --------------------- */

  // topo_view topo(aig);

  // auto gate = topo.num_gates();
  // std::cout << "Num of gate = " << gate << std::endl;
  // topo.foreach_gate_reverse([&](auto node, auto idx) {
  //   std::cout << node << " " << idx << std::endl;
  // });
}