#include "minhash.h"

//pagina 12 final, colisiones no importantes
void generateHash(map<string, vector<bool>>& matrix ,vector<vector<bool>*>& hash , int a, int b, int c) {
  int x=0;
  for(auto p : matrix) {
    int h = (a*x + b)%c;
    hash[h] = &p.second;
    x++;
  }
}


