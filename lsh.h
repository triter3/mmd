#include <map>
#include <vector>
#include <list>
#include <functional>
#include <iostream>
#include <bitset>
using namespace std;


void getCandidates(vector<vector<int>>& signatureMatrix, list<pair<int, int>>& candidates, int bands);

void getCandidatesOpt(vector<vector<int>>& signatureMatrix, list<pair<int, int>>& candidates, int bands, int nshingles);


