#include "key_processor.h"
namespace key {
  std::vector<std::string> split(const std::string &keys) {
    std::vector<std::string> ans;
    std::string single;
    int len = keys.length();
    /* todo if (len == 0) {
      ans.push_back("");
      return ans;
    }*/
    for (int i = 0; i < len; ++i) {
      if (keys[i] == '|') {
        ans.push_back(single);
        single.clear();
      } else {
        single += keys[i];
      }
    }
    if (!single.empty()) {
      ans.push_back(single);
    }
    return ans;
  }
  void insert(const std::string &keys, const std::string &ISBN) {
    std::vector<std::string> split_keys = split(keys);
    for (int i = 0; i < split_keys.size(); ++i) {
      keyword::insert(split_keys[i], ISBN);
    }
  }
  void erase(const std::string &keys, const std::string &ISBN) {
    std::vector<std::string> split_keys = split(keys);
    for (int i = 0; i < split_keys.size(); ++i) {
      keyword::erase(split_keys[i], ISBN);
    }
  }
  bool Repeated(const std::string &keys) {
    std::vector<std::string> split_keys = split(keys);
    for (int i = 0; i < split_keys.size() - 1; ++i) {
      for (int j = i + 1; j < split_keys.size(); ++j) {
        if (split_keys[i] == split_keys[j]) {
          return true;
        }
      }
    }
    return false;
  }
}