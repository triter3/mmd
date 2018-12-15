#include "lsh.h"

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

//te devuelve los possibles candidatos a ser similares
//candidates: el valor que devuelve
//bands: numero de tiras (tiene que ser obigatoriamente dividible por el numero de signatures)
void getCandidates(vector<vector<int>>& signatureMatrix, list<pair<int, int>>& candidates, int bands) {
  //optimizamos mucho la cantidad de operaciones en tiras muy largas
  int nSignatures = signatureMatrix.size();
  int nFiles = signatureMatrix[0].size();
  int r = nSignatures/bands;

  //para una version de mejora
  /*
  vector<int> buffer(r); //vector para calcular el codigo hash de cada tira
  vector<unsigned int> values(nFiles); //vector para guardar los valores de hash
  */
  
  //vector que nos indica si una pareja ya es candidata a ser similar o no
  vector<vector<bool>> vis(nFiles, vector<bool> (nFiles, false)); //para optimizar comparaciones
  //recoremos todas las tiras
  for(int i=0; i < nSignatures; i += r) {

    //para una version de mejora
    /*
    for(int f=0; f < nFiles; f++) {
      for(int j=0; j < r ; j++) {
	buffer[j] = signatureMatrix[i+j][f];
      }
      values[f] = vectorHash(buffer); 
    }
    */

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

