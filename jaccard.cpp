#include "jaccard.h"

void compareAll(map<string, vector<bool>>& matrix, vector<vector<double>>& res) {
  for(int i=0; i < res.size(); i++) {
    for(int j=0; j <= i; j++) {
      if(i == j) res[i][j] = 1.0d;
      else res[i][j] = compareFiles(matrix, i, j);
    }
  }
}

double compareFiles(map<string, vector<bool>>& matrix, int f1, int f2) {
  int tipo1 = 0; int tipo2 = 0;
  for(auto p : matrix) {
    if(p.second[f1] and p.second[f2]) tipo1++;
    else if(p.second[f1] or p.second[f2]) tipo2++;
  }

  return (double) tipo1/(tipo1+tipo2);
}
