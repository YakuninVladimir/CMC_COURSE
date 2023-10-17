#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <limits.h>
#include <sstream>
#include <vector>

#include "ari.h"

typedef unsigned long long ull;
typedef unsigned u;
typedef unsigned char uc;

bool is_eof = false;

class BitStream {
private:
  ull i_ptr = 0;
  uc i_bit = 0;
  ull o_ptr = 0;
  uc o_bit = 0;

public:
  std::string input_text;
  std::string output_text = "";
  std::ifstream inp;
  std::ofstream out;
  u in_len = 0;
  u follow = 0;

  BitStream(char *ifile, char *ofile) {
    inp.open(ifile);
    out.open(ofile);
    input_text = get_file_as_str(inp);
    in_len = input_text.size();
  }

  std::string get_file_as_str(std::ifstream &inp) {
    std::stringstream ans;
    ans << inp.rdbuf();
    return ans.str();
  }

  bool read_bit() {
    if (i_bit == 8) {
      i_ptr++;
      i_bit = 0;
      if (i_ptr >= input_text.size()) {
        is_eof = true;
        return 0;
      }
    }
    uc mask = 1 << i_bit;
    i_bit++;
    return (input_text[i_ptr] & mask) >> (i_bit - 1);
  }

  void write_bit(uc new_bit) {
    if (o_bit >= 8) {
      o_bit = 0;
      o_ptr++;
      output_text.push_back('\0');
    }
    uc mask = 1 << o_bit;
    o_bit++;
    output_text[o_ptr] = ((output_text[o_ptr] & mask) >> (i_bit - 1));
  }

  char read_byte() {
    i_ptr++;
    return input_text[i_ptr - 1];
  }

  void write_byte(char byte) {
    output_text[o_ptr] = byte;
    output_text.push_back('\0');
    o_ptr++;
  }

  void bits_to_follow(uc fbit) {
    write_bit(fbit);
    for (; follow > 0; follow--) {
      write_bit(1 - fbit);
    }
  }

  void close() {
    if (o_bit != 0) {
      output_text.push_back('\0');
    }
    out << output_text;
    inp.close();
    out.close();
  }
};

class FrequencyTable {
private:
  int ar_size = 256;
  u mode = 100000;
  const ull persistence = 50;
  std::vector<ull> weights;

public:
  std::vector<ull> pref;
  ull del;

  FrequencyTable() {
    weights.assign(ar_size, 1);
    pref.assign(ar_size + 1, 0);
    for (int i = 1; i <= ar_size; i++) {
      pref[i] = pref[i - 1] + weights[i - 1];
    }
    del = pref[256];
  }

  void update(u pos) {
    weights[pos] += persistence;
    if (weights[pos] >= mode) {
      weights[pos] /= 2;
    }
    for (int i = 1; i <= ar_size; i++) {
      pref[i] = pref[i - 1] + weights[i - 1];
    }
    del = pref[256];
  }

  char get_symb(u l, u h, u value) {
    ull len = h - l + 1;
    for (int i = 1; i <= 256; i++) {
      if (value >= l + len * pref[i - 1] / del &&
          value < l + len * pref[i] / del) {
        return i - 1;
      }
    }
  }
};

void compress_ari(char *ifile, char *ofile) {
  BitStream bs(ifile, ofile);
  FrequencyTable ft;
  u l = 0, h = UINT_MAX;
  u q1 = ((int)h + 1) / 4, q2 = q1 * 2, q3 = q1 * 3, bits = 0;
  for (int cur = 0; cur < bs.in_len; cur++) {
    int ind = bs.read_byte();
    int prl = l, prh = h;
    l = prl + 1ll * (prh - prl + 1) * ft.pref[ind] / ft.del;
    h = prl + 1ll * (prh - prl + 1) * ft.pref[ind + 1] / ft.del - 1;
    while (true) {
      if (h < q2) {
        bs.bits_to_follow(0);
      } else if (l >= q2) {
        bs.bits_to_follow(1);
        l -= q2;
        h -= q2;
      } else if (l >= q1 && h < q3) {
        bs.follow++;
        l -= q1;
        h -= q1;
      } else
        break;
      l += l;
      h += h + 1;
    }
  }
  bs.close();
}

void decompress_ari(char *ifile, char *ofile) {
  BitStream bs(ifile, ofile);
  FrequencyTable ft;
  u l = 0, h = UINT_MAX;
  u q1 = ((int)h + 1) / 4, q2 = q1 * 2, q3 = q1 * 3, bits = 0;

  u value = 0, ind;
  for (int i = 0; i < 32; i++) {
    value = (value << 1) + bs.read_bit();
  }

  ull num_of_step = 0;
  do {
    num_of_step++;
    ind = ft.get_symb(l, h, value);
    char sym = ind;
    bs.write_byte(sym);
    u prl = l, prh = h;
    l = prl + (prh - prl + 1) * ft.pref[ind] / ft.del;
    h = prl + (prh - prl + 1) * ft.pref[ind + 1] / ft.del - 1;
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
      if (is_eof) {
        break;
      }
      value = (value << 1) + bs.read_bit();
    }
    ft.update(ind);
  } while (ind != 0 && !is_eof);
  bs.close();
}
