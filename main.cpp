#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <list>
#include <algorithm>
#include <chrono>
#include <dirent.h>
#include "jaccard.h"
#include "decode.h"
#include "minhash.h"
#include "lsh.h"
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

void printDoubleMatrix(const vector<vector<double>>& vec, const vector<string>& filenames) {
  for(int i=0; i < filenames.size(); ++i)
      cout << "File " << (i+1) << " = " << filenames[i] << endl;
  cout << endl;
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

void printCandidates(const list<pair<int, int>>& list, const vector<double>& res) {
  auto itr = list.begin();
  int i=0;
  for(; itr != list.end(); itr++) {
    cout << "(doc: " << itr->first << " / doc: " << itr->second << ") similarity: ";
    cout << res[i] << endl;
    i++;
  }
}

void genSimilarityCandidates(vector<vector<int>>& signatureMatrix, int t, int nFiles, const vector<vector<bool>> candidates, vector<vector<double>>& res) {
  for(int f=0; f < nFiles; f++) {
    for(int f1=f; f1 < nFiles; f1++) {
      if (f == f1) res[f1][f] = 1.0;
      else if (candidates[f][f1]) res[f1][f] = compareFilesHash(signatureMatrix, f, f1, t); 
    }
  }
}

void getDir(string dir, vector<string>& files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "No se puede abrir o no se encuentra: " << dir << endl;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if (string(dirp->d_name) != "." && string(dirp->d_name) != "..")
            files.push_back(dir + "/" + string(dirp->d_name));
    }
    closedir(dp);
}

int main() {
  //Lectura datos iniciales
  cout << "Carpeta con documentos (path): ";
  string dir;
  cin >> dir;
  vector<string> filenames(0);
  getDir(dir, filenames);
  int nFiles = filenames.size();

  cout << "Documentos a analizar:" << endl;
  for (int i = 0; i < nFiles; ++i) {
    cout << " " << filenames[i];
  }
  cout << endl << endl;

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
  //Cronometramos tiempo de generar la similitud de jaccard por todas las parejas de ficheros posibles
  auto start = chrono::high_resolution_clock::now();
  compareAll(matrix, jaccard);
  chrono::duration<double> jaccardShinglesTime = chrono::high_resolution_clock::now() - start;
  printDoubleMatrix(jaccard, filenames);

  //MinHash
  //Cálculo signature matrix
  cout << endl << "Número de funciones de hash para MinHash: ";
  int t;
  cin >> t;
  cout << endl << endl;
  vector<vector<int>> signatureMatrix(t, vector<int>(nFiles, -1));
  //Cronometramos el tiempo que tarda en generar la matriz de signaturas
  start = chrono::high_resolution_clock::now();
  generateSignatureMatrix(matrix, t, signatureMatrix, nFiles);
  chrono::duration<double> generateSignatureTime = chrono::high_resolution_clock::now() - start;
  printSignatureMatrix(signatureMatrix);

  //Cálculo similaridad de signatures
  vector<vector<double>> signatureSimilarity(nFiles, vector<double>(nFiles, 0.0d));
  //Cronometramos el tiempo de generar la similitud de jaccard con la matriz de minhash por todas las parejas de ficheros posibles
  start = chrono::high_resolution_clock::now();
  generateSignatureSimilarity(signatureMatrix, signatureSimilarity, t);
  chrono::duration<double> jaccardMinHasTime = chrono::high_resolution_clock::now() - start;
  cout << endl << "Signature Similarity: " << endl;
  printDoubleMatrix(signatureSimilarity, filenames);

  //lsh
  //buscamos posibles candidatos para ser comparados
  cout << endl << "Número de tiras (tiene que ser divisor de " << t << "): ";
  int bands;
  while(cin >> bands and t%bands != 0)
  cout << endl << "Número de tiras (tiene que ser divisor de " << t << "): ";
  cout << endl;
  vector<vector<bool>> candidates(nFiles, vector<bool> (nFiles, false));
  vector<vector<double>> signatureSimilarityLsh(nFiles, vector<double>(nFiles, 0.0d));
  //Cronometramos el lsh
  start = chrono::high_resolution_clock::now();
  getCandidates(signatureMatrix, candidates, bands);
  genSimilarityCandidates(signatureMatrix, t, nFiles, candidates, signatureSimilarityLsh);
  chrono::duration<double> similarityWithLshTime = chrono::high_resolution_clock::now() - start;
  cout << "candidatos con lsh: " << endl;
  printDoubleMatrix(signatureSimilarityLsh, filenames);
  cout << endl;

  vector<vector<bool>> candidates1(nFiles, vector<bool> (nFiles, false));
  vector<vector<double>> signatureSimilarityLshOpt(nFiles, vector<double>(nFiles, 0.0d));
  //Cronometramos el lsg optimizado
  start = chrono::high_resolution_clock::now();
  getCandidatesOpt(signatureMatrix, candidates1, bands, matrix.size());
  genSimilarityCandidates(signatureMatrix, t, nFiles, candidates1, signatureSimilarityLshOpt);
  chrono::duration<double> similarityWithLshOptTime = chrono::high_resolution_clock::now() - start;
  cout << "candidatos con lsh optimizado: " << endl;
  printDoubleMatrix(signatureSimilarityLshOpt, filenames);
  cout << endl;

  //pintamos los tiempos
  cout << "tiempos:" << endl;
  cout << " Jaccard con shingles: " << jaccardShinglesTime.count() << "s" << endl;
  cout << " Generar signaturas con minhash: " << generateSignatureTime.count() << "s" << endl;
  cout << " Jaccard con minhash: " << jaccardMinHasTime.count() << "s" << endl;
  cout << " LSH: " << similarityWithLshTime.count() << "s" << endl;
  cout << " LSH con optimizacion: " << similarityWithLshOptTime.count() << "s" << endl;
}
