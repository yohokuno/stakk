#include "trie.h"
using namespace stakk;

int main(int argc, char *argv[]) {
  int result;
  string filename = "data/query.txt";
  string mode;
  int threshold = 2;
  int all = 36389567;

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
    }
  }
  cout << "inserting.." << endl;
  SimpleTrie trie;
  if (!trie.load(filename, 0, '\t')) {
    cout << filename << " is not found." << endl;
    exit(0);
  }
  cout << "input:" << endl;
  string input;
  while ((cin >> input)) {
    SimpleTrie::Entries results;
    trie.fuzzy_search_ex(input, threshold, results);

    double prob = 0;
    string output = "";
    for (int i = 0; i < results.size(); i++) {
      for (int j = 0; j < results[i].values.size(); j++) {
        vector<string> fields = split(results[i].values[j], '\t');
        int freq = strtol(fields[1].c_str(), NULL, 0);
        int edit = results[i].distance;
        double p = (double)freq / all / pow(2, edit); 
        if (p > prob) {
          prob = p;
          output = results[i].key;
        }
      }
    }
    cout << output << endl;
    //cout << SimpleTrie::format(results);
  }
  return 0;
}
