#include "../util.h"
using namespace stakk;

int main() {
    string input = "これはテストです。";
    int length = 9;
    assert(utf8_len(input.c_str(), input.length()) == length);
    assert(utf8_char(input.c_str()) == 3);

    vector<string> splited = utf8_split(input);
    assert(splited.size() == length);
    assert(splited[3] == "テ");
    assert(utf8_substring(input, 3, 3) == "テスト");
    return 0;
}
