#include "../trie.h"
using namespace stakk;

int main(int argc, char *argv[]) {
    string filename = "test/dictionary.txt";
    try { locale::global(locale("")); } catch (...) {}
    ListTrieWide trie;
    int result = trie.load(filename, 0, L'\t');

    assert(result == true);

    trie.display();

    { // search
        vector<wstring> *result = trie.search(L"わたし");
        assert(result != NULL);
        wcout << ListTrieWide::format(*result) << endl;
    }

    { // common prefix search
        ListTrieWide::Entries results;
        trie.common_prefix_search(L"わたし", L"", results);
        assert(results.size() != 0);
    }

    return 0;
}
