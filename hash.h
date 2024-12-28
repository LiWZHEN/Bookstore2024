//
// Created by anticlockwise on 2024/12/25.
//

#ifndef HASH_H
#define HASH_H
#include <string>

const unsigned long P = 131;
const unsigned long Q = 107;
const unsigned long M = 1e9 + 7;

struct db_hash {
  unsigned long long hash1 = 0;
  unsigned long long hash2 = 0;
  bool operator==(const db_hash &other) const {
    return hash1 == other.hash1 && hash2 == other.hash2;
  }
  bool operator>(const db_hash &other) const {
    if (hash1 != other.hash1) {
      return hash1 > other.hash1;
    }
    return hash2 > other.hash2;
  }
  bool operator<(const db_hash &other) const {
    if (hash1 != other.hash1) {
      return hash1 < other.hash1;
    }
    return hash2 < other.hash2;
  }
  bool operator!=(const db_hash &other) const {
    return !(*this == other);
  }
  bool operator>=(const db_hash &other) const {
    return !(*this < other);
  }
  bool operator<=(const db_hash &other) const {
    return !(*this > other);
  }
};
db_hash compute_hash(std::string);
#endif //HASH_H
