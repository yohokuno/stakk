#ifndef SPELLER_H
#define SPELLER_H

#include "trie.h"

namespace stakk {
class Speller {
 private:
  SimpleTrie &trie;
  int all;
 public:
  Speller(SimpleTrie &trie_, int all_)
      : trie(trie_), all(all_) {}
  string correct(string input, int threshold) {
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
    return output;
  }
};
}
#endif
