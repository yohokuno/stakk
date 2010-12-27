#ifndef TRIE_H
#define TRIE_H

#include "common.h"
#include "util.h"
#include "ustring.h"

namespace stakk {

class UTrie {
 private:
  typedef pair<uint16_t, UTrie> Pair;
  typedef list<Pair>::iterator Itr;
  list<Pair> children;
  vector<string> values;
  static Ustring u;

 public:
  struct Entry {
    string key;
    int distance;
    vector<string> values;
    Entry(string _key, int _distance, vector<string> _values) {
      key = _key;
      distance = _distance;
      values = _values;
    }
  };
  typedef vector<Entry> Entries;

  void insert(ustring key, string value) {
    if (key.length() != 0) {
      uint16_t first = key[0];
      ustring rest = key.substr(1);
      UTrie *child = find(first);
      if (child == NULL) {
        children.push_back(Pair(first, UTrie()));
        child = &(children.back().second);
      }
      child->insert(rest, value);
    } else {
      values.push_back(value);
    }
  }
  UTrie *find(uint16_t key) {
    for (Itr i = children.begin(); i != children.end(); i++) {
      if (key == i->first) {
        return &i->second;
      }
    }
    return NULL;
  }
  bool load(string filename, int key, char separator) {
    ifstream ifs(filename.c_str());
    if (!ifs.is_open())
      return false;
    string line;
    while (getline(ifs, line)) {
      vector<string> splited = split(line, separator);
      ustring k = u.decode(splited[key]);
      insert(k, line);
    }
    ifs.close();
    return true;
  }
  vector<string> *search(string key) {
    ustring ukey = u.decode(key);
    return search(ukey);
  }
  vector<string> *search(ustring key) {
    if (!key.length()) {
      if (values.size())
        return &values;
      else
        return NULL;
    }
    uint16_t first = key[0];
    ustring rest = key.substr(1);
    UTrie *child = find(first);
    if (child != NULL)
      return child->search(rest);
    return NULL;
  }
  void common_prefix_search(string query, Entries &results) {
    ustring uquery = u.decode(query);
    ustring ukey;
    common_prefix_search(uquery, ukey, results);
  }
  void common_prefix_search(ustring query, ustring key, Entries &results) {
    if (values.size())
      results.push_back(Entry(u.encode(key), 0, values));
    if (!query.length() || !children.size())
      return;
    UTrie *child = find(query.at(0));
    if (child != NULL) {
      ustring next = key + query.at(0);
      child->common_prefix_search(query.substr(1), next, results);
    }
  }
  void predictive_search(string query, string key, Entries &results) {
    ustring uquery = u.decode(query);
    ustring ukey;
    predictive_search(uquery, ukey, results);
  }
  void predictive_search(ustring query, ustring key, Entries &results) {
    if (query.length() <= key.length() && values.size())
      results.push_back(Entry(u.encode(key), 0, values));
    if (!children.size())
      return;
    if (query.length() > key.length()) {
      uint16_t c = query.at(key.length());
      UTrie *child = find(c);
      if (child != NULL)
        child->predictive_search(query, key+c, results);
    } else {
      for (Itr i = children.begin(); i != children.end(); i++) {
        i->second.predictive_search(query, key+i->first, results);
      }
    }
  }
  void fuzzy_search(string query, int distance, Entries &results) {
    ustring uquery = u.decode(query);
    ustring ukey;
    fuzzy_search(uquery, ukey, distance, results);
  }
  void fuzzy_search(ustring query, ustring key, int distance, Entries &results) {
    if (!query.length() && values.size())
      results.push_back(Entry(u.encode(key), distance, values));
    if (!children.size())
      return;

    // exact match
    if (query.length()) {
      UTrie *child = find(query.at(0));
      if (child != NULL)
        child->fuzzy_search(query.substr(1), key+query.at(0), distance, results);
    }
    // edit operation
    if (distance) {
      // insert
      for (Itr i = children.begin(); i != children.end(); i++) {
        i->second.fuzzy_search(query, key+i->first, distance-1, results);
      }
      if (query.length()) {
        // delete
        fuzzy_search(query.substr(1), key, distance-1, results);

        // substitute
        for (Itr i = children.begin(); i != children.end(); i++) {
          if (i->first != query.at(0))
            i->second.fuzzy_search(query.substr(1), key+i->first, distance-1, results);
        }
      }
      // transpose
      if (query.length() > 1) {
        uint16_t c = query.at(0);
        query[0] = query.at(1);
        query[1] = c;
        fuzzy_search(query, key, distance-1, results);
      }
    }
  }
  void fuzzy_search_ex(string query, int distance, Entries &results) {
    Entries entries;
    fuzzy_search(query, distance, entries);
    for (size_t i = 0; i < entries.size(); i++) {
      Entry entry = entries.at(i);
      bool flag = false;
      for (size_t j = 0; j < results.size(); j++) {
        if (entry.key == results.at(j).key) {
          if (entry.distance > results.at(j).distance)
            results[j] = entry;
          flag = true;
          break;
        }
      }
      if (!flag) {
        results.push_back(entry);
      }
    }
  }
};
}
#endif
