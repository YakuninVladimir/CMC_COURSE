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

void follow(ofstream &out, int bit, int foll) {
  wr(out, bit);
  for (; foll > 0; foll--) {
    wr(out, !bit);
  }
}

int main () {
  ifstream inp("./test");
  ofstream out("./out");
  string s;
  getline(inp, s);
  vector<int>weights(256);
  for (auto & i : s)weights[i]++;
  vector<int>pref(257, 0);
  for (int i = 1; i <= 256; i++)pref[i] = weights[i - 1] + pref[i - 1];
  unsigned short l = 0, h = 65535;
  long long int del = pref[256];
  unsigned short q1 = ((int)h + 1)/4, q2 = q1 * 2, q3 = q1*3, bits = 0;
  cout << q1 << ' ' << q2 << ' ' << q3 << endl;
  inp.seekg(0, ios_base::beg);
  char cur;
  while(inp.get(cur)) {
    int ind = cur;
    int prl = l, prh = h;
    l = prl + (prh - prl + 1) * pref[ind] / del;
    h = prl + (prh - prl + 1) * pref[ind + 1] / del - 1;
    while (true){
      //cout << l << " " << h << endl;
      if (h < q2) {
        follow(out, 0, bits);
        bits = 0;
      } else if (l > q2) {
        follow(out, 1, bits);
        bits = 0;
        l -= q2;
        h -= q2;
      } else if (l >= q1 && h < q3) {
        bits++;
        l -= q1;
        h -= q1;
      } else break;
      l += l;
      h += h + 1;
    }
  }
}
