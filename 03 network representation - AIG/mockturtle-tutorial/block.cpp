#include <mockturtle/mockturtle.hpp>
#include <mockturtle/networks/block.hpp>

using namespace mockturtle;

int main() {
  block_network ntk;

  const auto x1 = ntk.create_pi();
  const auto x2 = ntk.create_pi();
  const auto f1 = ntk.create_and(x2, x1);
  const auto carry = ntk.create_ha(x1, x2);
  const auto sum = ntk.next_output_pin(carry);

  ntk.create_po(f1);
  ntk.create_po(sum);
  ntk.create_po(carry);

  write_dot(ntk, "block.dot");

  auto ha_node = ntk.get_node(sum);
  

  std::cout << "half-adder node:\n  output_" << ntk.get_output_pin(carry)
            << " function = "
            << kitty::to_binary(
                   ntk.node_function_pin(ha_node, ntk.get_output_pin(carry))) << " (carry)"

            << "\n  output_" << ntk.get_output_pin(sum) << " function = "
            << kitty::to_binary(
                   ntk.node_function_pin(ha_node, ntk.get_output_pin(sum)))<< " (sum)"
            << std::endl;
}