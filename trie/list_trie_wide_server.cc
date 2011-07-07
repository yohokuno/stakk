#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstdlib>
#include "list_trie_wide.h"

//global
string filename, mode = "all";
ListTrieWide trie;

//wide to multi
void narrow(const std::wstring &src, std::string &dest) {
    char *mbs = new char[src.length() * MB_CUR_MAX + 1];
    wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
    dest = mbs;
    delete [] mbs;
}

//multi to wide
void widen(const std::string &src, std::wstring &dest) {
    wchar_t *wcs = new wchar_t[src.length() + 1];
    mbstowcs(wcs, src.c_str(), src.length() + 1);
    dest = wcs;
    delete [] wcs;
}

//parse option
void parse_option(int argc, char *argv[]) {
    int result;
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
}
//load trie
void load_trie() {
    if (filename.length() > 0) {
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
    } else {
        trie.insert(L"tree", L"value01");
        trie.insert(L"trie", L"value02");
        trie.insert(L"try", L"value03");
        trie.insert(L"tree", L"value04");
        trie.insert(L"tr", L"value05");
        trie.insert(L"trees", L"value06");
        trie.insert(L"tere", L"value06");
        trie.display();
    }

}
//socket communication
int communicate() {
    int sock0;
    struct sockaddr_in addr;
    int yes = 1;

    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    if (sock0 < 0) {
        perror("socket");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;

    setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

    if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("bind");
        return 1;
    }

    if (listen(sock0, 5) != 0) {
        perror("listen");
        return 1;
    }

    //main loop
    while (1) {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int sock = accept(sock0, (struct sockaddr *)&client, &len);
        if (sock < 0) {
            perror("accept");
            break;
        }

        char inbuf[2048];
        memset(inbuf, 0, sizeof(inbuf));
        recv(sock, inbuf, sizeof(inbuf), 0);
        wstring input;
        string multi = inbuf;
        widen(multi, input);
        wcout << L"input: " << input << endl;

        //query trie and generate response
        wstring response = L"";
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
            trie.fuzzy_search_ex(input, 2, results);
            response += ListTrieWide::format(results);
        }
        wcout << L"output: " << response << endl;

        string output;
        narrow(response, output);
        send(sock, output.c_str(), (int)strlen(output.c_str()), 0);

        close(sock);
    }
    close(sock0);
    return 0;
}
//main logic
int main(int argc, char *argv[]) {
    // set locale
    setlocale(LC_CTYPE, "");
    locale::global(locale(""));

    parse_option(argc,argv);

    load_trie();

    int result = communicate();

    return result;
}
