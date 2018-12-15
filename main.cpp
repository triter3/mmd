#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include "jaccard.h"
#include "decode.h"
#include "minhash.h"
using namespace std;

void printMatrix(const map<string, vector<bool>>& matrix, int k) {
  int l = k*8;
  for(auto p : matrix) {
    printf("%-*s", l, p.first.c_str());
    for(bool b : p.second) {
      printf("%-4d", (int) b);
    }
    printf("\n");
  }
}

void printDoubleMatrix(const vector<vector<double>>& vec) {
  char s[12];
  printf("%-12s", "");
  for(int i=0; i < vec[0].size(); i++) {
    sprintf(s, "File %d", (i+1));
    printf("%-12s", s);
  }
  printf("\n");
  for(int i=0; i < vec.size(); i++) {
    sprintf(s, "File %d", (i+1));
    printf("%-12s", s);
    for(int j=0; j <= i; j++) {
      printf("%-12f", vec[i][j]);
    }
    printf("\n");
  }
}

void printSignatureMatrix(const vector<vector<int>> &vec) {
  cout << "Signature Matrix: " << endl;
  for(int i=0; i < vec.size(); i++) {
    for(int j=0; j < vec[i].size(); j++) {
      printf("%-5d", vec[i][j]);
    }
    printf("\n");
  }
}

int main() {
  //Lectura datos iniciales
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
  
  //Preparar matriz shingles
  map<string, vector<bool>> matrix;
  parse (matrix, k ,filenames);
  printMatrix(matrix, k);

  //Calcular Jaccard a partir de la matriz
  cout << endl;
  vector<vector<double>> jaccard(nFiles, vector<double>(nFiles, 0.0d));
  compareAll(matrix, jaccard);
  printDoubleMatrix(jaccard);  
  
  //MinHash
  //Cálculo signature matrix
  cout << endl << "Número de funciones de hash para MinHash: ";
  int t;
  cin >> t;
  cout << endl << endl;
  vector<vector<int>> signatureMatrix(t, vector<int>(nFiles, -1));
  generateSignatureMatrix(matrix, t, signatureMatrix, nFiles);
  printSignatureMatrix(signatureMatrix);
  
  //Cálculo similaridad de signatures
  vector<vector<double>> signatureSimilarity(nFiles, vector<double>(nFiles, 0.0d));
  generateSignatureSimilarity(signatureMatrix, signatureSimilarity, t);
  cout << endl << "Signature Similarity: " << endl;
  printDoubleMatrix(signatureSimilarity);
}
