#include "server.h"

//main logic
int main(int argc, char *argv[]) {
    Server server;

    server.parse_option(argc,argv);
    server.load_trie();
    int result = server.communicate();

    return result;
}
