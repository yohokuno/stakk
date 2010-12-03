#include "../trie.h"
using namespace stakk;

int main(int argc, char *argv[]) {
    string filename = "test/dictionary.txt";
    try { locale::global(locale("")); } catch (...) {}
    ListTrieWide trie;
    int result = trie.load(filename, 0, L'\t');

    assert(result == true);

    //trie.display();

    wstring value = L"わたし	1	1	10	私";

    { // search
        vector<wstring> *result = trie.search(L"わたし");
        assert(result != NULL);
        assert((*result)[0] == value);
    }

    { // common prefix search
        ListTrieWide::Entries results;
        trie.common_prefix_search(L"わたしのなまえ", L"", results);
        assert(results.size() != 0);
        assert(results[0].key == L"わたし");
        assert(results[0].values.size() != 0);
        assert(results[0].values[0] == value);
    }

    { // common prefix search
        ListTrieWide::Entries results;
        trie.common_prefix_search(L"わたしのなまえ", L"", results);
        assert(results.size() != 0);
        assert(results[0].key == L"わたし");
        assert(results[0].values.size() != 0);
        assert(results[0].values[0] == value);
    }

    { // predictive search
        ListTrieWide::Entries results;
        trie.predictive_search(L"わた", L"", results);
        assert(results.size() != 0);
        assert(results[0].key == L"わたし");
        assert(results[0].values.size() != 0);
        assert(results[0].values[0] == value);
    }

    { // fuzzy search
        ListTrieWide::Entries results;
        trie.fuzzy_search_ex(L"わあし", 1, results);
        assert(results.size() != 0);
        assert(results[0].key == L"わたし");
        assert(results[0].values.size() != 0);
        assert(results[0].values[0] == value);
    }
    return 0;
}
