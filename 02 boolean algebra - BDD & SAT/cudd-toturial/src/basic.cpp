#include "cuddObj.hh"
#include <bitset>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>

using namespace std;

int main() {
  Cudd manager(0, 0);

  auto x1 = manager.bddVar();
  auto x2 = manager.bddVar();
  auto x3 = manager.bddVar();
  
  printf("BDD node x1: %d, x2: %d, x3: %d\n", x1.NodeReadIndex(),
         x2.NodeReadIndex(), x3.NodeReadIndex());

  // Half-adder
  auto xor1 = x1.Xor(x2);
  auto fa_sum = xor1.Xor(x3);
  auto fa_cout = x1.And(x2).Or(xor1.And(x3));

  // Represent sum in CNF
  auto x1x2x3 = x1.And(x2).And(x3);
  auto n1n2x3 = (!x1).And(!x2).And(x3);
  auto n1x2n3 = (!x1).And(x2).And(!x3);
  auto x1n2n3 = x1.And(!x2).And(!x3);
  auto expr = x1x2x3.Or(n1n2x3).Or(n1x2n3).Or(x1n2n3);

  // Equivalent check
  auto eq_check = fa_sum.Xnor(expr);
  if (eq_check.IsOne()) {
    cout << "EQ\n" << endl;
  } else {
    cout << "NEQ\n" << endl;
  }

  assert(fa_sum == expr);

  // Evaluate boolean expression value
  printf("x1 x2 x3 | C S\n");
  for (size_t i = 0; i < (1 << 3); i++) {
    bitset<3> bs{i};
    int vec[3] = {bs[0], bs[1], bs[2]};
    auto sum_value = fa_sum.Eval(vec);
    auto carry_value = fa_cout.Eval(vec);
    printf(" %d  %d  %d | %d %d\n", vec[0], vec[1], vec[2], carry_value.IsOne(),
           sum_value.IsOne());
  }

  {
    FILE *dotFile = fopen("full_adder_bdd.dot", "w");
    vector<BDD> bv{fa_cout, fa_sum};
    // vector<BDD> bv{fa_sum};
    manager.DumpDot(bv, nullptr, nullptr, dotFile);
    fclose(dotFile);
  }

  return 0;
}
