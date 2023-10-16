#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef unsigned int us;

const long long int persistensy = 50;

bool is_eof = false;

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

unsigned char rd(ifstream &inp) {
  static int pos = 8;
  static unsigned char buf;
  if (pos >= 8) {
    if (inp.peek() == EOF) {
      is_eof = true;
      return 0;
    } else
      inp.read((char *)&buf, 1);
    pos = 0;
  }
  unsigned char mask = 1 << (pos);
  pos++;
  return (mask & buf) >> (pos - 1);
}

int get_symb(us value, us l, us h, vector<long long int> &pref) {
  int len = h - l + 1;
  int del = pref[256];
  for (int i = 1; i <= 256; i++) {
    cout << l + len * pref[i - 1] / del << " " << l + len * pref[i] / del
         << endl;
    if (value >= l + len * pref[i - 1] / del &&
        value < l + len * pref[i] / del) {
      return i - 1;
    }
  }
}

string get_file_as_str(ifstream &inp) {
  stringstream ans;
  ans << inp.rdbuf();
  return ans.str();
}

void print_table(vector<long long int> &li, unsigned short l,
                 unsigned short h) {
  int del = li[256];
  for (int i = 1; i < size(li); i++) {
    cout << (char)(i - 1) << ' ' << (h - l + 1) * li[i - 1] / del << ' '
         << (h - l + 1) * li[i] / del << "      ";
  }
  cout << endl;
}

void update_pref_table(vector<long long int> &pref, int ind) {
  for (int i = ind + 1; i <= 256; i++)
    pref[i] += persistensy;
}

int main() {
  ifstream inp("./out");
  ofstream out("./output");
  vector<long long int> pref(257, 0);
  for (int i = 1; i <= 256; i++)
    pref[i] = pref[i - 1] + 1;
  us l = 0, h = 65535;
  int del = pref[256];
  us q1 = ((int)h + 1) / 4, q2 = q1 * 2, q3 = q1 * 3;
  us value = 0;
  inp.read((char *)&value, 2);
  us m = 1, nn = 0;
  for (int i = 0; i < 16; i++) {
    nn <<= 1;
    nn ^= (m & value) >> i;
    m <<= 1;
  }
  print_table(pref, l, h);
  value = nn;
  int num_of_step = 0;
  int ind;
  do {
    num_of_step++;
    if (num_of_step / 1000 != (num_of_step - 1) / 1000)
      cout << num_of_step << endl;
    cout << "kkk" << endl;
    cout << "@@ " << l << ' ' << value << ' ' << bitset<16>(value) << ' ' << h
         << " @@" << endl;
    ind = get_symb(value, l, h, pref);
    cout << "<>" << (char)ind << "<>" << endl;
    char sym = ind;
    cout << "ggg" << endl;
    out.write(&sym, 1);
    cout << "rrr" << endl;
    int prl = l, prh = h;
    l = prl + (prh - prl + 1) * pref[ind] / del;
    h = prl + (prh - prl + 1) * pref[ind + 1] / del - 1;

    cout << "@@ " << l << ' ' << h << " @@" << endl;
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
      } else {
        break;
      }
      l += l;
      h += h + 1;
      unsigned char bit = rd(inp);
      if (is_eof) {
        return 0;
      }
      value = (value << 1) + bit;
    }

    cout << "@@ " << l << ' ' << h << " @@" << endl;
    update_pref_table(pref, ind);
    print_table(pref, l, h);
  } while (ind != 0);
}
