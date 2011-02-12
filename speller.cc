#include "speller.h"
using namespace stakk;

int main(int argc, char *argv[]) {
  int result;
  string filename = "data/query.txt";
  string mode;
  int threshold = 2;
  int all = 36389567;
  double edit = 2.0;

  while((result = getopt(argc, argv, "f:m:r")) != -1) {
    switch(result) {
      case 'f':
        filename = optarg;
        break;
      case 'm':
        mode = optarg;
        break;
      case 't':
        threshold = strtol(optarg, NULL, 0);
        break;
      case 'a':
        all = strtol(optarg, NULL, 0);
        break;
      case 'e':
        edit = strtof(optarg, NULL);
        break;
    }
  }
  cout << "initializing.." << endl;
  SimpleTrie trie;
  if (!trie.load(filename, 0, '\t')) {
    cout << filename << " is not found." << endl;
    exit(0);
  }
  Speller speller(trie, all, edit);

  cout << "input:" << endl;
  string input;
  while (getline(cin, input)) {
    cout << speller.correct(input, threshold) << endl;
  }
  return 0;
}
