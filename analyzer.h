#ifndef ANALYZER_H
#define ANALYZER_H

struct Analyzer {
    ListTrieWide &trie;
    Connection &connection;
    Definition &definition;

    //lattice node
    struct Node {
        wstring yomi, word;
        unsigned short lid, rid, cost;
        unsigned int index, total, back;
        Node(vector<wstring> splited, wstring word_) {
            yomi = splited[0];
            wstringstream(splited[1]) >> lid;
            wstringstream(splited[2]) >> rid;
            wstringstream(splited[3]) >> cost;
            word = splited[4];
            index = back = 0;
            total = INT_MAX / 2;
        }
        Node() {
            word = yomi = L"S";
            lid = rid = cost = index = back = total = 0;
        }
        wstring format() {
            wstringstream s;
            s << index << L" " << total << L" " << back;
            return yomi + L" " + word + L" " + s.str();
        }
    };
    const Node bos;

    //initialize references
    Analyzer(ListTrieWide &trie_, Connection &connection_, Definition &definition_)
        : trie(trie_), connection(connection_), definition(definition_){}

    //morphological analyze
    vector<Node> analyze(wstring input, bool debug=false) {
        //create lattice
        vector<vector<Node> > lattice;
        lattice.clear();
        lattice.resize(input.length() + 2);
        lattice[0].push_back(bos);
        lattice[lattice.size()-1].push_back(bos);

        //common prefix search
        for (int i = 0; i < input.length(); i++) {
            ListTrieWide::Entries entries;
            wstring query = input.substr(i);
            trie.common_prefix_search(query, L"", entries);
            for (int j = 0; j < entries.size(); j++) {
                for (int k = 0; k < entries[j].values.size(); k++) {
                    Node node(split_w(entries[j].values[k], L'\t'), entries[j].key);
                    int index = i + entries[j].key.length();
                    node.index = lattice[index].size();
                    lattice[index].push_back(node);
                }
            }
        }
        //forward search
        for (int i = 0; i < lattice.size(); i++) {
            for (int j = 0; j < lattice[i].size(); j++) {
                int index = i - lattice[i][j].word.length();
                if (index < 0 || lattice[index].size() == 0)
                    continue;
                int best_score = -1;
                int best_index = -1;
                for (int k = 0; k < lattice[index].size(); k++) {
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
            for (int i = 0; i < lattice.size(); i++) {
                for (int j = 0; j < lattice[i].size(); j++) {
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
            current -= node.word.length();
        }
        return nodes;
    }
    wstring format(vector<Node> nodes, string output) {
        wstring result = L"";
        if (output == "mecab") {
            for (int i = 0; i < nodes.size(); i++) {
                Node &node = nodes[i];
                wstring lid = definition.get(node.lid);
                wstring rid = definition.get(node.rid);
                result += node.word + L"\t" + node.yomi + L"\t" + lid + L"\t" + rid + L"\n";
            }
            result += L"EOS\n";
        } else if (output == "wakati") {
            for (int i = 0; i < nodes.size(); i++) {
                result += nodes[i].word + L" ";
            }
            result += L"\n";
        } else if (output == "yomi") {
            for (int i = 0; i < nodes.size(); i++) {
                result += nodes[i].yomi;
            }
            result += L"\n";
        }
        return result;
    }
};
#endif
