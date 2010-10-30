#include "common.h"
#include "util.h"
#include "trie.h"
#include "converter.h"

int main(int argc, char *argv[]) {
    //parse option
    int result;
    string dictionary = "data/dictionary.txt";
    string connection = "data/connection.txt";
    locale::global(locale(""));

    while((result = getopt(argc, argv, "d:c:")) != -1) {
        switch(result) {
            case 'd':
                dictionary = optarg;
                break;
            case 'c':
                connection = optarg;
                break;
        }
    }

    ListTrieWide trie;
    wcout << "loading dictionary" << endl;
    trie.load_dictionary(dictionary);
    wcout << "loading connection" << endl;
    Converter converter(trie, connection);
    wcout << "input query: " << endl;
    wstring line;
    while (getline(wcin, line)) {
        vector<wstring> result = converter.convert(line);
        for (int i = 0; i < result.size(); i++) {
            wcout << result[i] << " ";
        }
        wcout << endl;
    }
    return 0;
}
