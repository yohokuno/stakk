#include "../trie.h"
using namespace stakk;

int main(int argc, char *argv[]) {
    string filename = "test/dictionary.txt";
    try { locale::global(locale("")); } catch (...) {}
    ListTrieWide trie;
    int result = trie.load(filename, 0, L'\t');

    assert(result == true);

    //trie.display();

    { // search
        vector<wstring> *result = trie.search(L"わたし");
        assert(result != NULL);
        wstring value = L"わたし	2283	2283	0	わたし";
        assert((*result)[0] == value);
    }

    { // common prefix search
        ListTrieWide::Entries results;
        trie.common_prefix_search(L"わたし", L"", results);
        assert(results.size() != 0);
        assert(results[0].key == L"わたし");
        assert(results[0].values.size() != 0);
        wstring value = L"わたし	2283	2283	0	わたし";
        assert(results[0].values[0] == value);
    }

    return 0;
}
