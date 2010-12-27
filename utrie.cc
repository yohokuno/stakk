#include "utrie.h"
using namespace stakk;

string format(Entries entries) {
    string result;
    for (size_t i = 0; i < entries.size(); i++) {
      Entry entry = entries.at(i);
      for (size_t j = 0; j < entry.values.size(); j++) {
        stringstream distance;
        distance << entry.distance;
        result += entry.key + " "
            + distance.str() + " "
            + entry.values.at(j) + "\n";
      }
    }
    return result;
}
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
  UTable utable;
  UTrie trie(utable);
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
        cout << join(*result, '\n') << endl;
      }
    }
    if (mode == "all" || mode == "common") {
      Entries results;
      trie.common_prefix_search(input, results);
      cout << "common:" << endl;
      cout << format(results);
    }
    if (mode == "all" || mode == "predict") {
      Entries results;
      trie.predictive_search(input, results);
      cout << "predict:" << endl;
      cout << format(results);
    }
    if (mode == "all" || mode == "fuzzy") {
      Entries results;
      trie.fuzzy_search_ex(input, 1, results);
      cout << "fuzzy:" << endl;
      cout << format(results);
    }
  }
  return 0;
}
