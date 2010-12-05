#include "stakk.h"
using namespace stakk;

int main(int argc, char *argv[]) {
  //parse option
  int result, threshold = 1;
  string dictionary_ = "data/dictionary.txt";
  string connection_ = "data/connection.txt";
  string mode = "spell";
  bool reverse = false;
  try { locale::global(locale("")); } catch (...) {}

  while((result = getopt(argc, argv, "d:c:t:m:r")) != -1) {
    switch(result) {
      case 'd':
        dictionary_ = optarg;
        break;
      case 'c':
        connection_ = optarg;
        break;
      case 't':
        threshold = atoi(optarg);
        break;
      case 'm':
        mode = optarg;
        break;
      case 'r':
        reverse = true;
        break;
    }
  }

  wcout << "loading dictionary" << endl;
  ListTrieWide trie;
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

  Stakk stakk(trie, connection);
  wstring line;

  wcout << "input query: " << endl;
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
