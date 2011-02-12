#ifndef SPELLER_H
#define SPELLER_H

#include "trie.h"

namespace stakk {
class Speller {
 private:
  SimpleTrie &trie;
  int all;
  double edit;
 public:
  Speller(SimpleTrie &trie_, int all_, double edit_)
      : trie(trie_), all(all_), edit(edit_) {}
  string correct(string input, int threshold) {
    SimpleTrie::Entries results;
    trie.fuzzy_search_ex(input, threshold, results);

    if (results.size() == 0)
      return input;

    double prob = 0;
    string output = "";
    for (size_t i = 0; i < results.size(); i++) {
      for (size_t j = 0; j < results[i].values.size(); j++) {
        vector<string> fields = split(results[i].values[j], '\t');
        int freq = strtol(fields[1].c_str(), NULL, 0);
        int distance = results[i].distance;
        double p = (double)freq / all / pow(edit, distance); 
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
