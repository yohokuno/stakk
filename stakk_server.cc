#include "common.h"
#include "util.h"
#include "trie.h"
#include "stakk.h"
#include "server.h"
#include "trie_server.h"
#include "stakk_server.h"

int main(int argc, char *argv[]) {
    //parse option
    int result;
    int port = 54633;
    int threshold = 2;
    int number = 50;
    string dictionary = "data/dictionary.txt";
    string connection = "data/connection.txt";
    locale::global(locale(""));

    while((result = getopt(argc, argv, "d:c:t:n:p:")) != -1) {
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
            case 't':
                threshold = atoi(optarg);
                break;
            case 'n':
                number = atoi(optarg);
                break;
        }
    }

    ListTrieWide trie;
    wcout << "loading dictionary" << endl;
    trie.load_dictionary(dictionary);
    wcout << "loading connection" << endl;
    Stakk stakk(trie, connection);
    wcout << "input query: " << endl;
    StakkServer server(trie, stakk, threshold, number);
    server.port = port;
    server.threshold = threshold;
    result = server.communicate();

    return result;
}
