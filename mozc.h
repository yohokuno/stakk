#ifndef MOZC_H
#define MOZC_H

#define CONNECTION_SIZE 3033

struct Mozc {
    ListTrieWide &trie;
    unsigned short *connection;

    //lattice node
    struct Node {
        wstring yomi, word;
        unsigned short lid, rid, cost;
        unsigned int index, total, back;
        Node(vector<wstring> splited, wstring yomi_) {
            wstringstream(splited[0]) >> lid;
            wstringstream(splited[1]) >> rid;
            wstringstream(splited[2]) >> cost;
            word = splited[3];
            yomi = yomi_;
            index = total = back = 0;
        }
        Node(wstring word_, wstring yomi_, unsigned short lid_, unsigned short rid_, unsigned short cost_) {
            word = word_;
            yomi = yomi_;
            lid = lid_;
            rid = rid_;
            cost = cost_;
            index = total = back = 0;
        }
        wstring format() {
            wstringstream s;
            s << index << L" " << total << L" " << back;
            return yomi + L" " + word + L" " + s.str();
        }
    };

    //load connection
    Mozc(ListTrieWide &trie_, string filename) : trie(trie_) {
        ifstream ifs;
        ifs.open(filename.c_str());
        string line;
        getline(ifs, line);
        connection = new unsigned short[CONNECTION_SIZE * CONNECTION_SIZE];
        while (getline(ifs, line)) {
            vector<string> splited = split(line, ' ');
            unsigned short lid = atoi(splited[0].c_str());
            unsigned short rid = atoi(splited[1].c_str());
            unsigned short cost = atoi(splited[2].c_str());
            connection[lid * CONNECTION_SIZE + rid] = cost;
        }
        ifs.close();
    }
    ~Mozc() {
        delete[] connection;
    }
    
    //kana kanji conversion
    vector<wstring> convert(wstring input, bool debug=false) {
        vector<wstring> results;

        //create lattice
        vector<vector<Node> > lattice;
        lattice.clear();
        lattice.resize(input.length() + 2);
        Node bos(L"BOS", L"S", 0, 0, 0);
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
                int index = i - lattice[i][j].yomi.length();
                if (index < 0 || lattice[index].size() == 0)
                    continue;
                int best_score = -1;
                int best_index = -1;
                for (int k = 0; k < lattice[index].size(); k++) {
                    int pos = lattice[index][k].rid * CONNECTION_SIZE + lattice[i][j].lid;
                    int score = lattice[index][k].total + connection[pos];
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
        int current = lattice.size()-2;
        int position = lattice[lattice.size()-1][0].back;
        while (current > 0 && lattice[current].size() > 0) {
            Node &node = lattice[current][position];
            results.insert(results.begin(), node.word);
            position = node.back;
            current -= node.yomi.length();
        }
        return results;
    }
};
#endif
