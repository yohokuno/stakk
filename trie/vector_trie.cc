#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Trie {
    typedef pair<string, vector<string> > Entry;
    typedef vector<Entry> Entries;
    typedef pair<char, Trie> Pair;
    vector<Pair> children;
    vector<string> values;

    void insert(string key, string value) {
        if (key.length() != 0) {
            char first = key[0];
            string rest = key.substr(1);
            Trie *child = find(first);
            if (child == NULL) {
                children.push_back(Pair(first, Trie()));
                child = &(children.at(children.size()-1).second);
            }
            child->insert(rest, value);
        } else {
            values.push_back(value);
        }
    }
    Trie *find(char key) {
        for (int i = 0; i < children.size(); i++) {
            if (key == children.at(i).first) {
                return &children.at(i).second;
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
        Trie *child = find(first);
        if (child != NULL)
            return child->search(rest);
        return NULL;
    }
    void common_prefix_search(string query, string key, Entries &results) {
        if (values.size())
            results.push_back(Entry(key, values));
        if (!query.length() || !children.size())
            return;
        Trie *child = find(query.at(0));
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
            Trie *child = find(c);
            if (child != NULL)
                child->predictive_search(query, key+c, results);
        } else {
            for (int i = 0; i < children.size(); i++) {
                Pair *pair = &children.at(i);
                pair->second.predictive_search(query, key+pair->first, results);
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
            Trie *child = find(query.at(0));
            if (child != NULL)
                child->fuzzy_search(query.substr(1), key+query.at(0), distance, results);
        }
        // edit operation
        if (distance) {
            // insert
            for (int i = 0; i < children.size(); i++) {
                Pair *pair = &children.at(i);
                pair->second.fuzzy_search(query, key+pair->first, distance-1, results);
            }
            if (query.length()) {
                // delete
                fuzzy_search(query.substr(1), key, distance-1, results);

                // substitute
                for (int i = 0; i < children.size(); i++) {
                    Pair *pair = &children.at(i);
                    if (pair->first != query.at(0))
                        pair->second.fuzzy_search(query.substr(1), key+pair->first, distance-1, results);
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
        for (int i = 0; i < children.size(); i++) {
            children.at(i).second.display(key + children.at(i).first);
        }
    }
    static string format(Entries entries) {
        string result = "";
        for (int i = 0; i < entries.size(); i++) {
            Trie::Entry entry = entries.at(i);
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
        Trie trie;
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
                    cout << Trie::format(*result) << endl;
            }
            if (mode == "all" || mode == "common") {
                Trie::Entries results;
                trie.common_prefix_search(input, "", results);
                cout << "common:" << endl;
                cout << Trie::format(results);
            }
            if (mode == "all" || mode == "predict") {
                Trie::Entries results;
                trie.predictive_search(input, "", results);
                cout << "predict:" << endl;
                cout << Trie::format(results);
            }
            if (mode == "all" || mode == "fuzzy") {
                Trie::Entries results;
                trie.fuzzy_search_ex(input, 3, results);
                cout << "fuzzy:" << endl;
                cout << Trie::format(results);
            }
        }
    } else { 
        Trie trie;
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
                cout << Trie::format(*result) << endl;
        }
        { // search2
            vector<string> *result;
            result = trie.search("tree");
            cout << "search2:" << endl;
            if (result)
                cout << Trie::format(*result) << endl;
        }
        { // common prefix search
            Trie::Entries results;
            trie.common_prefix_search("tree", "", results);
            cout << "common:" << endl;
            cout << Trie::format(results);
        }
        { // predictive search
            Trie::Entries results;
            trie.predictive_search("tre", "", results);
            cout << "predict:" << endl;
            cout << Trie::format(results);
        }
        { // fuzzy search
            Trie::Entries results;
            trie.fuzzy_search("tree", "", 1, results);
            cout << "fuzzy:" << endl;
            cout << Trie::format(results);
        }
        { // fuzzy search 2
            Trie::Entries results;
            trie.fuzzy_search_ex("tree", 2, results);
            cout << "fuzzy2:" << endl;
            cout << Trie::format(results);
        }
        {  // japanese test
            Trie trie;
            trie.insert("にほんご", "日本語");
            trie.insert("にほんじん", "日本人");
            trie.display();
        }
    }
    return 0;
}
