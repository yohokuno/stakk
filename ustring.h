#ifndef USTRING_H
#define USTRING_H

#include "common.h"

namespace stakk {

typedef basic_string<uint16_t> ustring;

inline size_t get_len(char c) {
  const uint8_t len_table[256] = {
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3, 3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4, 4,4,4,4,4,4,4,4
  };
  return (size_t)len_table[(uint8_t)c];
}
inline ustring decode(string input) {
  ustring result;
  for (size_t i = 0; i < input.length(); i+= get_len(input[i])) {
    switch (get_len(input[i])) {
      case 1:
        result += (uint16_t) input[i];
        break;
      case 3:
        result += (uint16_t) input[i+1] + ((uint16_t) input[i+2] << 8);
        break;
    }
  }
  return result;
}
inline string encode(ustring input) {
  string result;
  for (size_t i = 0; i < input.length(); i++) {
    uint8_t up = input[i] >> 8;
    uint8_t down = (uint8_t) input[i];
    if (up == 0) {
      result += (char) down;
    } else {
      result += (char) 0xe3;
      result += (char) down;
      result += (char) up;
    }
  }
  return result;
}
}
#endif
