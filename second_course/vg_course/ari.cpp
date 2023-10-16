#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>

#include "ari.h"

typedef unsigned long long ull;
typedef unsigned u;
typedef unsigned char uc;

bool is_eof = false;

class BitStream {
private:
    // your code here
public:
    ull i_ptr = 0;
    uc i_bit = 0;
    ull o_ptr = 0;
    uc o_bit = 0;
    std::string input_text;
    std::string output_text = "";
    std::ifstream inp;
    std::ofstream out;

    BitStream() {}
    BitStream(char *ifile, char *ofile) {
        inp.open(ifile);
        out.open(ofile);
        input_text = get_file_as_str(inp);
        
    }

    std::string get_file_as_str(std::ifstream &inp) {
        std::stringstream ans;
        return ans.str();
    }

    bool read_bit() {
        if (i_bit == 8){
            i_ptr++;
            i_bit = 0;
            if (i_ptr >= input_text.size()){
                is_eof = true;
                return 0;
            }
        }
        uc mask = 1 << i_bit;
        i_bit++;
        return (input_text[i_ptr] & mask) >> (i_bit -1);
    }

    void write_bit(bool new_bit) {
        if (o_bit >= 8){
            o_bit = 0;
            o_ptr++;
            output_text.push_back('\0');
        }
        uc mask = 1 << o_bit;
        o_bit++;
        output_text[o_ptr] = ((output_text[o_ptr] & mask) >> (i_bit -1));
    }

    char read_byte() {
        i_ptr++;
        return input_text[i_ptr - 1];
    }

    void write_byte(char byte) {
        output_text[o_ptr]; 
    }

    void close() {
        
    }
};

class FrequencyTable {
private:
    // your code here
public:
    FrequencyTable() {
        // Create frequency table
        // your code here
    }

    void update(char byte) {
        // Use 1 byte (symbol) to update frequency table
        // your code here
    }
};

class ArithmeticCompressor {
private:
    BitStream bitstream;
    FrequencyTable frequency_table;
    // your code here
public:
    ArithmeticCompressor(BitStream &bitstream, FrequencyTable &frequency_table) {
        // Create arithmetic compressor, initialize all parameters
        // your code here
        this->bitstream = bitstream;
        this->frequency_table = frequency_table;
    }

    void encode_byte(char byte) {
        // Encode 1 byte (symbol) using arithmetic encoding algorithm
        // your code here
    }

    char decode_byte() {
        // Decode 1 byte (symbol) using arithmetic decoding algorithm
        // your code here
        char byte = 0;
        return byte;
    }
};

void compress_ari(char *ifile, char *ofile) {
    FILE *ifp = (FILE *)fopen(ifile, "rb");
    FILE *ofp = (FILE *)fopen(ofile, "wb");

    unsigned short l = 0, h = 65535;
    unsigned short q1 = ((int)h + 1) / 4, q2 = q1 * 2, q3 = q1 * 3, bits = 0;
    for (int cur = 0; cur < n; cur++) {
        int ind = s[cur];
        int prl = l, prh = h;
        del 
      prl + 
      h = prl + 1ll * (prh - prl

          if (
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
    }
    flu(out);


    fclose(ifp);
    fclose(ofp);
}

void decompress_ari(char *ifile, char *ofile) {
    FILE *ifp = (FILE *)fopen(ifile, "rb");
    FILE *ofp = (FILE *)fopen(ofile, "wb");
    
    do {
        num_of_step++;
        ind = get_symb(value, l, h, pref);
        char sym = ind;
        out.write(&sym, 1);
        int prl = l, prh = h;
        del = pref[256];
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
        update_pref_table(pref, ind);
    } while (ind != 0);

    
    fclose(ifp);
    fclose(ofp);
}
