#include "decode.h"


vector<string> readWord(string &s) {
  vector<string> res (0);
  int lastStr = 0;
  int i=0;
  for(; i < s.size(); i++) {
    if(s[i] >= 'A' and s[i] <= 'Z') s[i] = 'a' + s[i] - 'A';
    if (not (s[i] >= 'a' and s[i] <= 'z' or s[i] >= '0' and s[i] <= '9' or s[i] == '-')) {
      if(lastStr != i) res.push_back(s.substr(lastStr, i-lastStr));
      lastStr = i+1;
    }
  }
  if(lastStr != i) res.push_back(s.substr(lastStr, i-lastStr));
  return res;
}

void parseFile(map<string, vector<bool>>& matrix, int k, const string& filename, int fPos, int nFiles) {
  ifstream f(filename);
  if (f.is_open()) {
    int i = 0;
    vector<string> buffer(k);
    int sfirst = 0;
    while(not f.eof()) {
      string w; f >> w;
      for(string& str :  readWord(w)) {
	buffer[i] = str;
	if(sfirst >= k-1) {
	  string res = buffer[(i+1)%k];
	  for(int j=2; j <= k; j++)
	    res += " " + buffer[(i+j)%k];

	  auto itr = matrix.insert(make_pair(res, vector<bool> (nFiles, 0))).first;
	  itr->second[fPos] = true;
	} else sfirst++;
	++i %= k;
      }
    }
  } else {
    cout << "El fichero " << filename << " no existe" << endl;
  }
  f.close();
}

void parse (map<string, vector<bool>>& matrix, int k, const vector<string>& filenames) {
  int nFiles = filenames.size();
  for(int i=0; i < nFiles; i++) {
    parseFile(matrix, k, filenames[i], i, nFiles);
  }
}


