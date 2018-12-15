#include "minhash.h"

//pagina 12 final: colisiones no importantes
//a y b: enteros escogidos de forma aleatoria más pequeños o iguales que la máxima x
//c: es un numero primo un poco mayor que la máxima x
//hash ha de ser de tamaño c para poder poner todas las posibles h
void generateHash(map<string, vector<bool>>& matrix ,vector<vector<bool>*>& hash , int a, int b, int c) {
  int x=0;
  for (auto it = matrix.begin(); it != matrix.end(); ++it) {
    int h = (a*x + b)%c;
    hash[h] = &(it->second);
    //cout << "x: " << x << " h: " << h << endl;
    //cout << hash[h]->at(0) << " " << hash[h]->at(1) << " "<< hash[h]->at(2) << endl; 
    x++;
  }
}

int getPrime(int maxX) {
  vector<int> primos(0);
  primos.push_back(2);
  int i=0;
  int n=3;
  while(primos[i] < maxX) {
    bool p = true;
    for(int k=0; k <= i and p and primos[k]*primos[k] <= n; k++)
      p = (n%primos[k] != 0);
    
    if(p) {
      primos.push_back(n);
      i++;
    }
    n += 2; //los pares seguro que no son primos  
  }
  
  return n-2;
}

//Admite mejoras... de momento sirve para un primer prototipo
void generateCoefficients(int& a, int& b, int maxX) {
    a = (rand()%(maxX-1))+2;
    //b no puede ser multiplo de a, si lo es lo volvemos a buscar
    cout << a << endl;
    while((b = (rand()%maxX)+1)%a == 0);
    cout << "DEBUG: a: " << a << " b: " << b << endl; 
}

/*void scriptedCoefficients(int& a, int& b, int& c, int i) {
    if (i == 0) {
        a = 12;
        b = 8;
        c = 17;
    }
    else if (i == 1) {
        a = 14;
        b = 3;
        c = 17;
    }
}*/

//t: número de funciones hash
//numDoc: número de documentos
//signatureMatrix inicializada con todo -1
void generateSignatureMatrix(map<string, vector<bool>>& matrix, int t, vector<vector<int>>& signatureMatrix, int numDoc) {
    srand(time(NULL));
    int c = getPrime(matrix.size()); //creo que es sin el -1, no?
    //Vector que almacena cada vector de valores generados por cada hash
    vector<vector<vector<bool>*>> hashes(t, vector<vector<bool>*>(c, nullptr)); 
    for (int i = 0; i < t; ++i) { //Por cada Hash
        int a, b;
        generateCoefficients(a, b, matrix.size());
        generateHash(matrix, hashes[i], a, b, c);
        //Rellenar signatureMatrix
        int found = 0;
        //Por cada valor obtenido con el Hash
        for (int j = 0; j < hashes[i].size() && found < numDoc; ++j) { 
            //Puede ser que una posición no tenga valor si la función de hash
            //ha dado colisiones
            if (hashes[i][j] != nullptr) {
                //Por cada documento
                for (int k = 0; k < hashes[i][j]->size() && found < numDoc; ++k) {
                    //cout << "i: " << i << " j: " << j << " k: " << k << endl;
                    //cout << hashes[i][j]->at(k) << endl;
                    if (signatureMatrix[i][k] == -1 && hashes[i][j]->at(k)) {
                        //cout << "ENTRY" << endl;
                        ++found;
                        signatureMatrix[i][k] = j;
                    }
                }
            }
        }
    }
}

double compareFilesHash(vector<vector<int>>& signatureMatrix, int file1, int file2, int t) {
    int count = 0;
    for (int i = 0; i < signatureMatrix.size(); ++i) {
        if (signatureMatrix[i][file1] == signatureMatrix[i][file2]) ++count;
    }
    return (double)count/t;
}

void generateSignatureSimilarity(vector<vector<int>>& signatureMatrix, vector<vector<double>>& sigSimilarity, int t) {
    for (int i = 0; i < sigSimilarity.size(); ++i) {
        for (int j = 0; j <= i; ++j) {
            if (i == j) sigSimilarity[i][j] = 1.0d;
            else sigSimilarity[i][j] = compareFilesHash(signatureMatrix, i, j, t);
        }
    }
}


