#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;

void parseFile(map<string, vector<bool>>& matrix, int k, const string& filename, int fPos, int nFiles);

void parse (map<string, vector<bool>>& matrix, int k, const vector<string>& filenames);
