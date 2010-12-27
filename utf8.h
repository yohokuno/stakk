#ifndef USTRING_H
#define USTRING_H

#include "common.h"

namespace stakk {

inline size_t utf8_len(char c) {
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
inline size_t utf8_len2(char input) {
  size_t len = 0;
  uint8_t c = (uint8_t) input;
  if(c == 0x00) len = 0;
  else if(c < 0x80) len = 1;
  else if(c < 0xC2) len = 0;
  else if(c < 0xE0) len = 2;
  else if(c < 0xF0) len = 3;
  else if(c < 0xF8) len = 4;
  else if(c < 0xFC) len = 5;
  else if(c < 0xFE) len = 6;
  else len = 1;
  return len;
}
//utf-8 character length
inline size_t utf8_char(const char *src) {
  const unsigned char table[256] = {
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3, 3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4, 4,4,4,4,4,4,4,4
  };
  return table[*reinterpret_cast<const unsigned char*>(src)];
}
//utf-8 string length
inline size_t utf8_len(const char *src, size_t length) {
  const char *begin = src;
  const char *end = src + length;
  size_t result = 0;
  while (begin < end) {
    ++result;
    begin += utf8_char(begin);
  }
  return result;
}
//utf-8 substring
string utf8_substring(const string &src, const size_t start, const size_t length) {
  string result = "";
  size_t l = start;
  const char *begin = src.c_str();
  const char *end = begin + src.size();
  while (l > 0) {
    begin += utf8_char(begin);
    --l;
  }

  l = length;
  while (l > 0 && begin < end) {
    const size_t len = utf8_char(begin);
    result.append(begin, len);
    begin += len;
    --l;
  }
  return result;
}
//utf-8 split string to character
vector<string> utf8_split(string input) {
  vector <string> result;
  const char *p = input.c_str();
  for (size_t i=0; i<input.length(); i+=utf8_char(p+i)) {
    result.push_back(input.substr(i, utf8_char(p+i)));
  }
  return result;
}
}
#endif
