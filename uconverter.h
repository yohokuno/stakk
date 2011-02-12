#ifndef CONVERTER_H
#define CONVERTER_H

#include "common.h"
#include "util.h"
#include "utrie.h"
#include "connection.h"

namespace stakk {
class Converter {
 private:
  UTrie &trie;
  Connection &connection;
  Definition &definition;

 public:
  //lattice node
  struct Node {
    string yomi, word, key;
    unsigned short lid, rid, cost;
    unsigned int index, total, back;
    Node(vector<string> splited, string key_) {
      yomi = splited[0];
      stringstream(splited[1]) >> lid;
      stringstream(splited[2]) >> rid;
      stringstream(splited[3]) >> cost;
      word = splited[4];
      key = key_;
      index = back = 0;
      total = INT_MAX / 2 - 1;
    }
    Node() {
      word = yomi = key = "S";
      lid = rid = cost = index = back = total = 0;
    }
    string format() {
      stringstream s;
      s << index << " " << cost << " " << total << " " << back
          << " " << lid << " " << rid;
      return yomi + " " + word + " " + s.str();
    }
  };
  const Node bos;

  //initialize references
  Converter(UTrie &trie_, Connection &connection_, Definition &definition_)
      : trie(trie_), connection(connection_), definition(definition_){}

  //convert from kana to kanji
  vector<Node> convert(string input, bool debug=false) {
    //create lattice
    vector<vector<Node> > lattice;
    lattice.clear();
    lattice.resize(input.length() + 2);
    lattice[0].push_back(bos);
    lattice[lattice.size()-1].push_back(bos);

    //common prefix search
    for (size_t i = 0; i < input.length(); i++) {
      Entries entries;
      string query = input.substr(i);
      trie.common_prefix_search(query, entries);
      for (size_t j = 0; j < entries.size(); j++) {
        for (size_t k = 0; k < entries[j].values.size(); k++) {
          Node node(split(entries[j].values[k], '\t'), entries[j].key);
          int index = i + entries[j].key.length();
          node.index = lattice[index].size();
          lattice[index].push_back(node);
        }
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
          unsigned short lid = lattice[index][k].rid;
          unsigned short rid = lattice[i][j].lid;
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
          cout << lattice[i][j].format() << endl;
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
  string format(vector<Node> nodes, string output) {
    string result = "";
    if (output == "mecab") {
      for (size_t i = 0; i < nodes.size(); i++) {
        Node &node = nodes[i];
        string lid = definition.get(node.lid);
        string rid = definition.get(node.rid);
        result += node.word + "\t" + node.yomi + "\t" + lid + "\t" + rid + "\n";
      }
      result += "EOS\n";
    } else if (output == "wakati") {
      for (size_t i = 0; i < nodes.size(); i++) {
        result += nodes[i].word + " ";
      }
      result += "\n";
    } else if (output == "yomi") {
      for (size_t i = 0; i < nodes.size(); i++) {
        result += nodes[i].yomi;
      }
      result += "\n";
    }
    return result;
  }
};
}
#endif
