#include "trie.h"
using namespace stakk;

int main(int argc, char *argv[]) {
  int result;
  string filename = "data/dictionary.txt";
  string mode = "all";
  bool reverse = false;
  try { locale::global(locale("")); } catch (...) {}

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
  wcout << "inserting.." << endl;
  WideTrie trie;
  int field = reverse ? 4 : 0;
  if (!trie.load(filename, field, L'\t')) {
    cout << filename << " is not found." << endl;
    exit(0);
  }
  wcout << "input kana:" << endl;
  wstring input;
  while (wcin >> input) {
    if (mode == "all" || mode == "search") {
      vector<wstring> *result;
      result = trie.search(input);
      wcout << "search:" << endl;
      if (result)
        wcout << WideTrie::format(*result) << endl;
    }
    if (mode == "all" || mode == "common") {
      WideTrie::Entries results;
      trie.common_prefix_search(input, L"", results);
      wcout << "common:" << endl;
      wcout << WideTrie::format(results);
    }
    if (mode == "all" || mode == "predict") {
      WideTrie::Entries results;
      trie.predictive_search(input, L"", results);
      wcout << "predict:" << endl;
      wcout << WideTrie::format(results);
    }
    if (mode == "all" || mode == "fuzzy") {
      WideTrie::Entries results;
      trie.fuzzy_search_ex(input, 1, results);
      wcout << "fuzzy:" << endl;
      wcout << WideTrie::format(results);
    }
  }
  return 0;
}
