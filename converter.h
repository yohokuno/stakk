#ifndef CONVERTER_H
#define CONVERTER_H

#include "common.h"
#include "util.h"
#include "trie.h"
#include "connection.h"

namespace stakk {
class Converter {
 private:
  WideTrie &trie;
  Connection &connection;
  Definition &definition;

 public:
  //lattice node
  struct Node {
    wstring yomi, word, key;
    unsigned int lid, rid, cost;
    unsigned int index, total, back;
    Node(vector<wstring> splited, wstring key_) {
      yomi = splited[0];
      wstringstream(splited[1]) >> lid;
      wstringstream(splited[2]) >> rid;
      wstringstream(splited[3]) >> cost;
      word = splited[4];
      key = key_;
      index = back = 0;
      total = 0xffffff;
    }
    Node(wstring key_) {
      yomi = key_;
      word = key_;
      lid = 0;
      rid = 0;
      cost = 0xffff;
      key = key_;
      index = back = 0;
      total = 0xffffff;
    }
    Node() {
      word = yomi = key = L"S";
      lid = rid = cost = index = back = total = 0;
    }
    wstring format() {
      wstringstream s;
      s << index << L" " << cost << L" " << total << L" " << back
          << L" " << lid << L" " << rid;
      return yomi + L" " + word + L" " + s.str();
    }
  };
  const Node bos;

  //initialize references
  Converter(WideTrie &trie_, Connection &connection_, Definition &definition_)
      : trie(trie_), connection(connection_), definition(definition_){}

  //convert from kana to kanji
  vector<Node> convert(wstring input, bool debug=false) {
    //create lattice
    vector<vector<Node> > lattice;
    lattice.clear();
    lattice.resize(input.length() + 2);
    lattice[0].push_back(bos);
    lattice[lattice.size()-1].push_back(bos);

    //common prefix search
    for (size_t i = 0; i < input.length(); i++) {
      WideTrie::Entries entries;
      wstring query = input.substr(i);
      trie.common_prefix_search(query, L"", entries);
      for (size_t j = 0; j < entries.size(); j++) {
        for (size_t k = 0; k < entries[j].values.size(); k++) {
          Node node(split_w(entries[j].values[k], L'\t'), entries[j].key);
          int index = i + entries[j].key.length();
          node.index = lattice[index].size();
          lattice[index].push_back(node);
        }
      }
      if (entries.size() == 0) {
        wstring key;
        key += query[0];
        Node node(key);
        int index = i + 1;
        node.index = lattice[index].size();
        lattice[index].push_back(node);
      }
    }
    //forward search
    for (size_t i = 0; i < lattice.size(); i++) {
      for (size_t j = 0; j < lattice[i].size(); j++) {
        int index = i - lattice[i][j].key.length();
        if (index < 0 || lattice[index].size() == 0)
          continue;
        int best_score = -1, best_index = -1;
        for (size_t k = 0; k < lattice[index].size(); k++) {
          unsigned int lid = lattice[index][k].rid;
          unsigned int rid = lattice[i][j].lid;
          int transition = connection.get(lid, rid);
          int score = lattice[index][k].total + transition;
          if (best_score == -1 || score < best_score) {
            best_index = lattice[index][k].index;
            best_score = score;
          }
        }
        lattice[i][j].total = lattice[i][j].cost + best_score;
        lattice[i][j].back = best_index;
      }
    }
    //debug
    if (debug) {
      for (size_t i = 0; i < lattice.size(); i++) {
        for (size_t j = 0; j < lattice[i].size(); j++) {
          wcout << lattice[i][j].format() << endl;
        }
      }
    }
    //back trace
    vector<Node> nodes;
    int current = lattice.size()-2;
    int position = lattice[lattice.size()-1][0].back;
    while (current > 0 && lattice[current].size() > 0) {
      Node &node = lattice[current][position];
      nodes.insert(nodes.begin(), node);
      position = node.back;
      current -= node.key.length();
    }
    return nodes;
  }
  wstring format(vector<Node> nodes, string output) {
    wstring result = L"";
    if (output == "mecab") {
      for (size_t i = 0; i < nodes.size(); i++) {
        Node &node = nodes[i];
        wstring lid = definition.get(node.lid);
        wstring rid = definition.get(node.rid);
        result += node.word + L"\t" + node.yomi + L"\t" + lid + L"\t" + rid + L"\n";
      }
      result += L"EOS\n";
    } else if (output == "wakati") {
      for (size_t i = 0; i < nodes.size(); i++) {
        result += nodes[i].word + L" ";
      }
      result += L"\n";
    } else if (output == "yomi") {
      for (size_t i = 0; i < nodes.size(); i++) {
        result += nodes[i].yomi;
      }
      result += L"\n";
    }
    return result;
  }
};
}
#endif
