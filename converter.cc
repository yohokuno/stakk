#include "converter.h"
using namespace stakk;

int main(int argc, char *argv[]) {
  //parse option
  int result;
  string dictionary_ = "data/dictionary.txt";
  string connection_ = "data/connection.txt";
  string id_def = "data/id.def";
  string output = "mecab";
  bool reverse = false;
  bool debug = false;
  try { locale::global(locale("")); } catch (...) {}

  while((result = getopt(argc, argv, "d:c:i:o:rb")) != -1) {
    switch(result) {
      case 'd':
        dictionary_ = optarg;
        break;
      case 'c':
        connection_ = optarg;
        break;
      case 'i':
        id_def = optarg;
        break;
      case 'o':
        output = optarg;
        break;
      case 'r':
        reverse = true;
        break;
      case 'b':
        debug = true;
        break;
    }
  }

  wcout << "loading dictionary" << endl;
  WideTrie trie;
  int field = reverse ? 4 : 0;
  if (!trie.load(dictionary_, field, L'\t')) {
    cout << dictionary_ << " is not found." << endl;
    exit(0);
  }

  wcout << "loading connection" << endl;
  Connection connection;
  if (!connection.load(connection_)) {
    cout << connection_ << " is not found." << endl;
    exit(0);
  }

  wcout << "loading id definition" << endl;
  Definition definition;
  if (!definition.load(id_def)) {
    cout << id_def << " is not found." << endl;
    exit(0);
  }

  Converter converter(trie, connection, definition);
  wstring line;

  wcout << "input query: " << endl;
  while (getline(wcin, line)) {
    vector<Converter::Node> nodes = converter.convert(line, debug);
    wcout << converter.format(nodes, output);
  }
  return 0;
}
