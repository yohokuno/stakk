#include "../ustring.h"
using namespace stakk;

int main() {
  Ustring u;

  string a = "あ";
  assert( get_len('a') == 1 );
  assert( get_len(a[0]) == 3 );

  {
    string test = "あいうえおabcde";
    ustring temp = u.decode(test);
    string result = u.encode(temp);

    assert( temp[0] == 1 );
    assert(test == result);
  }
  {
    string test = "あかさたなakb48";
    ustring temp = u.decode(test);
    string result = u.encode(temp);

    assert(test == result);
  }
  {
    Ustring u;
    string test = "あいうえおabcde";
    for (int i = 0; i < 10; i++) {
      ustring temp = u.decode(test);
      string result = u.encode(temp);
      assert( temp[0] == 1 );
      assert(test == result);
    }
    assert(u.size() == 10);
  }

  return 0;
}
