#include <mockturtle/mockturtle.hpp>

using namespace mockturtle;

template <typename T> void print_cut(const T &cut) {
  std::cout << "{ ";
  bool first = true;
  for (auto cut_node : cut) {
    if (!first) {
      std::cout << ", ";
    }
    std::cout << cut_node;
    first = false;
  }
  std::cout << " } ";
}

template <typename Ntk>
void display_cut_enum(const Ntk &ntk, const std::string &ntk_type) {
  cut_enumeration_params ps =
      cut_enumeration_params{4, 25, 10, true, false, false};

  std::cout << "Cut enumeration algorithm apply in \033[31m" << ntk_type
            << "\033[0m:\n";
  auto cut_mgr = cut_enumeration(ntk, ps);
  ntk.foreach_node([&](auto node) {
    if (ntk.is_constant(node)) {
      return;
    }

    auto node_idx = ntk.node_to_index(node);
    auto const &cut_set = cut_mgr.cuts(node_idx);
    std::cout << "  node_" << node_idx << " has " << cut_set.size()
              << " cuts: ";
    for (auto &&cut : cut_set) {
      print_cut(*cut);
    }
    std::cout << "\n";
  });

  std::cout << "------------------------\n\n";
}

int main() {
  kitty::dynamic_truth_table table(6u);
  kitty::create_from_expression(table, "{(((a)(bcd))(a[ef]))}");

  klut_network klut;
  const auto x1 = klut.create_pi();
  const auto x2 = klut.create_pi();
  const auto x3 = klut.create_pi();
  const auto x4 = klut.create_pi();
  const auto x5 = klut.create_pi();
  const auto x6 = klut.create_pi();

  auto fn = [&](kitty::dynamic_truth_table const &remainder,
                std::vector<klut_network::signal> const &children) {
    return klut.create_node(children, remainder);
  };

  klut.create_po(dsd_decomposition(klut, table, {x1, x2, x3, x4, x5, x6}, fn));

  aig_network aig;
  xag_network xag;
  mig_network mig;
  xmg_network xmg;

  aig = convert_klut_to_graph<aig_network>(klut);
  xag = convert_klut_to_graph<xag_network>(klut);
  mig = convert_klut_to_graph<mig_network>(klut);
  xmg = convert_klut_to_graph<xmg_network>(klut);

  write_dot(klut, "dsd_klut.dot");
  write_dot(aig, "dsd_aig.dot");
  write_dot(xag, "dsd_xag.dot");
  write_dot(mig, "dsd_mig.dot");
  write_dot(xmg, "dsd_xmg.dot");

  /*--------- Cut enumeration ----------*/
  display_cut_enum(aig, "aig_network");
  display_cut_enum(xag, "xag_network");
  display_cut_enum(mig, "mig_network");
  display_cut_enum(xmg, "xmg_network");
  display_cut_enum(klut, "klut_network");
}