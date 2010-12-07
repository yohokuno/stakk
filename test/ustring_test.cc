#include "../ustring.h"
using namespace stakk;

int main() {
  string a = "あ";
  assert( get_len('a') == 1 );
  assert( get_len(a[0]) == 3 );

  string test = "あいうえお abcde";
  ustring temp = decode(test);
  string result = encode(temp);
  cout << result << endl;
  assert(test == result);
  return 0;
}
