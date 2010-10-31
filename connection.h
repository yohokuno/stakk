#ifndef CONNECTION_H
#define CONNECTION_H

struct Connection {
    unsigned short *connection;
    static const int size = 3033;

    Connection(string filename, bool full = true) {
        ifstream ifs;
        ifs.open(filename.c_str());
        string line;
        getline(ifs, line);
        if (full)
            connection = new unsigned short[size * size];
        else
            connection = new unsigned short[size];
        while (getline(ifs, line)) {
            vector<string> splited = split(line, ' ');
            unsigned short lid = atoi(splited[0].c_str());
            unsigned short rid = atoi(splited[1].c_str());
            unsigned short cost = atoi(splited[2].c_str());
            if (!full && lid != 0) break;
            connection[lid * size + rid] = cost;
        }
        ifs.close();
    }
    ~Connection() {
        delete[] connection;
    }
    unsigned short get(unsigned short lid, unsigned short rid) {
        return connection[lid * size + rid];
    }
};

struct Definition {
    static const int size = 3033;
    wstring definition[size];

    Definition(string filename) {
        wifstream ifs;
        ifs.open(filename.c_str());
        wstring line;
        while (getline(ifs, line)) {
            int index;
            wstring detail;
            wstringstream(line) >> index >> detail;
            definition[index] = detail;
        }
        ifs.close();
    }
    wstring get(int index) {
        return definition[index];
    }
};
#endif
