#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
using namespace std;

struct MapTrie {
    typedef map<char, MapTrie> Map;
    typedef Map::iterator Itr;
    typedef Map::value_type Pair;
    typedef pair<string, vector<string> > Entry;
    typedef vector<Entry> Entries;
    Map children;
    vector<string> values;

    void insert(string key, string value) {
        if (key.length() != 0) {
            char first = key[0];
            string rest = key.substr(1);
            Itr itr = children.find(first);
            if (itr == children.end())
                itr = children.insert(itr, Pair(first, MapTrie()));
            itr->second.insert(rest, value);
        } else {
            values.push_back(value);
        }
    }
    vector<string> *search(string key) {
        if (!key.length())
            if (values.size())
                return &values;
            else
                return NULL;
        char first = key[0];
        string rest = key.substr(1);
        Itr itr = children.find(first);
        if (itr != children.end())
            return itr->second.search(rest);
        return NULL;
    }
    void common_prefix_search(string query, string key, Entries &results) {
        if (values.size())
            results.push_back(Entry(key, values));
        if (!query.length() || !children.size())
            return;
        Itr itr = children.find(query.at(0));
        if (itr != children.end())
            itr->second.common_prefix_search(query.substr(1), key+query.at(0), results);
    }
    void predictive_search(string query, string key, Entries &results) {
        if (query.length() <= key.length() && values.size())
            results.push_back(Entry(key, values));
        if (!children.size())
            return;
        if (query.length() > key.length()) {
            char c = query.at(key.length());
            Itr itr = children.find(c);
            if (itr != children.end())
                itr->second.predictive_search(query, key+c, results);
        } else {
            for (Itr itr = children.begin(); itr != children.end(); itr++)
                itr->second.predictive_search(query, key+itr->first, results);
        }
    }
    void fuzzy_search(string query, string key, int distance, Entries &results) {
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
                char c = query.at(0);
                query[0] = query.at(1);
                query[1] = c;
                fuzzy_search(query, key, distance-1, results);
            }
        }
    }
    void fuzzy_search_ex(string query, int distance, Entries &results) {
        Entries entries;
        fuzzy_search(query, "", distance, entries);
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
    void display(string key="") {
        if (values.size() > 0)
            cout << key << "\t" << format(values) << endl;
        for (Itr itr = children.begin(); itr != children.end(); itr++)
            itr->second.display(key + itr->first);
    }
    static string format(Entries entries) {
        string result = "";
        for (int i = 0; i < entries.size(); i++) {
            MapTrie::Entry entry = entries.at(i);
            result += entry.first + "\t";
            for (int j = 0; j < entry.second.size(); j++)
                result += entry.second.at(j) + " ";
            result += "\n";
        }
        return result;
    }
    static string format(vector<string> entries) {
        string result = "";
        for (int i = 0; i < entries.size(); i++)
            result += entries.at(i) + " ";
        return result;
    }
};

int main(int argc, char *argv[]) {
    int result;
    string filename, mode = "all";
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
        MapTrie trie;
        ifstream ifs(filename.c_str());
        string line, input;
        while (getline(ifs, line)) {
            size_t pos = line.find('\t');
            if (pos == string::npos)
                continue;
            string key = line.substr(0, pos);
            string value = line.substr(pos+1);
            trie.insert(key, value);
        }
        while (cin >> input) {
            if (mode == "all" || mode == "search") {
                vector<string> *result;
                result = trie.search(input);
                cout << MapTrie::format(*result) << endl;
            }
            if (mode == "all" || mode == "common") {
                MapTrie::Entries results;
                trie.common_prefix_search(input, "", results);
                cout << MapTrie::format(results);
            }
            if (mode == "all" || mode == "predict") {
                MapTrie::Entries results;
                trie.predictive_search(input, "", results);
                cout << MapTrie::format(results);
            }
            if (mode == "all" || mode == "fuzzy") {
                MapTrie::Entries results;
                trie.fuzzy_search_ex(input, 3, results);
                cout << MapTrie::format(results);
            }
        }
    } else { 
        MapTrie trie;
        trie.insert("tree", "value01");
        trie.insert("trie", "value02");
        trie.insert("try", "value03");
        trie.insert("tree", "value04");
        trie.insert("tr", "value05");
        trie.insert("trees", "value06");
        trie.insert("tere", "value06");
        trie.display();

        { // search
            vector<string> *result;
            result = trie.search("tree");
            cout << "search:\n" << MapTrie::format(*result) << endl;
        }
        { // common prefix search
            MapTrie::Entries results;
            trie.common_prefix_search("tree", "", results);
            cout << "common:\n" << MapTrie::format(results);
        }
        { // predictive search
            MapTrie::Entries results;
            trie.predictive_search("tre", "", results);
            cout << "predict:\n" << MapTrie::format(results);
        }
        { // fuzzy search
            MapTrie::Entries results;
            trie.fuzzy_search("tree", "", 1, results);
            cout << "fuzzy:\n" << MapTrie::format(results);
        }
        { // fuzzy search 2
            MapTrie::Entries results;
            trie.fuzzy_search_ex("tree", 2, results);
            cout << "fuzzy2:\n" << MapTrie::format(results);
        }
        {  // japanese test
            MapTrie trie;
            trie.insert("にほんご", "日本語");
            trie.insert("にほんじん", "日本人");
            trie.display();
        }
    }
    return 0;
}
