#ifndef TRIE_SERVER_H
#define TRIE_SERVER

struct TrieServer : public Server {
    string filename, mode;
    ListTrieWide &trie;

    //init trie reference
    TrieServer(ListTrieWide &trie_) : trie(trie_) {}

    //parse request path
    wstring parse_path(vector<string> &path) {
        if (path.size() != 4)
            return L"";
        mode = path[2];
        return widen(urldecode(path[3]));
    }

    //get result
    virtual wstring get_result(vector<string> path) {
        wstring response = L"";
        wstring input = parse_path(path);
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
            trie.fuzzy_search_ex(input, 1, results);
            response += ListTrieWide::format(results);
        }
        return response;
    }
};

#endif

