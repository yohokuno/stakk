#ifndef UTIL_H
#define UTIL_H

//wide to multi
string narrow(wstring src) {
    char *mbs = new char[src.length() * MB_CUR_MAX + 1];
    wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
    string dest = mbs;
    delete [] mbs;
    return dest;
}

//multi to wide
wstring widen(string src) {
    wchar_t *wcs = new wchar_t[src.length() + 1];
    mbstowcs(wcs, src.c_str(), src.length() + 1);
    wstring dest = wcs;
    delete [] wcs;
    return dest;
}
//urldecode
string urldecode(string url) {
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
vector<string> split(string s, char c) {
    vector<string> v;
    for (int p = 0; (p = s.find(c)) != s.npos; ) {
        v.push_back(s.substr(0, p));
        s = s.substr(p + 1);
    }
    v.push_back(s);
    return v;
}

#endif

