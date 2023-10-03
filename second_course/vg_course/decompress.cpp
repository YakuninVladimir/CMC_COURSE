#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

char buf = 0, pos = 0;

void wr(ofstream &out, int bit) {
  if (pos >= 8){
    out.write(&buf, 1);
    pos = 0;
    buf = 0; 
  }
  char mask = (char)(bit ? 1 : 0) << pos;
  buf += mask;
  pos++;
}

int main() {
  ifstream table("./test");
  ifstream inp("./out");
  ofstream out("./output");
  string s;
  getline(table, s);
  vector<int>weights(256);
  for (auto & i : s)weights[i]++;
  vector<int>pref(257, 0);
  for (int i = 1; i <= 256; i++)pref[i] = weights[i - 1] + pref[i - 1];
}
