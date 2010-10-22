#include "trie.h"

int main(int argc, char *argv[]) {
    int result;
    string filename, mode = "all";
    locale::global(locale(""));

    while((result = getopt(argc, argv, "f:m:")) != -1) {
        switch(result) {
            case 'f':
                filename = optarg;
                break;
            case 'm':
                mode = optarg;
                break;
        }
    }
    if (filename.length() > 0) {
        ListTrieWide trie;
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
        while (wcin >> input) {
            if (mode == "all" || mode == "search") {
                vector<wstring> *result;
                result = trie.search(input);
                wcout << "search:" << endl;
                if (result)
                    wcout << ListTrieWide::format(*result) << endl;
            }
            if (mode == "all" || mode == "common") {
                ListTrieWide::Entries results;
                trie.common_prefix_search(input, L"", results);
                wcout << "common:" << endl;
                wcout << ListTrieWide::format(results);
            }
            if (mode == "all" || mode == "predict") {
                ListTrieWide::Entries results;
                trie.predictive_search(input, L"", results);
                wcout << "predict:" << endl;
                wcout << ListTrieWide::format(results);
            }
            if (mode == "all" || mode == "fuzzy") {
                ListTrieWide::Entries results;
                trie.fuzzy_search_ex(input, 2, results);
                wcout << "fuzzy:" << endl;
                wcout << ListTrieWide::format(results);
            }
        }
    } else { 
        ListTrieWide trie;
        trie.insert(L"tree", L"value01");
        trie.insert(L"trie", L"value02");
        trie.insert(L"try", L"value03");
        trie.insert(L"tree", L"value04");
        trie.insert(L"tr", L"value05");
        trie.insert(L"trees", L"value06");
        trie.insert(L"tere", L"value06");
        trie.display();

        { // search
            vector<wstring> *result;
            result = trie.search(L"tree");
            wcout << "search:" << endl;
            if (result)
                wcout << ListTrieWide::format(*result) << endl;
        }
        { // search2
            vector<wstring> *result;
            result = trie.search(L"tree");
            wcout << "search2:" << endl;
            if (result)
                wcout << ListTrieWide::format(*result) << endl;
        }
        { // common prefix search
            ListTrieWide::Entries results;
            trie.common_prefix_search(L"tree", L"", results);
            wcout << "common:" << endl;
            wcout << ListTrieWide::format(results);
        }
        { // predictive search
            ListTrieWide::Entries results;
            trie.predictive_search(L"tre", L"", results);
            wcout << "predict:" << endl;
            wcout << ListTrieWide::format(results);
        }
        { // fuzzy search
            ListTrieWide::Entries results;
            trie.fuzzy_search(L"tree", L"", 1, results);
            wcout << "fuzzy:" << endl;
            wcout << ListTrieWide::format(results);
        }
        { // fuzzy search 2
            ListTrieWide::Entries results;
            trie.fuzzy_search_ex(L"tree", 2, results);
            wcout << "fuzzy2:" << endl;
            wcout << ListTrieWide::format(results);
        }
        {  // japanese test
            ListTrieWide trie;
            trie.insert(L"にほんご", L"日本語");
            trie.insert(L"にほんじん", L"日本人");
            trie.display();
        }
    }
    return 0;
}
