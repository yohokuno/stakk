#ifndef STAKK_SERVER_H
#define STAKK_SERVER_H

#include "common.h"
#include "util.h"
#include "trie.h"
#include "connection.h"
#include "stakk.h"
#include "converter.h"
#include "server.h"
#include "trie_server.h"

namespace stakk {
    class StakkServer : public TrieServer {
        protected:
            Stakk &stakk;
            Converter &converter;
            int number;
            string output;

        public:
            //init trie and stakk references
            StakkServer(int port_, ListTrieWide &trie_, Stakk &stakk_, Converter &converter_)
                : TrieServer(port_, trie_), stakk(stakk_), converter(converter_)
            {
            }

            //parse request path
            wstring parse_path(vector<string> &path) {
                if (path.size() < 4)
                    return L"";
                mode = path[2];
                if (mode == "convert") {
                    if (path.size() > 4)
                        output = path[4];
                    else
                        output = "wakati";
                } else if (mode == "predict") {
                    if (path.size() > 4)
                        number = atoi(path[4].c_str());
                    else
                        number = 50;
                } else if (mode == "spell") {
                    if (path.size() > 4)
                        threshold = atoi(path[4].c_str());
                    else
                        threshold = 1;
                    if (path.size() > 5)
                        number = atoi(path[5].c_str());
                    else
                        number = 50;
                }
                return widen(urldecode(path[3]));
            }

            //implement spell and predict mode
            virtual wstring get_result(vector<string> path) {
                wstring input = parse_path(path);

                //converter
                if (mode == "convert") { 
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
}
#endif
