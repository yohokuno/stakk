#ifndef TRIE_SERVER_H
#define TRIE_SERVER_H

struct TrieServer : public Server {
    string filename;
    ListTrieWide trie;

    //parse option
    void parse_option(int argc, char *argv[]) {
        int result;
        while((result = getopt(argc, argv, "f:p:")) != -1) {
            switch(result) {
                case 'f':
                    filename = optarg;
                    break;
                case 'p':
                    port = atoi(optarg);
                    break;
            }
        }
    }
    //load trie
    void load_trie() {
        if (filename.length() > 0) {
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
        } else {
            trie.insert(L"tree", L"value01");
            trie.insert(L"trie", L"value02");
            trie.insert(L"try", L"value03");
            trie.insert(L"tree", L"value04");
            trie.insert(L"tr", L"value05");
            trie.insert(L"trees", L"value06");
            trie.insert(L"tere", L"value06");
            trie.display();
        }

    }

    //get result
    virtual wstring get_result(vector<string> path) {
        wstring response = L"";
        if (path.size() != 3)
            return L"";
        string mode = path[1];
        wstring input = widen(urldecode(path[2]));
        if (mode == "all" || mode == "search") {
            vector<wstring> *result;
            result = trie.search(input);
            if (result)
                response += ListTrieWide::format(*result);
        }
        if (mode == "all" || mode == "common") {
            ListTrieWide::Entries results;
            trie.common_prefix_search(input, L"", results);
            response +=  ListTrieWide::format(results);
        }
        if (mode == "all" || mode == "predict") {
            ListTrieWide::Entries results;
            trie.predictive_search(input, L"", results);
            response += ListTrieWide::format(results);
        }
        if (mode == "all" || mode == "fuzzy") {
            ListTrieWide::Entries results;
            trie.fuzzy_search_ex(input, 2, results);
            response += ListTrieWide::format(results);
        }
        return response;
    }
};

#endif

