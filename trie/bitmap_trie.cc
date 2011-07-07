#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct BitmapTrie {
    typedef unsigned int uint;
    uint bitmap;
    vector<BitmapTrie> children;
    BitmapTrie() {
        bitmap = 0;
    }
    uint bitof(uint i) {
        return (bitmap >> i) & 1;
    }
    uint rank(uint pos) {
        uint result = 0;
        for (int i = 0; i < pos; i++) {
            if (bitof(i))
                result++;
        }
        return result;
    }
    uint select(uint num) {
        uint pos = 0;
        while (num) {
            if (bitof(pos))
                num--;
            pos++;
        }
        return pos-1;
    }

    void insert(string key) {
        if (key.length() != 0) {
            uint first = key[0] - 'a';
            string rest = key.substr(1);
            if (!bitof(first)) {
                children.push_back(BitmapTrie());
                bitmap |= (1<<first);
            }
            uint pos = rank(first);
            children.at(pos).insert(rest);
        }
    }
    void display(string key="") {
        cout << "bitmap: " << bitmap << endl;
        if (!bitmap)
            cout << "key: " << key << endl;
        for (int i = 0; i < children.size(); i++) {
            char c = 'a' + select(i+1);
            children.at(i).display(key + c);
        }
    }
};

int main(int argc, char *argv[]) {
    BitmapTrie trie;
    // please insert sorted order
    trie.insert("aaa");
    trie.insert("abc");
    trie.insert("bbc");
    trie.display();
    return 0;
}
