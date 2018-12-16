#include "lsh.h"
#define PRIME_NUMBER 999983


//genera un codigo de hash por cada fihero de una tira determinada
//values: valor de retorno
//nFiles: numero de ficheros
//s: pos donde empieza la tira
//r: tamaño de la tira
//ns: numero de shingles
void getHashValue(vector<vector<int>>& signatureMatrix, vector<unsigned int>& values, int nFiles, int s, int r, int ns) {
  for(int f=0; f < nFiles; f++) {
    values[f] = 0;
    int a = 1;
    for(int i=0; i < r; i++) {
      values[f] = (values[f] + signatureMatrix[s+i][f]*a) % PRIME_NUMBER;
      a *= ns;
    }
  }
  cout << endl;
}

//indica si dos subvector de los vectors son iguales
//f: numero del fichero a comparar
//f1: numero del fichero a comparar
//s: indice a comenzar a evaluar
//len: numero de elementos del subvector
bool equalVector(vector<vector<int>>& vec, int f, int f1, int s, int len) {
  for(int i=0; i < len; i++) {
    if(vec[s+i][f] != vec[s+i][f1]) return false;
  }
  return true;
}

//Devuelve las possibles parajas de ficheros candidatas a ser similares
//candidates: el valor que devuelve
//bands: numero de tiras (tiene que ser obigatoriamente dividible por el numero de signatures)
void getCandidates(vector<vector<int>>& signatureMatrix, list<pair<int, int>>& candidates, int bands) {
  //optimizamos mucho la cantidad de operaciones en tiras muy largas
  int nSignatures = signatureMatrix.size();
  int nFiles = signatureMatrix[0].size();
  int r = nSignatures/bands;

  //vector que nos indica si una pareja ya es candidata a ser similar o no
  vector<vector<bool>> vis(nFiles, vector<bool> (nFiles, false)); //para optimizar comparaciones
  //recoremos todas las tiras
  for(int i=0; i < nSignatures; i += r) {
    //comparamos todos los ficheros entre ellos con esa tira
    for(int f=0; f < nFiles; f++) {
      for(int f1=f+1; f1 < nFiles; f1++) {
	if(not vis[f][f1] and equalVector(signatureMatrix, f, f1, i, r)) {
	  candidates.push_back(make_pair(f, f1));
	  vis[f][f1] = true;
	}
      }
    }
  }
}

//Devuelve las possibles parajas de ficheros candidatas a ser similares
//candidates: el valor que devuelve
//bands: numero de tiras (tiene que ser obigatoriamente dividible por el numero de signatures)
//nshingles: numero de shingles
void getCandidatesOpt(vector<vector<int>>& signatureMatrix, list<pair<int, int>>& candidates, int bands, int nshingles) {
  //optimizamos mucho la cantidad de operaciones en tiras muy largas
  int nSignatures = signatureMatrix.size();
  int nFiles = signatureMatrix[0].size();
  int r = nSignatures/bands;

  vector<unsigned int> values(nFiles); //vector para guardar los valores de hash
  //vector que nos indica si una pareja ya es candidata a ser similar o no
  vector<vector<bool>> vis(nFiles, vector<bool> (nFiles, false)); //para optimizar comparaciones
  //recoremos todas las tiras
  for(int i=0; i < nSignatures; i += r) {
    //buscamos los valores de hash
    getHashValue(signatureMatrix, values, nFiles, i, r, nshingles);
    //comparamos todos los ficheros entre ellos con esa tira
    for(int f=0; f < nFiles; f++) {
      for(int f1=f+1; f1 < nFiles; f1++) {
	if(not vis[f][f1] and values[f] == values[f1]) {
	  candidates.push_back(make_pair(f, f1));
	  vis[f][f1] = true;
	}
      }
    }
  }
}

