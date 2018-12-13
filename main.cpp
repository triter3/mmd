#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include "decode.h"
using namespace std;

void printMatrix(const map<string, vector<bool>>& matrix) {
  for(auto p : matrix) {
    cout << p.first;
    for(bool b : p.second) {
      cout << "     " << b;
    }
    cout << endl;
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
  
  map<string, vector<bool>> matrix;
  parse (matrix, k ,filenames);
  printMatrix(matrix);
  
}
