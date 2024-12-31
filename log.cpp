#include "log.h"
#include <iostream>

namespace log {
  void CreatFileIfNotExist(const std::string &fn) {
    bool exist = false;
    std::fstream file(fn);
    if (file.is_open()) {
      exist = true;
    }
    file.close();
    if (!exist) {
      std::ofstream new_file(fn);
      int size;
      new_file.seekp(0);
      new_file.write(reinterpret_cast<char *>(& size), sizeof(size));
      new_file.close();
    }
  }

  void insert(const std::string &who, const std::string &what, const std::string &how) {
    CreatFileIfNotExist(log_file);
    system_log n(who, what, how);
    std::fstream file(log_file);
    int size;
    file.seekg(0);
    file.read(reinterpret_cast<char *>(& size), sizeof(size));
    file.seekp(sizeof(size) + size * sizeof(n));
    file.write(reinterpret_cast<char *>(& n), sizeof(n));
    ++size;
    file.seekp(0);
    file.write(reinterpret_cast<char *>(& size), sizeof(size));
    file.close();
  }

  void print_all() {
    int total;
    CreatFileIfNotExist(log_file);
    std::fstream file(log_file);
    file.seekg(0);
    file.read(reinterpret_cast<char *>(& total), sizeof(total));
    if (total == 0) {
      std::cout << "\n";
      file.close();
      return;
    }
    system_log group[total];
    file.seekg(sizeof(int));
    file.read(reinterpret_cast<char *>(& group), sizeof(system_log) * total);

    for (int i = 0; i < total; ++i) {
      std::cout << "|" << group[i].who;
      for (int j = 0; j < 30 - group[i].who.ToString().length(); ++j) {
        std::cout << "_";
      }
      std::cout << "|";
      std::cout << group[i].what;
      for (int j = 0; j < 30 - group[i].what.ToString().length(); ++j) {
        std::cout << "_";
      }
      std::cout << "|";
      std::cout << group[i].how;
      for (int j = 0; j < 30 - group[i].how.ToString().length(); ++j) {
        std::cout << "_";
      }
      std::cout << "|\n";
    }
    file.close();
  }
}