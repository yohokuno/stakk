#include "common.h"
#include "util.h"
#include "trie.h"
#include "connection.h"
#include "stakk.h"

int main(int argc, char *argv[]) {
    //parse option
    int result, threshold = 1;
    string dictionary_ = "data/dictionary.txt";
    string connection_ = "data/connection.txt";
    string mode = "spell";
    bool reverse = false;
    try { locale::global(locale("")); } catch (...) {}

    while((result = getopt(argc, argv, "d:c:t:m:r")) != -1) {
        switch(result) {
            case 'd':
                dictionary_ = optarg;
                break;
            case 'c':
                connection_ = optarg;
                break;
            case 't':
                threshold = atoi(optarg);
                break;
            case 'm':
                mode = optarg;
                break;
            case 'r':
                reverse = true;
                break;
        }
    }

    wcout << "loading dictionary" << endl;
    ListTrieWide trie;
    if (reverse)
        trie.load(dictionary_, 4, L'\t');
    else
        trie.load(dictionary_, 0, L'\t');

    wcout << "loading connection" << endl;
    Connection connection(connection_);

    Stakk stakk(trie, connection);
    wstring line;

    wcout << "input query: " << endl;
    while (getline(wcin, line)) {
        vector<Stakk::Entry> result;
        if (mode == "spell")
            stakk.correct(line, threshold, result);
        if (mode == "predict")
            stakk.predict(line, result);
        for (int i = 0; i < min(50, (int)result.size()); i++) {
            wcout << result[i].format() << endl;
        }
    }
    return 0;
}
