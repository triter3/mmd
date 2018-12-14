#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include "jaccard.h"
#include "decode.h"
using namespace std;

void printMatrix(const map<string, vector<bool>>& matrix, int k) {
  for(auto p : matrix) {
    int l = k*8;
    printf("%-*s", l, p.first.c_str());
    for(bool b : p.second) {
      printf("%-4d", (int) b);
    }
    printf("\n");
  }
}

void printJaccard(const vector<vector<double>>& vec) {
  for(int i=0; i < vec.size(); i++) {
    for(int j=0; j <= i; j++) {
      printf("%-12f", vec[i][j]);
    }
    printf("\n");
  }
}

int main() {
  cout << "Numero de ficheros: ";
  int nFiles;
  cin >> nFiles;
  
  vector<string> filenames(nFiles);
  for(int i =0; i < nFiles; i++) {
    cout << "Nombre fichero " << i+1 << ": ";
    cin >> filenames[i];
  }

  int k;
  cout << "k-shingles? ";
  cin >> k;
  cout << endl;
  
  map<string, vector<bool>> matrix;
  parse (matrix, k ,filenames);
  printMatrix(matrix, k);

  cout << endl;
  vector<vector<double>> jaccard(nFiles, vector<double>(nFiles, 0.0d));
  compareAll(matrix, jaccard);
  printJaccard(jaccard);  
  
}
