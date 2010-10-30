#ifndef MOZC_SERVER_H
#define MOZC_SERVER_H

struct MozcServer : public TrieServer {
    Mozc &mozc;

    //init trie and mozc references
    MozcServer(ListTrieWide &trie_, Mozc &mozc_)
        : TrieServer(trie_), mozc(mozc_) {}

    //implement mozc
    virtual wstring get_result(vector<string> path) {
        wstring response = L"";

        if (mode == "convert") {
            wstring input = parse_path(path);
            vector<wstring> result = mozc.convert(input);
            for (int i = 0; i < result.size(); i++) {
                response += result[i] + L"\n";
            }
        } else {
            response = TrieServer::get_result(path);
        }
        return response;
    }
};

#endif

