#ifndef SPELL_H
#define SPELL_H

struct SpellCorrector {
    struct Entry {
        wstring yomi, word;
        int distance, lid, rid, cost, rank;
        Entry(wstring yomi_, wstring word_, int distance_, int lid_, int rid_, int cost_, int rank_) {
            yomi = yomi_;
            word = word_;
            distance = distance_;
            lid = lid_;
            rid = rid_;
            cost = cost_;
            rank = rank_;
        }
        wstring format() {
            return yomi + L"\t" + word;
        }
    };

    ListTrieWide trie;
    short connection[3033];

    SpellCorrector(string dictionary, string connection) {
        //load dictionary
        {
            wifstream ifs(dictionary.c_str());
            wstring line, input;
            wcout << "loading dictionary" << endl;
            while (getline(ifs, line)) {
                size_t pos = line.find('\t');
                if (pos == wstring::npos)
                    continue;
                wstring key = line.substr(0, pos);
                wstring value = line.substr(pos+1);
                trie.insert(key, value);
            }
            ifs.close();
        }

        //load connection
        {
            ifstream ifs;
            ifs.open(connection.c_str());
            string line;
            getline(ifs, line);
            wcout << "loading connection" << endl;
            while (getline(ifs, line)) {
                vector<string> splited = split(line, ' ');
                int lid = atoi(splited[0].c_str());
                int rid = atoi(splited[1].c_str());
                int cost = atoi(splited[2].c_str());
                if (lid != 0) break;
                this->connection[rid] = cost;
            }
            ifs.close();
        }
    }

    void correct(wstring input, int threshold, vector<Entry> &results) {
        ListTrieWide::Entries entries;
        trie.fuzzy_search_ex(input, threshold, entries);
        for (int i = 0; i < entries.size(); i++) {
            wstring yomi = entries[i].yomi;
            int distance = entries[i].distance;
            for (int j = 0; j < entries[i].values.size(); j++) {
                vector<wstring> splited = split_w(entries[i].values[j], L'\t');
                int lid, rid, cost;
                wstringstream(splited[0]) >> lid;
                wstringstream(splited[1]) >> rid;
                wstringstream(splited[2]) >> cost;
                wstring word = splited[3];
                int total = cost + connection[lid];
                int rank = total + (threshold-distance) * 5000;
                Entry entry(yomi, word, distance, lid, rid, cost, rank);
                results.push_back(entry);
            }
        }
        sort(results.begin(), results.end());
    }
};
bool operator<(const SpellCorrector::Entry& left, const SpellCorrector::Entry& right) {
    return left.rank < right.rank ;
}
#endif
