#include "common.h"
#include "util.h"
#include "trie.h"
#include "spell.h"

int main(int argc, char *argv[]) {
    //parse option
    int result, threshold = 2;
    string dictionary = "data/mozc-dictionary.txt";
    string connection = "data/mozc-connection.txt";
    locale::global(locale(""));

    while((result = getopt(argc, argv, "d:c:t:")) != -1) {
        switch(result) {
            case 'd':
                dictionary = optarg;
                break;
            case 'c':
                connection = optarg;
                break;
            case 't':
                threshold = atoi(optarg);
        }
    }

    SpellCorrector corrector(dictionary, connection);
    wcout << "input query: " << endl;
    wstring line;
    while (getline(wcin, line)) {
        vector<SpellCorrector::Entry> result;
        corrector.correct(line, threshold, result);
        for (int i = 0; i < min(50, (int)result.size()); i++) {
            wcout << result[i].format() << endl;
        }
    }
    return 0;
}
