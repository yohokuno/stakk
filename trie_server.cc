#include "trie_server.h"
using namespace stakk;

int main(int argc, char *argv[]) {
  //parse option
  int result, port = 50000;
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
  int field = reverse ? 4 : 0;
  if (!trie.load(filename, field, L'\t')) {
    cout << filename << " is not found." << endl;
    exit(0);
  }

  TrieServer server(port, trie);

  wcout << "input query: " << endl;
  result = server.communicate();

  return result;
}
