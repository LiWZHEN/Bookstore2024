//
// Created by anticlockwise on 2024/12/25.
//

#include "hash.h"
db_hash compute_hash(const std::string &str) {
  unsigned long hash1 = 0;
  unsigned long hash2 = 0;
  for (int i = 0; i < str.length(); ++i) {
    hash1 = (hash1 * P + str[i]) % M;
    hash2 = (hash2 * Q + str[i]) % M;
  }
  return {hash1, hash2};
}
