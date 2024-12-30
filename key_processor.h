#ifndef KEY_PROCESSOR_H
#define KEY_PROCESSOR_H

#include <vector>
#include "key_file.h"

namespace key {
  std::vector<std::string> split(const std::string &keys);
  void insert(const std::string &keys, const std::string &ISBN);
  void erase(const std::string &keys, const std::string &ISBN);
  bool Repeated(const std::string &keys);
}
#endif //KEY_PROCESSOR_H
