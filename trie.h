#ifndef TRIE_H
#define TRIE_H

#include "common.h"
#include "util.h"

namespace stakk {

template<typename CHAR>
class Trie {
 public:
  typedef basic_string<CHAR> String;
  struct Entry {
    String key;
    int distance;
    vector<String> values;
    Entry(String _key, int _distance, vector<String> _values) {
      key = _key;
      distance = _distance;
      values = _values;
    }
  };
  typedef vector<Entry> Entries;
  typedef pair<CHAR, Trie> Pair;
  typedef typename list<Pair>::iterator Itr;
 private:
  list<Pair> children;
  vector<String> values;

 public:
  void insert(String key, String value) {
    if (key.length() != 0) {
      CHAR first = key[0];
      String rest = key.substr(1);
      Trie *child = find(first);
      if (child == NULL) {
        children.push_back(Pair(first, Trie()));
        child = &(children.back().second);
      }
      child->insert(rest, value);
    } else {
      values.push_back(value);
    }
  }
  Trie *find(CHAR key) {
    for (Itr i = children.begin(); i != children.end(); i++) {
      if (key == i->first) {
        return &i->second;
      }
    }
    return NULL;
  }
  vector<String> *search(String key) {
    if (!key.length()) {
      if (values.size())
        return &values;
      else
        return NULL;
    }
    CHAR first = key[0];
    String rest = key.substr(1);
    Trie *child = find(first);
    if (child != NULL)
      return child->search(rest);
    return NULL;
  }
  void common_prefix_search(String query, String key, Entries &results) {
    if (values.size())
      results.push_back(Entry(key, 0, values));
    if (!query.length() || !children.size())
      return;
    Trie *child = find(query.at(0));
    if (child != NULL)
      child->common_prefix_search(query.substr(1), key+query.at(0), results);
  }
  void predictive_search(String query, String key, Entries &results) {
    if (query.length() <= key.length() && values.size())
      results.push_back(Entry(key, 0, values));
    if (!children.size())
      return;
    if (query.length() > key.length()) {
      CHAR c = query.at(key.length());
      Trie *child = find(c);
      if (child != NULL)
        child->predictive_search(query, key+c, results);
    } else {
      for (Itr i = children.begin(); i != children.end(); i++) {
        i->second.predictive_search(query, key+i->first, results);
      }
    }
  }
  void fuzzy_search(String query, String key, int distance, Entries &results) {
    if (!query.length() && values.size())
      results.push_back(Entry(key, distance, values));
    if (!children.size())
      return;

    // exact match
    if (query.length()) {
      Trie *child = find(query.at(0));
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
        CHAR c = query.at(0);
        query[0] = query.at(1);
        query[1] = c;
        fuzzy_search(query, key, distance-1, results);
      }
    }
  }
  void fuzzy_search_ex(String query, int distance, Entries &results) {
    Entries entries;
    fuzzy_search(query, empty, distance, entries);
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
  bool load(string filename, int key, CHAR separator) {
    basic_fstream<CHAR> ifs(filename.c_str());
    if (!ifs.is_open())
      return false;
    String line;
    while (getline(ifs, line)) {
      vector<String> splited = split(line, separator);
      insert(splited[key], line);
    }
    ifs.close();
    return true;
  }
  static String format(Entries entries) {
    String result;
    for (size_t i = 0; i < entries.size(); i++) {
      Entry entry = entries.at(i);
      for (size_t j = 0; j < entry.values.size(); j++) {
        basic_stringstream<CHAR> distance;
        distance << entry.distance;
        result += entry.key + separator
            + distance.str() + separator
            + entry.values.at(j) + linebreak;
      }
    }
    return result;
  }
  static String format(vector<String> values) {
    String result;
    for (size_t i = 0; i < values.size(); i++)
      result += values.at(i) + linebreak;
    return result;
  }
  static vector<String> split(String s, CHAR c) {
    vector<String> v;
    for (size_t p = 0; (p = s.find(c)) != s.npos; ) {
      v.push_back(s.substr(0, p));
      s = s.substr(p + 1);
    }
    v.push_back(s);
    return v;
  }
  static String separator;
  static String linebreak;
  static String empty;
};
typedef Trie<char> SimpleTrie;
typedef Trie<wchar_t> WideTrie;

template <> string SimpleTrie::separator("\t");
template <> string SimpleTrie::linebreak("\n");
template <> string SimpleTrie::empty("");

template <> wstring WideTrie::separator(L"\t");
template <> wstring WideTrie::linebreak(L"\n");
template <> wstring WideTrie::empty(L"");

}
#endif
