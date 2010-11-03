#include "common.h"
#include "util.h"
#include "trie.h"
#include "connection.h"
#include "stakk.h"
#include "analyzer.h"
#include "server.h"
#include "trie_server.h"
#include "stakk_server.h"

int main(int argc, char *argv[]) {
    //parse option
    int result;
    int port = 54633;
    int threshold = 2;
    int number = 50;
    string dictionary_ = "data/dictionary.txt";
    string connection_ = "data/connection.txt";
    string id_def = "data/id.def";
    bool debug = false;
    bool reverse = false;

    while((result = getopt(argc, argv, "d:c:i:t:n:p:blr")) != -1) {
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
            case 'p':
                port = atoi(optarg);
                break;
            case 't':
                threshold = atoi(optarg);
                break;
            case 'n':
                number = atoi(optarg);
                break;
            case 'l':
                locale::global(locale(""));
                break;
            case 'r':
                reverse = true;
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

    //initialize action objects using data object reference.
    Stakk stakk(trie, connection);
    Analyzer analyzer(trie, connection, definition);
    StakkServer server(trie, stakk, analyzer, threshold, number);
    server.port = port;
    server.threshold = threshold;

    wcout << "server ready" << endl;
    result = server.communicate();

    return result;
}
