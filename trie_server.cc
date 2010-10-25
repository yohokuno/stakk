#include "common.h"
#include "trie.h"
#include "util.h"
#include "server.h"
#include "trie_server.h"

//main logic
int main(int argc, char *argv[]) {
    TrieServer server;

    server.parse_option(argc,argv);
    server.load_trie();
    int result = server.communicate();

    return result;
}
