#ifndef STAKK_SERVER_H
#define STAKK_SERVER_H

struct StakkServer : public TrieServer {
    Stakk &stakk;
    Analyzer &analyzer;
    int threshold, number;

    //init trie and stakk references
    StakkServer(ListTrieWide &trie_, Stakk &stakk_, Analyzer &analyzer_, int threshold_, int number_)
        : TrieServer(trie_), stakk(stakk_), analyzer(analyzer_)
    {
        threshold = threshold_;
        number = number_;
    }

    //parse request path
    wstring parse_path(vector<string> &path) {
        if (path.size() < 3)
            return L"";
        mode = path[1];
        if (path.size() > 3)
            threshold = atoi(path[3].c_str());
        if (path.size() > 4)
            number = atoi(path[4].c_str());
        return widen(urldecode(path[2]));
    }

    //implement spell and predict mode
    virtual wstring get_result(vector<string> path) {
        wstring response = L"";
        wstring input = parse_path(path);

        vector<Stakk::Entry> result;
        if (mode == "spell")
            stakk.correct(input, threshold, result);
        else if (mode == "predict")
            stakk.predict(input, result);

        for (int i = 0; i < min(number, (int)result.size()); i++) {
            response += result[i].format() + L"\n";
        }
        if (result.size() == 0)
            response = TrieServer::get_result(path);
        return response;
    }
};

#endif

