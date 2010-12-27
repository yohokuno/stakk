#ifndef TRIE_SERVER_H
#define TRIE_SERVER

#include "common.h"
#include "util.h"
#include "trie.h"
#include "server.h"

namespace stakk {
class TrieServer : public Server {
 protected:
  string filename, mode;
  WideTrie &trie;
  int threshold;

 public:
  //init trie reference
  TrieServer(int port_, WideTrie &trie_) : Server(port_), trie(trie_) {}

  //parse request path
  wstring parse_path(vector<string> &path) {
    if (path.size() != 4)
      return L"";
    mode = path[2];
    if (mode == "fuzzy") {
      if (path.size() > 4)
        threshold = atoi(path[4].c_str());
      else
        threshold = 1;
    }
    return widen(urldecode(path[3]));
  }

  //get result
  virtual wstring get_result(vector<string> path) {
    wstring response = L"";
    wstring input = parse_path(path);
    if (mode == "search") {
      vector<wstring> *result;
      result = trie.search(input);
      if (result)
        response += WideTrie::format(*result);
    } else if (mode == "common") {
      WideTrie::Entries results;
      trie.common_prefix_search(input, L"", results);
      response +=  WideTrie::format(results);
    } else if (mode == "predict") {
      WideTrie::Entries results;
      trie.predictive_search(input, L"", results);
      response += WideTrie::format(results);
    } else if (mode == "fuzzy") {
      WideTrie::Entries results;
      trie.fuzzy_search_ex(input, threshold, results);
      response += WideTrie::format(results);
    }
    return response;
  }
};
}
#endif
