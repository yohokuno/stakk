#include "common.h"
#include "util.h"
#include "trie.h"
#include "connection.h"
#include "converter.h"

int main(int argc, char *argv[]) {
    //parse option
    int result;
    string dictionary_ = "data/dictionary.txt";
    string connection_ = "data/connection.txt";
    string id_def = "data/id.def";
    string output = "mecab";
    bool reverse = false;
    bool debug = false;
    locale::global(locale());

    while((result = getopt(argc, argv, "d:c:i:o:rb")) != -1) {
        switch(result) {
            case 'd':
                dictionary_ = optarg;
                break;
            case 'c':
                connection_ = optarg;
                break;
            case 'i':
                id_def = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case 'r':
                reverse = true;
                break;
            case 'b':
                debug = true;
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

    wcout << "loading id definition" << endl;
    Definition definition(id_def);

    Converter converter(trie, connection, definition);
    wstring line;

    wcout << "input query: " << endl;
    while (getline(wcin, line)) {
        vector<Converter::Node> nodes = converter.convert(line, debug);
        wcout << converter.format(nodes, output);
    }
    return 0;
}
