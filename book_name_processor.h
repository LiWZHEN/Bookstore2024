#ifndef HASH_PROCESSOR_H
#define HASH_PROCESSOR_H
#include <fstream>
#include <vector>

#include "book_data.h"

namespace book_name {
  const unsigned long P = 131;
  const unsigned long Q = 107;
  const unsigned long M = 1e9 + 7;
  const int MAX_BLOCK_SIZE = 400;
  const int MAX_MENU_SIZE = 1000;

  struct hash_pair {
    unsigned long long hash1 = 0;
    unsigned long long hash2 = 0;
    hash_pair &operator=(const hash_pair &other) {
      hash1 = other.hash1;
      hash2 = other.hash2;
      return *this;
    }
    bool operator==(const hash_pair &other) const {
      if (hash1 == other.hash1 && hash2 == other.hash2) {
        return true;
      }
      return false;
    }
    bool operator>(const hash_pair &other) const {
      if (hash1 != other.hash1) {
        return hash1 > other.hash1;
      }
      return hash2 > other.hash2;
    }
    bool operator<(const hash_pair &other) const {
      if (hash1 != other.hash1) {
        return hash1 < other.hash1;
      }
      return hash2 < other.hash2;
    }
    bool operator!=(const hash_pair &other) const {
      return !(*this == other);
    }
  };
  hash_pair db_hash(const std::string &str);
  struct index_value {
    hash_pair index;
    book::fixed_char_20 value;
    index_value &operator=(const index_value &other) {
      index = other.index;
      value = other.value;
      return *this;
    }
    bool operator==(const index_value &other) const {
      if (index != other.index || value != other.value) {
        return false;
      }
      return true;
    }
    bool operator<(const index_value &other) const {
      if (index != other.index) {
        return index < other.index;
      }
      return value < other.value;
    }
    bool operator>(const index_value &other) const {
      if (index != other.index) {
        return index > other.index;
      }
      return value > other.value;
    }
  };

  struct block {
    void insert(const index_value &p);
    void erase(const index_value &p);

    index_value pairs[MAX_BLOCK_SIZE + 10];
    int size = 0;
  };

  struct unit_menu {
    unsigned long long block_pos = 0;
    index_value smallest;
    bool operator==(const unit_menu &other) const {
      return smallest == other.smallest;
    }
    bool operator<(const unit_menu &other) const {
      return smallest < other.smallest;
    }
    bool operator>(const unit_menu &other) const {
      return smallest > other.smallest;
    }
  };
  struct Menu {
    unit_menu menu[MAX_MENU_SIZE + 10];
    int size = 0;

    [[nodiscard]] int FindLower(const index_value &p) const;
    [[nodiscard]] int FindUpper(const index_value &p) const;
    void insert(const unit_menu &u);
    void erase(const unit_menu &u);
  };

  void insert(const std::string &BookName, const std::string &ISBN);
  void erase(const std::string &BookName, const std::string &ISBN);
  std::vector<std::string> find(const std::string &BookName);
}
#endif //HASH_PROCESSOR_H
