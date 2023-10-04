#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;


void wr(ofstream &out, int bit) {
  static char buf = 0, pos = 0;
  if (pos >= 8){
    out.write(&buf, 1);
    pos = 0;
    buf = 0; 
  }
  char mask = (char)(bit ? 1 : 0) << pos;
  buf += mask;
  pos++;
}

void rd(ifstream & inp, char & bit) {
  static char buf, pos;
  if (pos >= 8) {
    inp.read(buf, 1);
    pos = 0;
  }
  char mask = (char)(bit ? 1 : 0) << pos;
  bit = (mask & buf) >> pos;
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
  unsigned short l = 0, h = 65535;
  unsigned short q1 = ((int)h + 1)/4, q2 = q1 * 2, q3 = q1*3, bits = 0;
  char value;
  inp.read(value, 1);
  while(inp.peek() != EOF) {
    int ind = cur;
    int prl = l, prh = h;
    l = prl + (prh - prl + 1) * pref[ind] / del;
    h = prl + (prh - prl + 1) * pref[ind + 1] / del - 1;
    while (true) {
      if (h < q2) {}
      else if (l >= q2){
        value -= q2;
        l -= q2;
        h -= q2;
      }
      else if (l >= q1 && h < q3) {
        l -= q2;
        h -= q2
      } else break;
      l += l;
      h += h + 1;
      char bit;
      rd(inp, bit);
      value = (value << 1) + bit;
    }
  }
}
