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
    string mode = "analyze";
    bool debug = false;
    locale::global(locale(""));

    while((result = getopt(argc, argv, "d:c:i:o:m:b")) != -1) {
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
            case 'm':
                mode = optarg;
                break;
            case 'b':
                debug = true;
                break;
        }
    }

    wcout << "loading dictionary" << endl;
    ListTrieWide trie;
    if (mode == "analyze")
        trie.load(dictionary_, 4, L'\t');
    else if (mode == "convert")
        trie.load(dictionary_, 0, L'\t');

    wcout << "loading connection" << endl;
    Connection connection(connection_);

    wcout << "loading id definition" << endl;
    Definition definition(id_def);

    Analyzer analyzer(trie, connection, definition);
    wstring line;

    wcout << "input query: " << endl;
    while (getline(wcin, line)) {
        vector<Analyzer::Node> nodes = analyzer.analyze(line, debug);
        wcout << analyzer.format(nodes, output);
    }
    return 0;
}
