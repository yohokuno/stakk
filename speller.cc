#include "trie.h"
using namespace stakk;

int main(int argc, char *argv[]) {
  int result;
  string filename = "data/query.txt";
  string mode;
  try { locale::global(locale("")); } catch (...) {}

  while((result = getopt(argc, argv, "f:m:r")) != -1) {
    switch(result) {
      case 'f':
        filename = optarg;
        break;
      case 'm':
        mode = optarg;
        break;
    }
  }
  cout << "inserting.." << endl;
  SimpleTrie trie;
  if (!trie.load(filename, 1, '\t')) {
    cout << filename << " is not found." << endl;
    exit(0);
  }
  cout << "input kana:" << endl;
  string input;
  while (cin >> input) {
    SimpleTrie::Entries results;
    trie.fuzzy_search_ex(input, 1, results);
    cout << SimpleTrie::format(results);
  }
  return 0;
}
