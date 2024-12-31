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
    worker_work(const user::fixed_char &name, const user::fixed_char &operation, const user::fixed_char &object)
        : name(name), operation(operation), object(object){}
    worker_work &operator=(const worker_work &other) {
      name = other.name;
      operation = other.operation;
      object = other.object;
      return *this;
    }
  };

  void insert(const std::string& name, const std::string& operation, const std::string& object);
  void print_all();
}
#endif //WORKER_H
