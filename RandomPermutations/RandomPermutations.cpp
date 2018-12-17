#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm> //random_shuffle()
#include <cstdlib> //srand()
#include <ctime> //time()
using namespace std;

void swap(string& a, string& b) {
  string aux = a;
  a = b;
  b = aux;
}

//permuta n veces los elementos del vector
void shuffle(vector<string>& words, int n) {
  int s = words.size();
  for(int i=0; i < n; i++) {
    swap(words[rand()%s], words[rand()%s]);
  }
}

int main() {
  srand(time(NULL));
  int nPerm;
  string file;
  int nPermFile;
  cout << "Nombre del fichero: ";
  cin >> file;
  cout << endl;
  cout << "Número de permutaciones: ";
  cin >> nPerm;
  cout << endl;
  cout << "Numero de permutaciones por fichero: ";
  cin >> nPermFile;
    
  //Lectura fichero
  vector<string> words(0);
  ifstream f(file);
  if (f.is_open()) {
    while (not f.eof()) {
      string w;
      f >> w;
      words.push_back(w);
    }
    f.close();
        
    //Aprovechamos una función de la libreria estandar de c++
    //para hacer las permutaciones aleatorias
    char buffer[128];
    for (int i = 1; i <= nPerm; ++i) {
      //random_shuffle(words.begin(), words.end());
      shuffle(words, nPermFile);
      sprintf(buffer, "%s_perm_%d", file.c_str(), i);
      ofstream fout(buffer);
      int endline = 0;
      if (fout.is_open()) {
	for (int j = 0; j < words.size(); ++j) {
	  fout << words[j] << " ";
	  //Añadir un salto de linea cada 10 palabras
	  //para leer el fichero más fácilmente
	  if (++endline == 10) {
	    fout << endl;
	    endline = 0;
	  }
	}
	fout.close();
      }
      else {
	cout << "No se ha podido crear " << buffer << endl;
      }
    }
        
  } else {
    cout << "El fichero " << file << " no existe" << endl;
  }
}
