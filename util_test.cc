#include <iostream>
#include <sys/types.h>
#include <vector>
#include <string>
#include "util.h"
using namespace Stakk;

int main() {
    string input = "これはテストです。";
    cout << "utf8_len: " << utf8_len(input.c_str(), input.length()) << endl;
    for (int i = 0; i < input.length(); i++) {
        cout << "utf8_char " << i << ": " << (int)utf8_char(input.c_str()+i) << endl;
    }
    vector<string> splited = utf8_split(input);
    for (int i = 0; i < splited.size(); i++) {
        cout << "utf8_split: " << splited[i] << endl;
    }
    string result = utf8_substring(input, 1, 4);
    cout << "utf8_substring: " << result << endl;
    return 0;
}
