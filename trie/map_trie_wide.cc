#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
using namespace std;

struct MapTrieWide {
    typedef map<wchar_t, MapTrieWide> Map;
    typedef Map::iterator Itr;
    typedef Map::value_type Pair;
    typedef pair<wstring, vector<wstring> > Entry;
    typedef vector<Entry> Entries;
    Map children;
    vector<wstring> values;

    void insert(wstring key, wstring value) {
        if (key.length() != 0) {
            wchar_t first = key[0];
            wstring rest = key.substr(1);
            Itr itr = children.find(first);
            if (itr == children.end())
                itr = children.insert(itr, Pair(first, MapTrieWide()));
            itr->second.insert(rest, value);
        } else {
            values.push_back(value);
        }
    }
    vector<wstring> *search(wstring key) {
        if (!key.length())
            if (values.size())
                return &values;
            else
                return NULL;
        wchar_t first = key[0];
        wstring rest = key.substr(1);
        Itr itr = children.find(first);
        if (itr != children.end())
            return itr->second.search(rest);
        return NULL;
    }
    void common_prefix_search(wstring query, wstring key, Entries &results) {
        if (values.size())
            results.push_back(Entry(key, values));
        if (!query.length() || !children.size())
            return;
        Itr itr = children.find(query.at(0));
        if (itr != children.end())
            itr->second.common_prefix_search(query.substr(1), key+query.at(0), results);
    }
    void predictive_search(wstring query, wstring key, Entries &results) {
        if (query.length() <= key.length() && values.size())
            results.push_back(Entry(key, values));
        if (!children.size())
            return;
        if (query.length() > key.length()) {
            wchar_t c = query.at(key.length());
            Itr itr = children.find(c);
            if (itr != children.end())
                itr->second.predictive_search(query, key+c, results);
        } else {
            for (Itr itr = children.begin(); itr != children.end(); itr++)
                itr->second.predictive_search(query, key+itr->first, results);
        }
    }
    void fuzzy_search(wstring query, wstring key, int distance, Entries &results) {
        if (!query.length() && values.size())
            results.push_back(Entry(key, values));
        if (!children.size())
            return;

        // normal match
        if (query.length()) {
            Itr itr = children.find(query.at(0));
            if (itr != children.end())
                itr->second.fuzzy_search(query.substr(1), key+query.at(0), distance, results);
        }
        // edit operation
        if (distance) {
            // insert
            for (Itr itr = children.begin(); itr != children.end(); itr++)
                itr->second.fuzzy_search(query, key+itr->first, distance-1, results);
            if (query.length()) {
                // delete
                fuzzy_search(query.substr(1), key, distance-1, results);

                // substitute
                for (Itr itr = children.begin(); itr != children.end(); itr++) {
                    if (itr->first != query.at(0))
                        itr->second.fuzzy_search(query.substr(1), key+itr->first, distance-1, results);
                }
            }
            // transpose
            if (query.length() > 1) {
                wchar_t c = query.at(0);
                query[0] = query.at(1);
                query[1] = c;
                fuzzy_search(query, key, distance-1, results);
            }
        }
    }
    void fuzzy_search_ex(wstring query, int distance, Entries &results) {
        Entries entries;
        fuzzy_search(query, L"", distance, entries);
        for (int i = 0; i < entries.size(); i++) {
            Entry entry = entries.at(i);
            bool flag = false;
            for (int j = 0; j < results.size(); j++) {
                if (entry.first == results.at(j).first) {
                    flag = true;
                }
            }
            if (!flag) {
                results.push_back(entry);
            }
        }
    }
    void display(wstring key=L"") {
        if (values.size() > 0)
            wcout << key << "\t" << format(values) << endl;
        for (Itr itr = children.begin(); itr != children.end(); itr++)
            itr->second.display(key + itr->first);
    }
    static wstring format(Entries entries) {
        wstring result = L"";
        for (int i = 0; i < entries.size(); i++) {
            MapTrieWide::Entry entry = entries.at(i);
            result += entry.first + L"\t";
            for (int j = 0; j < entry.second.size(); j++)
                result += entry.second.at(j) + L" ";
            result += L"\n";
        }
        return result;
    }
    static wstring format(vector<wstring> entries) {
        wstring result = L"";
        for (int i = 0; i < entries.size(); i++)
            result += entries.at(i) + L" ";
        return result;
    }
};

int main(int argc, char *argv[]) {
    int result;
    string filename, mode = "all";
    locale::global(locale(""));

    while((result = getopt(argc, argv, "f:m:")) != -1) {
        switch(result) {
            case 'f':
                filename = optarg;
                break;
            case 'm':
                mode = optarg;
                break;
        }
    }
    if (filename.length() > 0) {
        MapTrieWide trie;
        wifstream ifs(filename.c_str());
        wstring line, input;
        wcout << "inserting.." << endl;
        while (getline(ifs, line)) {
            size_t pos = line.find('\t');
            if (pos == wstring::npos)
                continue;
            wstring key = line.substr(0, pos);
            wstring value = line.substr(pos+1);
            trie.insert(key, value);
        }
        wcout << "input kana:" << endl;
        while (getline(wcin, input)) {
            if (mode == "all" || mode == "search") {
                vector<wstring> *result;
                result = trie.search(input);
                wcout << "search:" << endl;
                if (result)
                    wcout << MapTrieWide::format(*result) << endl;
            }
            if (mode == "all" || mode == "common") {
                MapTrieWide::Entries results;
                trie.common_prefix_search(input, L"", results);
                wcout << "common:" << endl;
                wcout << MapTrieWide::format(results);
            }
            if (mode == "all" || mode == "predict") {
                MapTrieWide::Entries results;
                trie.predictive_search(input, L"", results);
                wcout << "predict:" << endl;
                wcout << MapTrieWide::format(results);
            }
            if (mode == "all" || mode == "fuzzy") {
                MapTrieWide::Entries results;
                trie.fuzzy_search_ex(input, 2, results);
                wcout << "fuzzy:" << endl;
                wcout << MapTrieWide::format(results);
            }
        }
    } else { 
        MapTrieWide trie;
        trie.insert(L"tree", L"value01");
        trie.insert(L"trie", L"value02");
        trie.insert(L"try", L"value03");
        trie.insert(L"tree", L"value04");
        trie.insert(L"tr", L"value05");
        trie.insert(L"trees", L"value06");
        trie.insert(L"tere", L"value06");
        trie.display();

        { // search
            vector<wstring> *result;
            result = trie.search(L"tree");
            if (result)
                wcout << "search:\n" << MapTrieWide::format(*result) << endl;
        }
        { // common prefix search
            MapTrieWide::Entries results;
            trie.common_prefix_search(L"tree", L"", results);
            wcout << "common:\n" << MapTrieWide::format(results);
        }
        { // predictive search
            MapTrieWide::Entries results;
            trie.predictive_search(L"tre", L"", results);
            wcout << "predict:\n" << MapTrieWide::format(results);
        }
        { // fuzzy search
            MapTrieWide::Entries results;
            trie.fuzzy_search(L"tree", L"", 1, results);
            wcout << "fuzzy:\n" << MapTrieWide::format(results);
        }
        { // fuzzy search 2
            MapTrieWide::Entries results;
            trie.fuzzy_search_ex(L"tree", 2, results);
            wcout << "fuzzy2:\n" << MapTrieWide::format(results);
        }
        {  // japanese test
            MapTrieWide trie;
            trie.insert(L"にほんご", L"日本語");
            trie.insert(L"にほんじん", L"日本人");
            trie.display();
        }
    }
    return 0;
}
