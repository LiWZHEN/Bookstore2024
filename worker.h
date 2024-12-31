#ifndef WORKER_H
#define WORKER_H
#include "user_data.h"

namespace worker {
  const std::string worker_file = "worker.txt";
  void CreatFileIfNotExist(const std::string &);

  struct worker_work {
    user::fixed_char name;
    user::fixed_char operation;
    user::fixed_char object;
    worker_work() = default;
    worker_work(user::fixed_char name, user::fixed_char operation, user::fixed_char object)
        : name(name), operation(operation), object(object){}
    worker_work &operator=(const worker_work &other) {
      name = other.name;
      operation = other.operation;
      object = other.object;
      return *this;
    }
  };

  void insert(std::string name, std::string operation, std::string object);
  void print_all();
}
#endif //WORKER_H
