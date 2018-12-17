#include <iostream>
#include <map>
#include <vector>
#include <cstdlib> //random numbers
#include <ctime> //used generating seeds
using namespace std;

//pagina 12 final: colisiones no importantes
//a y b: enteros escogidos de forma aleatoria más pequeños o iguales que la máxima x
//c: es un numero primo un poco mayor que la máxima x
//hash ha de ser de tamaño c para poder poner todas las posibles h
void generateHash(map<string, vector<bool>>& matrix ,vector<vector<bool>*>& hash , int a, int b, int c);

int getPrime(int maxX);

void generateCoefficients(int& a, int& b, int maxX);

//t: número de funciones hash
//numDoc: número de documentos
//signatureMatrix inicializada con todo -1
void generateSignatureMatrix(map<string, vector<bool>>& matrix, int t, vector<vector<int>>& signatureMatrix, int numDoc);

double compareFilesHash(vector<vector<int>>& signatureMatrix, int file1, int file2, int t);

void generateSignatureSimilarity(vector<vector<int>>& signatureMatrix, vector<vector<double>>& sigSimilarity, int t);
