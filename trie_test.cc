#include "common.h"
#include "util.h"
#include "trie.h"

int main(int argc, char *argv[]) {
    int result;
    string filename = "data/dictionary.txt";
    string mode = "all";

    while((result = getopt(argc, argv, "f:m:")) != -1) {
        switch(result) {
            case 'f':
                filename = optarg;
                break;
            case 'm':
                mode = optarg;
                break;
        }
    }
    Trie trie;
    cout << "inserting.." << endl;
    trie.load(filename, 0, '\t');
    cout << "input kana:" << endl;
    string input;
    while (cin >> input) {
        if (mode == "all" || mode == "search") {
            vector<string> *result;
            result = trie.search(input);
            cout << "search:" << endl;
            if (result)
                cout << Trie::format(*result) << endl;
        }
        if (mode == "all" || mode == "common") {
            Trie::Entries results;
            trie.common_prefix_search(input, "", results);
            cout << "common:" << endl;
            cout << Trie::format(results);
        }
        if (mode == "all" || mode == "predict") {
            Trie::Entries results;
            trie.predictive_search(input, "", results);
            cout << "predict:" << endl;
            cout << Trie::format(results);
        }
        if (mode == "all" || mode == "fuzzy") {
            Trie::Entries results;
            trie.fuzzy_search_ex(input, 2, results);
            cout << "fuzzy:" << endl;
            cout << Trie::format(results);
        }
    }
    return 0;
}
