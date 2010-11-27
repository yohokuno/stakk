#include "trie_server.h"
using namespace stakk;

int main(int argc, char *argv[]) {
    //parse option
    int result, port = 54633;
    string filename = "data/dictionary.txt";
    bool reverse = false;
    try { locale::global(locale("")); } catch (...) {}

    while((result = getopt(argc, argv, "f:p:r")) != -1) {
        switch(result) {
            case 'f':
                filename = optarg;
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
    if (reverse)
        trie.load(filename, 4, L'\t');
    else
        trie.load(filename, 0, L'\t');

    TrieServer server(trie);
    server.port = port;

    wcout << "input query: " << endl;
    result = server.communicate();

    return result;
}
