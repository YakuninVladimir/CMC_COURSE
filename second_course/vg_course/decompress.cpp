#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef unsigned short us;

void wr(ofstream &out, int bit) {
  static char buf = 0, pos = 0;
  if (pos >= 8) {
    out.write(&buf, 1);
    pos = 0;
    buf = 0;
  }
  char mask = (char)(bit ? 1 : 0) << pos;
  buf += mask;
  pos++;
}

void rd(ifstream &inp, char &bit) {
  static char buf, pos;
  if (pos >= 8) {
    inp.read((char *)&buf, 1);
    pos = 0;
  }
  char mask = (char)(bit ? 1 : 0) << (7 - pos);
  bit = (mask & buf) >> (pos);
}

int get_symb(us value, us l, us h, vector<int> &pref) {
  // cerr << "ffffuuu" << endl;
  int len = h - l + 1;
  int del = pref[256];
  for (int i = 1; i <= 256; i++) {
    // cout << l + len * pref[i - 1] / del << ' ' << value << ' '
    //    << l + len * pref[i] / del << endl;
    if (value >= l + len * pref[i - 1] / del &&
        value < l + len * pref[i] / del) {
      return i - 1;
    }
  }
  // cerr << "lildeadxd" << endl;
}

int main() {
  cout << "fuck" << endl;
  ifstream table("./test");
  ifstream inp("./out");
  ofstream out("./output");
  cerr << "fuck" << endl;
  string s;
  getline(table, s);
  vector<int> weights(256);
  for (auto &i : s)
    weights[i]++;
  vector<int> pref(257, 0);
  for (int i = 1; i <= 256; i++)
    pref[i] = weights[i - 1] + pref[i - 1];
  us l = 0, h = 65535;
  int del = pref[256];
  for (auto i : pref)
    cout << i << " ";
  us q1 = ((int)h + 1) / 4, q2 = q1 * 2, q3 = q1 * 3, bits = 0;
  us value;
  inp.read((char *)&value, 2);
  us m = 1, nn = 0;
  for (int i = 0; i < 16; i++) {
    cout << ((m & value) >> i);
    nn ^= (m & value) >> i;
    nn <<= 1;
    m <<= 1;
  }
  value = nn;
  for (int e = 0; e < 12; e++) {
    cout << '<' << l << ' ' << value << ' ' << h << '>' << endl;
    int ind = get_symb(value, l, h, pref);
    char sym = ind;
    out.write(&sym, 1);
    int prl = l, prh = h;
    l = prl + (prh - prl + 1) * pref[ind] / del;
    h = prl + (prh - prl + 1) * pref[ind + 1] / del - 1;
    while (true) {
      if (h < q2) {
      } else if (l >= q2) {
        value -= q2;
        l -= q2;
        h -= q2;
      } else if (l >= q1 && h < q3) {
        l -= q1;
        h -= q1;
        value -= q1;
      } else
        break;
      // cout << "sex" << endl;
      // cout << l << ' ' << h << ' ' << ind << endl;
      // cout << pref[ind] << ' ' << pref[ind + 1] << endl;

      l += l;
      h += h + 1;
      char bit;
      rd(inp, bit);
      cerr << bit;
      value = (value << 1) + bit;
    }
  }
}
