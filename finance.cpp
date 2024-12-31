#include "finance.h"

#include <iostream>

namespace finance {
  void CreatFileIfNotExist(const std::string &fn) {
    bool exist = false;
    std::fstream file(fn);
    if (file.is_open()) {
      exist = true;
    }
    file.close();
    if (!exist) {
      std::ofstream new_file(fn);
      unsigned size;
      new_file.seekp(0);
      new_file.write(reinterpret_cast<char *>(& size), sizeof(size));
      new_file.close();
    }
  }
  void insert(double add, double minus) {
    CreatFileIfNotExist(finance_file);
    add_minus n(add, minus);
    std::fstream file(finance_file);
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
  void print(int count) {
    if (count == 0) {
      std::cout << "\n";
      return;
    }
    int total;
    CreatFileIfNotExist(finance_file);
    std::fstream file(finance_file);
    file.seekg(0);
    file.read(reinterpret_cast<char *>(& total), sizeof(total));
    if (total < count) {
      std::cout << "Invalid\n";
      file.close();
      return;
    }
    add_minus group[count];
    file.seekg(sizeof(int) + (total - count) * sizeof(add_minus));
    file.read(reinterpret_cast<char *>(& group), sizeof(add_minus) * count);
    add_minus ans(0, 0);
    for (int i = 0; i < count; ++i) {
      ans += group[i];
    }
    std::cout << std::fixed << std::setprecision(2) << "+ " << ans.add << " - " << ans.minus << "\n";
    file.close();
  }
  void print_all() {
    int total;
    CreatFileIfNotExist(finance_file);
    std::fstream file(finance_file);
    file.seekg(0);
    file.read(reinterpret_cast<char *>(& total), sizeof(total));
    if (total == 0) {
      std::cout << "\n";
      file.close();
      return;
    }
    add_minus group[total];
    file.seekg(sizeof(int));
    file.read(reinterpret_cast<char *>(& group), sizeof(add_minus) * total);
    add_minus ans(0, 0);
    for (int i = 0; i < total; ++i) {
      ans += group[i];
    }
    std::cout << std::fixed << std::setprecision(2) << "+ " << ans.add << " - " << ans.minus << "\n";
    file.close();
  }
}
