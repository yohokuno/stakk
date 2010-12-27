#include "../utable.h"
using namespace stakk;

int main() {
  UTable table;
  {
    string test = "あいうえおabcde";
    ustring temp = table.decode(test);
    string result = table.encode(temp);

    assert( temp[0] == 1 );
    assert(test == result);
  }
  {
    string test = "あかさたなakb48";
    ustring temp = table.decode(test);
    string result = table.encode(temp);

    assert(test == result);
  }
  {
    UTable table;
    string test = "あいうえおabcde";
    for (int i = 0; i < 10; i++) {
      ustring temp = table.decode(test);
      string result = table.encode(temp);
      assert( temp[0] == 1 );
      assert(test == result);
    }
    assert(table.size() == 10);
  }

  return 0;
}
