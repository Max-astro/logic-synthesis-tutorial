#include <mockturtle/mockturtle.hpp>

using namespace mockturtle;
// using namespace std;

int main() {
  klut_network klut;

  const auto a = klut.create_pi();
  const auto b = klut.create_pi();
  const auto c = klut.create_pi();

  kitty::dynamic_truth_table tt_maj(3u), tt_xor(3u), tt_complex(5u);
  kitty::create_from_hex_string(tt_maj, "e8");
  kitty::create_from_hex_string(tt_xor, "96");
  kitty::create_from_hex_string(tt_complex, "abcdefef");

  const auto maj_node = klut.create_node({a, b, c}, tt_maj);
  const auto xor_node = klut.create_node({a, b, c}, tt_xor);

  const auto e = klut.create_pi();
  const auto const0 = klut.get_constant(false);
  const auto const1 = klut.get_constant(true);
  const auto complex_node =
      klut.create_node({maj_node, xor_node, e, const0, const1}, tt_complex);

  klut.create_po(complex_node);

  write_dot(klut, "klut.dot");

  std::cout << "complex_node's fanin:\n";
  klut.foreach_fanin(complex_node, [&](auto node, auto idx) {
    std::cout << "  Input_" << idx << " = node_" << node << ", input node function = "
              << kitty::to_binary(klut.node_function(node)) << std::endl;
  });
  std::cout << "---------------------\n\n";
}