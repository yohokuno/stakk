#ifndef CONVERTER_SERVER_H
#define CONVERTER_SERVER_H

struct ConverterServer : public TrieServer {
    Converter &converter;

    //init trie and converter references
    ConverterServer(ListTrieWide &trie_, Converter &converter_)
        : TrieServer(trie_), converter(converter_) {}

    //implement converter
    virtual wstring get_result(vector<string> path) {
        wstring response = L"";

        if (mode == "convert") {
            wstring input = parse_path(path);
            vector<wstring> result = converter.convert(input);
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

