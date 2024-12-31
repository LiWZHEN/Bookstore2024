#include "worker.h"
#include <iostream>
namespace worker {

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
  void insert(std::string name, std::string operation, std::string object) {
    CreatFileIfNotExist(worker_file);
    worker_work n(name, operation, object);
    std::fstream file(worker_file);
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
    CreatFileIfNotExist(worker_file);
    std::fstream file(worker_file);
    file.seekg(0);
    file.read(reinterpret_cast<char *>(& total), sizeof(total));
    if (total == 0) {
      std::cout << "\n";
      file.close();
      return;
    }
    worker_work group[total];
    file.seekg(sizeof(int));
    file.read(reinterpret_cast<char *>(& group), sizeof(worker_work) * total);

    for (int i = 0; i < total; ++i) {
      std::cout << "|" << group[i].name;
      for (int j = 0; j < 30 - group[i].name.ToString().length(); ++j) {
        std::cout << "_";
      }
      std::cout << "|";
      std::cout << group[i].operation;
      for (int j = 0; j < 30 - group[i].operation.ToString().length(); ++j) {
        std::cout << "_";
      }
      std::cout << "|";
      std::cout << group[i].object;
      for (int j = 0; j < 30 - group[i].object.ToString().length(); ++j) {
        std::cout << "_";
      }
      std::cout << "|\n";
    }
    file.close();
  }
}