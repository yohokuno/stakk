#ifndef SPELL_H
#define SPELL_H

#include "trie.h"
#include "util.h"

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
        /*
           url = self.base + quote(input)
           results = []
           for line in urlopen(url):
           (yomi, distance, lid, rid, cost, word) = line.strip().split("\t", 5)
           lid, rid, cost, distance = int(lid), int(rid), int(cost), int(distance)
           yomi, word = unicode(yomi, 'utf-8'), unicode(word, 'utf-8')
           results.append((yomi, distance, lid, rid, cost, word))

           entries = []
           for yomi, distance, lid, rid, cost, word in results:
           total = cost + self.connection[lid]
           rank = total + (threshold-distance) * 5000
           entry = Entry(yomi, distance, word, lid, rid, cost, rank)
           entries.append(entry)
           entries.sort(key=lambda x:x.rank)
         */
    }
};
#endif
