#include "stakk_server.h"
using namespace stakk;

int main(int argc, char *argv[]) {
    //parse option
    int result;
    int port = 50000;
    string dictionary_ = "data/dictionary.txt";
    string connection_ = "data/connection.txt";
    string id_def = "data/id.def";
    bool debug = false;
    bool reverse = false;
    try { locale::global(locale("")); } catch (...) {}

    while((result = getopt(argc, argv, "d:c:i:p:br")) != -1) {
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
            case 'r':
                reverse = true;
                break;
        }
    }

    wcout << "loading dictionary" << endl;
    ListTrieWide trie;
    int field = reverse ? 4 : 0;
    if (!trie.load(dictionary_, field, L'\t')) {
        cout << dictionary_ << " is not found." << endl;
        exit(0);
    }

    wcout << "loading connection" << endl;
    Connection connection;
    if (!connection.load(connection_)) {
        cout << connection_ << " is not found." << endl;
        exit(0);
    }

    wcout << "loading id definition" << endl;
    Definition definition;
    if (!definition.load(id_def)) {
        cout << id_def << " is not found." << endl;
        exit(0);
    }

    //initialize action objects using data object reference.
    Stakk stakk(trie, connection);
    Converter converter(trie, connection, definition);
    StakkServer server(trie, stakk, converter);
    server.port = port;

    wcout << "server ready" << endl;
    result = server.communicate();

    return result;
}
