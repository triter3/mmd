#include "lsh.h"
#define PRIME_NUMBER 999983


//indica si todas las signaturas de una tira en los dos ficheros son iguales
//f: numero del fichero a comparar
//f1: numero del fichero a comparar
//s: indice para comenzar a evaluar
//len: numero de elementos del subvector
inline bool equalVector(vector<vector<int>>& vec, int f, int f1, int s, int len) {
  for(int i=0; i < len; i++) {
    if(vec[s+i][f] != vec[s+i][f1]) return false;
  }
  return true;
}

//Devuelve las possibles parajas de ficheros candidatas a ser similares
//candidates: el valor que devuelve
//bands: numero de tiras (tiene que ser obigatoriamente dividible por el numero de signatures)
void getCandidates(vector<vector<int>>& signatureMatrix, vector<vector<bool>>& candidates, int bands) {
  int nSignatures = signatureMatrix.size();
  int nFiles = signatureMatrix[0].size();
  int r = nSignatures/bands; //calculamos el numero signaturas por tira
  
  //recoremos todas las tiras
  for(int i=0; i < nSignatures; i += r) {
    //comparamos todos los ficheros entre ellos con esa tira
    for(int f=0; f < nFiles; f++) {
      for(int f1=f+1; f1 < nFiles; f1++) {
	if(!candidates[f][f1] and equalVector(signatureMatrix, f, f1, i, r)) {
	  candidates[f][f1] = true;
	}
      }
    }
  }
}

//Devuelve las possibles parajas de ficheros candidatas a ser similares
//candidates: el valor que devuelve
//bands: numero de tiras (tiene que ser obigatoriamente dividible por el numero de signatures)
//nshingles: numero de shingles
void getCandidatesOpt(vector<vector<int>>& signatureMatrix, vector<vector<bool>>& candidates, int bands, int nshingles) {
  //optimizamos mucho la cantidad de operaciones en tiras muy largas
  int nSignatures = signatureMatrix.size();
  int nFiles = signatureMatrix[0].size();
  int r = nSignatures/bands;

  vector<unsigned int> values(nFiles); //vector para guardar los codigos hash generados
  //recoremos todas las tiras
  for(int i=0; i < nSignatures; i += r) {
    //buscamos los valores de hash
    int a=1;
    for(int f=0; f < nFiles; f++) values[f] = 0;
    for(int j=0; j < r; j++) {
      for(int f=0; f < nFiles; f++) {
	values[f] = (values[f] + signatureMatrix[j+i][f]*a) % PRIME_NUMBER;
      }
      a *= nshingles;
    }
    //comparamos todos los ficheros entre ellos con esa tira
    for(int f=0; f < nFiles; f++) {
      for(int f1=f+1; f1 < nFiles; f1++) {
	if(!candidates[f][f1] and values[f] == values[f1]) {
	  candidates[f][f1] = true;
	}
      }
    }
  }
}

