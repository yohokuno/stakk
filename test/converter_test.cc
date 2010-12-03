#include "../converter.h"
using namespace stakk;

int main(int argc, char *argv[]) {
    try { locale::global(locale("")); } catch (...) {}

    string dictionary_ = "test/dictionary.txt";
    string connection_ = "test/connection.txt";
    string id_def = "test/id.def";

    ListTrieWide trie;
    assert(trie.load(dictionary_, 0, L'\t') == true);

    Connection connection;
    assert(connection.load(connection_) == true);

    Definition definition;
    assert(definition.load(id_def) == true);

    Converter converter(trie, connection, definition);

    {
        vector<Converter::Node> result;
        wstring input = L"わたしのなまえ";
        result = converter.convert(input, false);
        assert(result.size() == 3);
        assert(result[0].yomi == L"わたし");
        assert(result[0].word == L"私");
        assert(result[1].yomi == L"の");
        assert(result[1].word == L"の");
        assert(result[2].yomi == L"なまえ");
        assert(result[2].word == L"名前");
    }
    return 0;
}
