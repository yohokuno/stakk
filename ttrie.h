#ifndef TRIE_H
#define TRIE_H

template<typename CHAR, typename STRING>
struct Trie {
    struct Entry {
        STRING key;
        int distance;
        vector<STRING> values;
        Entry(STRING _key, int _distance, vector<STRING> _values) {
            key = _key;
            distance = _distance;
            values = _values;
        }
    };
    typedef vector<Entry> Entries;
    typedef pair<CHAR, Trie> Pair;
    typedef typename list<Pair>::iterator Itr;
    list<Pair> children;
    vector<STRING> values;

    void insert(STRING key, STRING value) {
        if (key.length() != 0) {
            CHAR first = key[0];
            STRING rest = key.substr(1);
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
    Trie *find(CHAR key) {
        for (Itr i = children.begin(); i != children.end(); i++) {
            if (key == i->first) {
                return &i->second;
            }
        }
        return NULL;
    }
    vector<STRING> *search(STRING key) {
        if (!key.length())
            if (values.size())
                return &values;
            else
                return NULL;
        CHAR first = key[0];
        STRING rest = key.substr(1);
        Trie *child = find(first);
        if (child != NULL)
            return child->search(rest);
        return NULL;
    }
    void common_prefix_search(STRING query, STRING key, Entries &results) {
        if (values.size())
            results.push_back(Entry(key, 0, values));
        if (!query.length() || !children.size())
            return;
        Trie *child = find(query.at(0));
        if (child != NULL)
            child->common_prefix_search(query.substr(1), key+query.at(0), results);
    }
    void predictive_search(STRING query, STRING key, Entries &results) {
        if (query.length() <= key.length() && values.size())
            results.push_back(Entry(key, 0, values));
        if (!children.size())
            return;
        if (query.length() > key.length()) {
            CHAR c = query.at(key.length());
            Trie *child = find(c);
            if (child != NULL)
                child->predictive_search(query, key+c, results);
        } else {
            for (Itr i = children.begin(); i != children.end(); i++) {
                i->second.predictive_search(query, key+i->first, results);
            }
        }
    }
    void fuzzy_search(STRING query, STRING key, int distance, Entries &results) {
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
                CHAR c = query.at(0);
                query[0] = query.at(1);
                query[1] = c;
                fuzzy_search(query, key, distance-1, results);
            }
        }
    }
    void fuzzy_search_ex(STRING query, int distance, Entries &results) {
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
    void load(string filename, int key, CHAR separator) {
        ifstream ifs(filename.c_str());
        string line;
        while (getline(ifs, line)) {
            vector<string> splited = split(line, separator);
            insert(splited[key], line);
        }
        ifs.close();
    }
    static STRING format(Entries entries) {
        STRING result;
        for (int i = 0; i < entries.size(); i++) {
            Entry entry = entries.at(i);
            for (int j = 0; j < entry.values.size(); j++) {
                stringstream distance;
                distance << entry.distance;
                result += entry.key + "\t" + distance.str() + "\t" + entry.values.at(j) + "\n";
            }
        }
        return result;
    }
    static STRING format(vector<STRING> values) {
        STRING result = "";
        for (int i = 0; i < values.size(); i++)
            result += values.at(i) + "\n";
        return result;
    }
    static STRING separator;
    static STRING linebreak;
    static STRING empty;
};
typedef Trie<char, string> SimpleTrie;
typedef Trie<wchar_t, wstring> WideTrie;

template<>
void Trie<wchar_t, wstring>::load(string filename, int key, wchar_t separator) {
    wifstream ifs(filename.c_str());
    wstring line;
    while (getline(ifs, line)) {
        vector<wstring> splited = split_w(line, separator);
        insert(splited[key], line);
    }
    ifs.close();
}

#endif

