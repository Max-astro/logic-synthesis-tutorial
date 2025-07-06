#include "nqueen.h"
#include <cstddef>

using namespace std;

NQueensSolver::NQueensSolver(int n) : board(n, vector<BDD>(n, BDD{})) {
  // Fill up with bdd variables
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      board[i][j] = mgr.bddVar();
    }
  }
}

/* ------------------- Core algorithm ---------------------- */
BDD NQueensSolver::QueenExistConstrain(int x, int y) {
  // If baord[x][y] is a queen:
  BDD bdd = board[x][y];

  // 1. No other queen can be positioned on the same row
  for (int j = 0; j < board.size(); j++) {
    if (j == y) {
      continue;
    }
    bdd = bdd.And(!board[x][j]);
  }

  // 2. Can not be positioned on the same column
  for (int i = 0; i < board.size(); i++) {
    if (i == x) {
      continue;
    }
    bdd = bdd.And(!board[i][y]);
  }

  // 3. Or any of the diagonals.
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board.size(); j++) {
      if (i == x && j == y) {
        continue;
      }
      if (std::abs(x - i) == std::abs(y - j)) {
        bdd = bdd.And(!board[i][j]);
      }
    }
  }

  return bdd;
}

// Each row should at least placed 1 queen
BDD NQueensSolver::RowConstrain(int row) {
  BDD bdd = QueenExistConstrain(row, 0);
  for (int j = 1; j < board.size(); j++) {
    bdd = bdd.Or(QueenExistConstrain(row, j));
  }
  return bdd;
}

// Combine all constrains for each positions
BDD NQueensSolver::BoardConstrain() {
  BDD bdd = RowConstrain(0);
  for (int i = 1; i < board.size(); i++) {
    bdd = bdd.And(RowConstrain(i));
  }
  return bdd;
}
/* ------------------------------------------------------ */

void NQueensSolver::EvalAndPrint() {
  BDD root = BoardConstrain();
  DumpDot(root);
  PrintAllSolutions(root);
}

size_t NQueensSolver::GetSolutionCount() {
  int n = board.size();
  BDD root = BoardConstrain();
  return root.CountMinterm(n * n);
}

void NQueensSolver::PrintAllSolutions(BDD root) {
  int n = board.size();
  DdGen *gen;
  int cube[n * n];
  int *p = cube;
  memset(p, 0, n * n * sizeof(int));

  CUDD_VALUE_TYPE val;

  size_t sat = 0;
  cout << endl;
  // Initialize the generator
  Cudd_ForeachCube(mgr.getManager(), root.getNode(), gen, p, val) {
    cout << "Solution " << sat++ << ":\n";
    PrintBoard(p);
  }

  cout << "\n" << n << "-queens problem has " << sat << " solutions." << endl;
}

void NQueensSolver::DumpDot(BDD bdd) {
  FILE *dotFile = fopen("bdd-graph.dot", "w");
  if (dotFile == NULL) {
    std::cerr << "Error opening file for writing." << std::endl;
  }

  // Create an array of BDD nodes to dump
  std::vector<BDD> bddArray;
  bddArray.push_back(bdd);

  // Dump the BDD to the .dot file
  mgr.DumpDot(bddArray, NULL, NULL, dotFile);

  // Close the file
  fclose(dotFile);

  std::cout << "BDD graph dumped to bdd-graph.dot" << std::endl;
}

void NQueensSolver::PrintBoard(int *assign) {
  int n = board.size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int pos = i * n + j;
      if (assign[pos]) {
        cout << "Q ";
      } else {
        cout << "_ ";
      }
    }
    cout << endl;
  }
  cout << endl;
}

#include "answer.h"
#include <chrono>
using namespace std::chrono;

int main(int argc, char *argv[]) {
  // Print all 8-queens problem solutions
  NQueensSolver queen(8);
  queen.EvalAndPrint();

  // Benchmarking CUDD solve N-queens problem
  for (int i = 1; i <= 20; i++) {
    auto t0 = high_resolution_clock::now();

    NQueensSolver queen(i);
    size_t cnt = queen.GetSolutionCount();

    auto t1 = high_resolution_clock::now();
    double time = duration_cast<milliseconds>(t1 - t0).count();

    cout << "\n"
         << i << "-Queens problem has: " << cnt
         << " solutions, ref = " << nqueen_expect[i]
         << ". Elapsed time = " << time << " ms" << endl;
  }

  return 0;
}
