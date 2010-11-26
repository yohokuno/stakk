#ifndef UTIL_H
#define UTIL_H
#include "common.h"

namespace stakk {
    //wide to multi
    inline string narrow(wstring src) {
        char *mbs = new char[src.length() * MB_CUR_MAX + 1];
        wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
        string dest = mbs;
        delete [] mbs;
        return dest;
    }

    //multi to wide
    inline wstring widen(string src) {
        wchar_t *wcs = new wchar_t[src.length() + 1];
        mbstowcs(wcs, src.c_str(), src.length() + 1);
        wstring dest = wcs;
        delete [] wcs;
        return dest;
    }
    //urldecode
    inline string urldecode(string url) {
        string buffer = "";

        for (int i = 0; i < url.length(); i++) {
            int j = i ;
            char ch = url.at(j);
            if (ch == '%'){
                char tmpstr[] = "0x0__";
                int chnum;
                tmpstr[3] = url.at(j+1);
                tmpstr[4] = url.at(j+2);
                chnum = strtol(tmpstr, NULL, 16);
                buffer += chnum;
                i += 2;
            } else {
                buffer += ch;
            }
        }
        return buffer;
    }
    //split
    inline vector<string> split(string s, char c) {
        vector<string> v;
        for (int p = 0; (p = s.find(c)) != s.npos; ) {
            v.push_back(s.substr(0, p));
            s = s.substr(p + 1);
        }
        v.push_back(s);
        return v;
    }
    //split wide
    inline vector<wstring> split_w(wstring s, wchar_t c) {
        vector<wstring> v;
        for (int p = 0; (p = s.find(c)) != s.npos; ) {
            v.push_back(s.substr(0, p));
            s = s.substr(p + 1);
        }
        v.push_back(s);
        return v;
    }
    //join
    inline string join(vector<string> v, char c) {
        string result = "";
        for (int i = 0; i < v.size(); i++) {
            result += v[i] + c;
        }
        return result.substr(0, result.length()-1);
    }

    //join wide
    inline wstring join_w(vector<wstring> v, wchar_t c) {
        wstring result = L"";
        for (int i = 0; i < v.size(); i++) {
            result += v[i] + c;
        }
        return result.substr(0, result.length()-1);
    }
    //utf-8 character length
    inline size_t utf8_char(const char *src) {
        const unsigned char table[256] = {
            1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
            2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2,
            3,3,3,3,3,3,3,3, 3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4, 4,4,4,4,4,4,4,4
        };
        return table[*reinterpret_cast<const unsigned char*>(src)];
    }
    //utf-8 string length
    inline size_t utf8_len(const char *src, size_t length) {
        const char *begin = src;
        const char *end = src + length;
        int result = 0;
        while (begin < end) {
            ++result;
            begin += utf8_char(begin);
        }
        return result;
    }
    //utf-8 substring
    string utf8_substring(const string &src, const size_t start, const size_t length) {
        string result = "";
        size_t l = start;
        const char *begin = src.c_str();
        const char *end = begin + src.size();
        while (l > 0) {
            begin += utf8_char(begin);
            --l;
        }

        l = length;
        while (l > 0 && begin < end) {
            const size_t len = utf8_char(begin);
            result.append(begin, len);
            begin += len;
            --l;
        }
        return result;
    }
    //utf-8 split string to character
    vector<string> utf8_split(string input) {
        vector <string> result;
        const char *p = input.c_str();
        for (int i=0; i<input.length(); i+=utf8_char(p+i)) {
            result.push_back(input.substr(i, utf8_char(p+i)));
        }
        return result;
    }
}
#endif
