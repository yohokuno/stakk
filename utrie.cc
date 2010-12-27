#include "utrie.h"
using namespace stakk;

int main(int argc, char *argv[]) {
  int result;
  string filename = "data/dictionary.txt";
  string mode = "all";
  bool reverse = false;

  while((result = getopt(argc, argv, "f:m:r")) != -1) {
    switch(result) {
      case 'f':
        filename = optarg;
        break;
      case 'r':
        reverse = true;
        break;
      case 'm':
        mode = optarg;
        break;
    }
  }
  cout << "inserting.." << endl;
  UTrie trie;
  int field = reverse ? 4 : 0;
  if (!trie.load(filename, field, '\t')) {
    cout << filename << " is not found." << endl;
    exit(0);
  }
  cout << "input kana:" << endl;
  string input;
  while (cin >> input) {
    if (mode == "all" || mode == "search") {
      vector<string> *result;
      result = trie.search(input);
      cout << "search:" << endl;
      if (result) {
        for (int i = 0; i < result->size(); i++) {
          cout << (*result)[i] << " ";
        }
        cout << endl;
      }
    }
    /*
    if (mode == "all" || mode == "common") {
      UTrie::Entries results;
      trie.common_prefix_search(input, "", results);
      cout << "common:" << endl;
      cout << UTrie::format(results);
    }
    if (mode == "all" || mode == "predict") {
      UTrie::Entries results;
      trie.predictive_search(input, "", results);
      cout << "predict:" << endl;
      cout << UTrie::format(results);
    }
    if (mode == "all" || mode == "fuzzy") {
      UTrie::Entries results;
      trie.fuzzy_search_ex(input, 1, results);
      cout << "fuzzy:" << endl;
      cout << UTrie::format(results);
    }
    */
  }
  return 0;
}
