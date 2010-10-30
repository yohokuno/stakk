#ifndef STAKK_H
#define STAKK_H

#define CONNECTION_SIZE 3033

struct Stakk {
    ListTrieWide &trie;
    unsigned short *connection;

    //result entry
    struct Entry {
        wstring yomi, word;
        int lid, rid, cost, rank;
        Entry(vector<wstring> splited, wstring yomi_) {
            wstringstream(splited[0]) >> lid;
            wstringstream(splited[1]) >> rid;
            wstringstream(splited[2]) >> cost;
            word = splited[3];
            yomi = yomi_;
        }
        wstring format() {
            return yomi + L"\t" + word;
        }
    };
    //initalize
    Stakk(ListTrieWide &trie_, unsigned short *connection_)
        : trie(trie_), connection(connection_) {}

    //load connection
    Stakk(ListTrieWide &trie_, string filename) : trie(trie_) {
        connection = load_connection(filename);
    }
    //load connection
    static unsigned short *load_connection(string filename) {
        unsigned short *connection = new unsigned short[CONNECTION_SIZE];
        ifstream ifs;
        ifs.open(filename.c_str());
        string line;
        getline(ifs, line);
        while (getline(ifs, line)) {
            vector<string> splited = split(line, ' ');
            int lid = atoi(splited[0].c_str());
            int rid = atoi(splited[1].c_str());
            int cost = atoi(splited[2].c_str());
            if (lid != 0) break;
            connection[rid] = cost;
        }
        ifs.close();
        return connection;
    }

    //spell correct
    void correct(wstring input, int threshold, vector<Entry> &results) {
        ListTrieWide::Entries entries;
        trie.fuzzy_search_ex(input, threshold, entries);
        for (int i = 0; i < entries.size(); i++) {
            wstring yomi = entries[i].key;
            int distance = entries[i].distance;
            for (int j = 0; j < entries[i].values.size(); j++) {
                vector<wstring> splited = split_w(entries[i].values[j], L'\t');
                Entry entry(splited, yomi);
                int total = entry.cost + connection[entry.lid];
                entry.rank = total + (threshold-distance) * 5000;
                results.push_back(entry);
            }
        }
        sort(results.begin(), results.end());
    }

    //predictive input
    void predict(wstring input, vector<Entry> &results) {
        ListTrieWide::Entries entries;
        trie.predictive_search(input, L"", entries);
        for (int i = 0; i < entries.size(); i++) {
            wstring yomi = entries[i].key;
            int length = yomi.length()-input.length();
            for (int j = 0; j < entries[i].values.size(); j++) {
                vector<wstring> splited = split_w(entries[i].values[j], L'\t');
                Entry entry(splited, yomi);
                int total = entry.cost + connection[entry.lid];
                entry.rank = total - int(1000.0 * log(1+length));
                results.push_back(entry);
            }
        }
        sort(results.begin(), results.end());
    }
};
//for result sort
bool operator<(const Stakk::Entry& left, const Stakk::Entry& right) {
    return left.rank < right.rank ;
}
#endif
