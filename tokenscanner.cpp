#include "tokenscanner.h"

std::string TokenScanner::nextToken() {
  bool space_end = false;
  std::string next_token;
  for (; pos < line.length(); ++pos) {
    if (line[pos] == '\0') {
      return next_token;
    }
    if (line[pos] == ' ') {
      if (!space_end) {
        continue;
      }
      break;
    }
    if (line[pos] != ' ') {
      space_end = true;
      next_token += line[pos];
    }
  }
  return next_token;
}

bool TokenScanner::hasMoreTokens() const {
  int p = pos;
  for (; p < line.length(); ++p) {
    if (line[p] != ' ' && line[p] != '\0') {
      return true;
    }
  }
  return false;
}
