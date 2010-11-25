#include "common.h"
#include "util.h"
#include "trie.h"

using namespace Stakk;

int main(int argc, char *argv[]) {
    int result;
    string filename = "data/dictionary.txt";
    string mode = "all";
    try { locale::global(locale("")); } catch (...) {}

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
    ListTrieWide trie;
    wcout << "inserting.." << endl;
    trie.load(filename, 0, L'\t');
    wcout << "input kana:" << endl;
    wstring input;
    while (wcin >> input) {
        if (mode == "all" || mode == "search") {
            vector<wstring> *result;
            result = trie.search(input);
            wcout << "search:" << endl;
            if (result)
                wcout << ListTrieWide::format(*result) << endl;
        }
        if (mode == "all" || mode == "common") {
            ListTrieWide::Entries results;
            trie.common_prefix_search(input, L"", results);
            wcout << "common:" << endl;
            wcout << ListTrieWide::format(results);
        }
        if (mode == "all" || mode == "predict") {
            ListTrieWide::Entries results;
            trie.predictive_search(input, L"", results);
            wcout << "predict:" << endl;
            wcout << ListTrieWide::format(results);
        }
        if (mode == "all" || mode == "fuzzy") {
            ListTrieWide::Entries results;
            trie.fuzzy_search_ex(input, 2, results);
            wcout << "fuzzy:" << endl;
            wcout << ListTrieWide::format(results);
        }
    }
    return 0;
}
