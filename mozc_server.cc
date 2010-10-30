#include "common.h"
#include "util.h"
#include "trie.h"
#include "mozc.h"
#include "server.h"
#include "trie_server.h"
#include "mozc_server.h"

int main(int argc, char *argv[]) {
    //parse option
    int result;
    int port = 54633;
    string dictionary = "data/dictionary.txt";
    string connection = "data/connection.txt";
    locale::global(locale(""));

    while((result = getopt(argc, argv, "d:c:p:")) != -1) {
        switch(result) {
            case 'd':
                dictionary = optarg;
                break;
            case 'c':
                connection = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
        }
    }

    ListTrieWide trie;
    wcout << "loading dictionary" << endl;
    trie.load_dictionary(dictionary);
    wcout << "loading connection" << endl;
    Mozc mozc(trie, connection);
    wcout << "input query: " << endl;
    MozcServer server(trie, mozc);
    server.port = port;
    result = server.communicate();

    return result;
}
