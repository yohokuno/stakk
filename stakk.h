#ifndef STAKK_H
#define STAKK_H

struct Stakk {
    ListTrieWide &trie;
    Connection  &connection;

    //result entry
    struct Entry {
        wstring yomi, word;
        int lid, rid, cost, rank;
        Entry(vector<wstring> splited, wstring yomi_) {
            yomi = splited[0];
            wstringstream(splited[1]) >> lid;
            wstringstream(splited[2]) >> rid;
            wstringstream(splited[3]) >> cost;
            word = splited[4];
        }
        wstring format() {
            return yomi + L"\t" + word;
        }
    };
    //initalize
    Stakk(ListTrieWide &trie_, Connection &connection_)
        : trie(trie_), connection(connection_)
    {
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
                int total = entry.cost + connection.get(0, entry.lid);
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
                int total = entry.cost + connection.get(0, entry.lid);
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
