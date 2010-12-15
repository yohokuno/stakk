#include "../ustring.h"
using namespace stakk;

int main() {
  Ustring u;

  string a = "あ";
  assert( get_len('a') == 1 );
  assert( get_len(a[0]) == 3 );

  string test = "あいうえお abcde";
  ustring temp = u.decode(test);

  assert( temp[0] == 0 );

  string result = u.encode(temp);

  assert(test == result);

  return 0;
}
