#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const long long int persistensy = 50;

char buf = 0, pos = 0;

void wr(ofstream &out, int bit) {
  if (pos >= 8) {
    out.write(&buf, 1);
    pos = 0;
    buf = 0;
  }
  // cout << bit;
  char mask = (char)(bit ? 1 : 0) << pos;
  buf += mask;
  pos++;
}

void flu(ofstream &out) { out.write(&buf, 1); }

void follow(ofstream &out, int bit, int foll) {
  wr(out, bit);
  for (; foll > 0; foll--) {
    wr(out, !bit);
  }
}

void print_table(vector<long long int> &li, unsigned short l,
                 unsigned short h) {
  int del = li[256];
  for (int i = 1; i < size(li); i++) {
    cout << (char)(i - 1) << ' ' << 1ll * (h - l + 1) * li[i - 1] / del << ' '
         << 1ll * (h - l + 1) * li[i] / del << "      ";
  }
  cout << endl;
}

string get_file_as_str(ifstream &inp) {
  stringstream ans;
  // ans << inp.rdbuf();
  return ans.str();
}

void update_pref_table(vector<long long int> &pref, int ind) {
  for (int i = ind + 1; i <= 256; i++)
    pref[i] += persistensy;
}

int main() {
  ifstream inp("./test");
  ofstream out("./out");
  string s = get_file_as_str(inp);
  int n = s.size();
  vector<int> weights(256, 1);
  vector<long long int> pref(257, 0);
  for (int i = 1; i <= 256; i++)
    pref[i] = weights[i - 1] + pref[i - 1];
  unsigned short l = 0, h = 65535;
  print_table(pref, l, h);
  long long int del = pref[256];
  unsigned short q1 = ((int)h + 1) / 4, q2 = q1 * 2, q3 = q1 * 3, bits = 0;
  for (int cur = 0; cur < n; cur++) {
    int ind = s[cur];
    // cout << "@@ " << l << ' ' << h << " @@" << endl;
    int prl = l, prh = h;
    del = pref[256];
    l = prl + 1ll * (prh - prl + 1) * pref[ind] / del;
    h = prl + 1ll * (prh - prl + 1) * pref[ind + 1] / del - 1;
    // cout << "@@ " << l << ' ' << h << " @@" << endl;
    while (true) {
      if (h < q2) {
        follow(out, 0, bits);
        bits = 0;
      } else if (l >= q2) {
        follow(out, 1, bits);
        bits = 0;
        l -= q2;
        h -= q2;
      } else if (l >= q1 && h < q3) {
        bits++;
        l -= q1;
        h -= q1;
      } else
        break;
      l += l;
      h += h + 1;
    }
    // cout << "@@ " << l << ' ' << h << " @@" << endl;
    update_pref_table(pref, ind);
    // cout << endl;
    // print_table(pref, l, h);
    // cout << endl;
  }
  flu(out);
}
