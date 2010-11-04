#ifndef STAKK_SERVER_H
#define STAKK_SERVER_H

struct StakkServer : public TrieServer {
    Stakk &stakk;
    Converter &converter;
    int threshold, number;

    //init trie and stakk references
    StakkServer(ListTrieWide &trie_, Stakk &stakk_, Converter &converter_, int threshold_, int number_)
        : TrieServer(trie_), stakk(stakk_), converter(converter_)
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
        wstring input = parse_path(path);

        //converter
        if (mode == "convert") { 
            string output = "wakati";
            vector<Converter::Node> nodes = converter.convert(input);
            return converter.format(nodes, output);
        }
        //prediction or spell correction
        {
            vector<Stakk::Entry> result;
            if (mode == "spell")
                stakk.correct(input, threshold, result);
            else if (mode == "predict")
                stakk.predict(input, result);

            wstring response = L"";
            for (int i = 0; i < min(number, (int)result.size()); i++) {
                response += result[i].format() + L"\n";
            }
            if (result.size() > 0)
                return response;
        }
        //otherwise raw trie operation
        return TrieServer::get_result(path);
    }
};

#endif

