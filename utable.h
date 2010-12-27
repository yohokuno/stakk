#ifndef UTABLE_H
#define UTABLE_H

#include "common.h"
#include "utf8.h"

namespace stakk {

typedef basic_string<uint16_t> ustring;

class UTable {
 private:
  vector<string> u2s;
  map<string, uint16_t> s2u;
 public:
  UTable() {
    u2s.push_back("NULL");
  }
  ustring decode(string input) {
    ustring result;
    for (size_t i = 0; i < input.length(); i+= utf8_len(input[i])) {
      string c = input.substr(i, utf8_len(input[i]));
      uint16_t id = s2u[c];
      if (id == 0) {
        u2s.push_back(c);
        id = u2s.size() - 1;
        s2u[c] = id;
      }
      result.push_back(id);
    }
    return result;
  }
  string encode(ustring input) {
    string result;
    for (size_t i = 0; i < input.length(); i++) {
      result += u2s[input[i]];
    }
    return result;
  }
  size_t size() {
    if (u2s.size()-1 == s2u.size()) {
      return s2u.size();
    }
    return 0;
  }
};
}
#endif
