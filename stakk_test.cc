#include "common.h"
#include "util.h"
#include "trie.h"
#include "stakk.h"

int main(int argc, char *argv[]) {
    //parse option
    int result, threshold = 2;
    string dictionary = "data/dictionary.txt";
    string connection = "data/connection.txt";
    string mode = "spell";
    locale::global(locale(""));

    while((result = getopt(argc, argv, "d:c:t:m:")) != -1) {
        switch(result) {
            case 'd':
                dictionary = optarg;
                break;
            case 'c':
                connection = optarg;
                break;
            case 't':
                threshold = atoi(optarg);
                break;
            case 'm':
                mode = optarg;
                break;
        }
    }

    ListTrieWide trie;
    wcout << "loading dictionary" << endl;
    trie.load_dictionary(dictionary);
    wcout << "loading connection" << endl;
    unsigned short connection_[CONNECTION_SIZE];
    {
        ifstream ifs;
        ifs.open(connection.c_str());
        string line;
        getline(ifs, line);
        while (getline(ifs, line)) {
            vector<string> splited = split(line, ' ');
            int lid = atoi(splited[0].c_str());
            int rid = atoi(splited[1].c_str());
            int cost = atoi(splited[2].c_str());
            if (lid != 0) break;
            connection_[rid] = cost;
        }
        ifs.close();
    }
    wcout << "input query: " << endl;
    Stakk stakk(trie, connection_);
    wstring line;
    while (getline(wcin, line)) {
        vector<Stakk::Entry> result;
        if (mode == "spell")
            stakk.correct(line, threshold, result);
        if (mode == "predict")
            stakk.predict(line, result);
        for (int i = 0; i < min(50, (int)result.size()); i++) {
            wcout << result[i].format() << endl;
        }
    }
    return 0;
}
