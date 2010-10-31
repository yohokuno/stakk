#include "common.h"
#include "util.h"
#include "trie.h"
#include "connection.h"
#include "analyzer.h"

int main(int argc, char *argv[]) {
    //parse option
    int result;
    string dictionary_ = "data/dictionary.txt";
    string connection_ = "data/connection.txt";
    string id_def = "data/id.def";
    string output = "mecab";
    locale::global(locale(""));

    while((result = getopt(argc, argv, "d:c:i:o:")) != -1) {
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
        }
    }

    wcout << "loading dictionary" << endl;
    ListTrieWide trie;
    trie.load_dictionary_reverse(dictionary_);

    wcout << "loading connection" << endl;
    Connection connection(connection_);

    wcout << "loading id definition" << endl;
    Definition definition(id_def);

    Analyzer analyzer(trie, connection, definition);
    wstring line;

    wcout << "input query: " << endl;
    while (getline(wcin, line)) {
        vector<Analyzer::Node> nodes = analyzer.analyze(line);
        wcout << analyzer.format(nodes, output);
    }
    return 0;
}
