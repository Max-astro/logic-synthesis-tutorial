#include "cuddObj.hh"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <vector>

class NQueensSolver {
public:
  using BddBoard = std::vector<std::vector<BDD>>;

  NQueensSolver(int n);
  size_t GetSolutionCount();

  void EvalAndPrint();
  void PrintAllSolutions(BDD root);
  void DumpDot(BDD bdd);

private:
  BDD QueenExistConstrain(int x, int y);
  BDD RowConstrain(int row);
  BDD BoardConstrain();

  void PrintBoard(int *assign);

private:
  Cudd mgr{};
  BddBoard board;
};