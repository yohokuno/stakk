#include "common.h"
#include "util.h"
#include "trie.h"
#include "server.h"
#include "trie_server.h"

//main logic
int main(int argc, char *argv[]) {
    //parse option
    int result, port = 54633;
    string filename = "data/dictionary.txt";
    locale::global(locale(""));

    while((result = getopt(argc, argv, "f:p:")) != -1) {
        switch(result) {
            case 'f':
                filename = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
        }
    }

    ListTrieWide trie;
    trie.load(filename, 0, L'\t');
    wcout << "input query: " << endl;
    TrieServer server(trie);
    server.port = port;
    result = server.communicate();

    return result;
}
