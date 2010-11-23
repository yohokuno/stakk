#include "common.h"
#include "util.h"
#include "ttrie.h"

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
    WideTrie trie;
    wcout << L"inserting.." << endl;
    trie.load(filename, 0, '\t');
    wcout << L"input kana:" << endl;
    wstring input;
    while (wcin >> input) {
        if (mode == "all" || mode == "search") {
            vector<wstring> *result;
            result = trie.search(input);
            wcout << L"search:" << endl;
            /*
            if (result)
                wcout << WideTrie::format(*result) << endl;
                */
        }
        if (mode == "all" || mode == "common") {
            WideTrie::Entries results;
            trie.common_prefix_search(input, L"", results);
            wcout << L"common:" << endl;
            //wcout << WideTrie::format(results);
        }
        if (mode == "all" || mode == "predict") {
            WideTrie::Entries results;
            trie.predictive_search(input, L"", results);
            wcout << L"predict:" << endl;
            //wcout << WideTrie::format(results);
        }
        if (mode == "all" || mode == "fuzzy") {
            WideTrie::Entries results;
            trie.fuzzy_search_ex(input, 2, results);
            wcout << L"fuzzy:" << endl;
            //wcout << WideTrie::format(results);
        }
    }
    return 0;
}
