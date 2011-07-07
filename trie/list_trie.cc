#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

struct ListTrie {
    typedef pair<string, vector<string> > Entry;
    typedef vector<Entry> Entries;
    typedef pair<char, ListTrie> Pair;
    typedef list<Pair>::iterator Itr;
    list<Pair> children;
    vector<string> values;

    void insert(string key, string value) {
        if (key.length() != 0) {
            char first = key[0];
            string rest = key.substr(1);
            ListTrie *child = find(first);
            if (child == NULL) {
                children.push_back(Pair(first, ListTrie()));
                child = &(children.back().second);
            }
            child->insert(rest, value);
        } else {
            values.push_back(value);
        }
    }
    ListTrie *find(char key) {
        for (Itr i = children.begin(); i != children.end(); i++) {
            if (key == i->first) {
                return &i->second;
            }
        }
        return NULL;
    }
    vector<string> *search(string key) {
        if (!key.length())
            if (values.size())
                return &values;
            else
                return NULL;
        char first = key[0];
        string rest = key.substr(1);
        ListTrie *child = find(first);
        if (child != NULL)
            return child->search(rest);
        return NULL;
    }
    void common_prefix_search(string query, string key, Entries &results) {
        if (values.size())
            results.push_back(Entry(key, values));
        if (!query.length() || !children.size())
            return;
        ListTrie *child = find(query.at(0));
        if (child != NULL)
            child->common_prefix_search(query.substr(1), key+query.at(0), results);
    }
    void predictive_search(string query, string key, Entries &results) {
        if (query.length() <= key.length() && values.size())
            results.push_back(Entry(key, values));
        if (!children.size())
            return;
        if (query.length() > key.length()) {
            char c = query.at(key.length());
            ListTrie *child = find(c);
            if (child != NULL)
                child->predictive_search(query, key+c, results);
        } else {
            for (Itr i = children.begin(); i != children.end(); i++) {
                i->second.predictive_search(query, key+i->first, results);
            }
        }
    }
    void fuzzy_search(string query, string key, int distance, Entries &results) {
        if (!query.length() && values.size())
            results.push_back(Entry(key, values));
        if (!children.size())
            return;

        // normal match
        if (query.length()) {
            ListTrie *child = find(query.at(0));
            if (child != NULL)
                child->fuzzy_search(query.substr(1), key+query.at(0), distance, results);
        }
        // edit operation
        if (distance) {
            // insert
            for (Itr i = children.begin(); i != children.end(); i++) {
                i->second.fuzzy_search(query, key+i->first, distance-1, results);
            }
            if (query.length()) {
                // delete
                fuzzy_search(query.substr(1), key, distance-1, results);

                // substitute
                for (Itr i = children.begin(); i != children.end(); i++) {
                    if (i->first != query.at(0))
                        i->second.fuzzy_search(query.substr(1), key+i->first, distance-1, results);
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
        for (Itr i = children.begin(); i != children.end(); i++) {
            i->second.display(key + i->first);
        }
    }
    static string format(Entries entries) {
        string result = "";
        for (int i = 0; i < entries.size(); i++) {
            ListTrie::Entry entry = entries.at(i);
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
        ListTrie trie;
        ifstream ifs(filename.c_str());
        string line, input;
        cout << "inserting.." << endl;
        while (getline(ifs, line)) {
            size_t pos = line.find('\t');
            if (pos == string::npos)
                continue;
            string key = line.substr(0, pos);
            string value = line.substr(pos+1);
            trie.insert(key, value);
        }
        cout << "input kana:" << endl;
        while (cin >> input) {
            if (mode == "all" || mode == "search") {
                vector<string> *result;
                result = trie.search(input);
                cout << "search:" << endl;
                if (result)
                    cout << ListTrie::format(*result) << endl;
            }
            if (mode == "all" || mode == "common") {
                ListTrie::Entries results;
                trie.common_prefix_search(input, "", results);
                cout << "common:" << endl;
                cout << ListTrie::format(results);
            }
            if (mode == "all" || mode == "predict") {
                ListTrie::Entries results;
                trie.predictive_search(input, "", results);
                cout << "predict:" << endl;
                cout << ListTrie::format(results);
            }
            if (mode == "all" || mode == "fuzzy") {
                ListTrie::Entries results;
                trie.fuzzy_search_ex(input, 3, results);
                cout << "fuzzy:" << endl;
                cout << ListTrie::format(results);
            }
        }
    } else { 
        ListTrie trie;
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
            cout << "search:" << endl;
            if (result)
                cout << ListTrie::format(*result) << endl;
        }
        { // search2
            vector<string> *result;
            result = trie.search("tree");
            cout << "search2:" << endl;
            if (result)
                cout << ListTrie::format(*result) << endl;
        }
        { // common prefix search
            ListTrie::Entries results;
            trie.common_prefix_search("tree", "", results);
            cout << "common:" << endl;
            cout << ListTrie::format(results);
        }
        { // predictive search
            ListTrie::Entries results;
            trie.predictive_search("tre", "", results);
            cout << "predict:" << endl;
            cout << ListTrie::format(results);
        }
        { // fuzzy search
            ListTrie::Entries results;
            trie.fuzzy_search("tree", "", 1, results);
            cout << "fuzzy:" << endl;
            cout << ListTrie::format(results);
        }
        { // fuzzy search 2
            ListTrie::Entries results;
            trie.fuzzy_search_ex("tree", 2, results);
            cout << "fuzzy2:" << endl;
            cout << ListTrie::format(results);
        }
        {  // japanese test
            ListTrie trie;
            trie.insert("にほんご", "日本語");
            trie.insert("にほんじん", "日本人");
            trie.display();
        }
    }
    return 0;
}
