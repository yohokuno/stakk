#ifndef TRIE_H
#define TRIE_H

struct Trie {
    struct Entry {
        string key;
        int distance;
        vector<string> values;
        Entry(string _key, int _distance, vector<string> _values) {
            key = _key;
            distance = _distance;
            values = _values;
        }
    };
    typedef vector<Entry> Entries;
    typedef pair<char, Trie> Pair;
    typedef list<Pair>::iterator Itr;
    list<Pair> children;
    vector<string> values;

    void insert(string key, string value) {
        if (key.length() != 0) {
            char first = key[0];
            string rest = key.substr(1);
            Trie *child = find(first);
            if (child == NULL) {
                children.push_back(Pair(first, Trie()));
                child = &(children.back().second);
            }
            child->insert(rest, value);
        } else {
            values.push_back(value);
        }
    }
    Trie *find(char key) {
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
        Trie *child = find(first);
        if (child != NULL)
            return child->search(rest);
        return NULL;
    }
    void common_prefix_search(string query, string key, Entries &results) {
        if (values.size())
            results.push_back(Entry(key, 0, values));
        if (!query.length() || !children.size())
            return;
        Trie *child = find(query.at(0));
        if (child != NULL)
            child->common_prefix_search(query.substr(1), key+query.at(0), results);
    }
    void predictive_search(string query, string key, Entries &results) {
        if (query.length() <= key.length() && values.size())
            results.push_back(Entry(key, 0, values));
        if (!children.size())
            return;
        if (query.length() > key.length()) {
            char c = query.at(key.length());
            Trie *child = find(c);
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
            results.push_back(Entry(key, distance, values));
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
                if (entry.key == results.at(j).key) {
                    if (entry.distance > results.at(j).distance)
                        results[j] = entry;
                    flag = true;
                    break;
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
    void load(string filename, int key, char separator) {
        ifstream ifs(filename.c_str());
        string line;
        while (getline(ifs, line)) {
            vector<string> splited = split(line, separator);
            insert(splited[key], line);
        }
        ifs.close();
    }
    static string format(Entries entries) {
        string result = "";
        for (int i = 0; i < entries.size(); i++) {
            Trie::Entry entry = entries.at(i);
            for (int j = 0; j < entry.values.size(); j++) {
                stringstream distance;
                distance << entry.distance;
                result += entry.key + "\t" + distance.str() + "\t" + entry.values.at(j) + "\n";
            }
        }
        return result;
    }
    static string format(vector<string> values) {
        string result = "";
        for (int i = 0; i < values.size(); i++)
            result += values.at(i) + "\n";
        return result;
    }
};

#endif

