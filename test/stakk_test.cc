#include "../stakk.h"
using namespace stakk;

int main(int argc, char *argv[]) {
  try { locale::global(locale("")); } catch (...) {}

  string dictionary_ = "test/dictionary.txt";
  string connection_ = "test/connection.txt";
  string id_def = "test/id.def";

  WideTrie trie;
  assert(trie.load(dictionary_, 0, L'\t') == true);

  Connection connection;
  assert(connection.load(connection_) == true);

  Definition definition;
  assert(definition.load(id_def) == true);

  Stakk stakk(trie, connection);
  {
    vector<Stakk::Entry> result;
    stakk.correct(L"わあし", 1, result);
    assert(result.size() > 0);
    assert(result[0].yomi == L"わたし");
    assert(result[0].word == L"私");
  }
  {
    vector<Stakk::Entry> result;
    stakk.predict(L"わ", result);
    assert(result.size() > 0);
    assert(result[0].yomi == L"わたし");
    assert(result[0].word == L"私");
  }
  return 0;
}
