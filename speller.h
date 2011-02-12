#ifndef SPELLER_H
#define SPELLER_H

#include "trie.h"

namespace stakk {
class Speller {
 private:
  SimpleTrie &trie;
  int all;
  double editprob;
 public:
  Speller(SimpleTrie &trie_, int all_, double editprob_)
      : trie(trie_), all(all_), editprob(editprob_) {}

  string correct(string input, int threshold) {
    // shorter input should not be corrected
    if (input.length() < 4) return input;
    
    SimpleTrie::Entries results;
    trie.fuzzy_search_ex(input, threshold, results);

    // don't correct if there is no match
    if (results.size() == 0) return input;

    // find best candidate
    double prob = 0;
    string output = "";
    for (size_t i = 0; i < results.size(); i++) {
      for (size_t j = 0; j < results[i].values.size(); j++) {
        vector<string> fields = split(results[i].values[j], '\t');
        int freq = strtol(fields[1].c_str(), NULL, 0);
        int editnum = threshold - results[i].distance;
        double p = (double)freq / all * pow(editprob, editnum); 
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
