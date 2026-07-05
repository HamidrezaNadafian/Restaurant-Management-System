#include "lib.h"


string get_hidden_string(const vector<unsigned char>& bytes) {

    const unsigned char key = 0x5F;

    string decrypted_str = "";
    for (unsigned char b : bytes) {
        decrypted_str += (b ^ key);
    }
    return decrypted_str;
}


