#ifndef LOG_IN_AND_OUT_H
#define LOG_IN_AND_OUT_H

#include "user_data.h"

const int stack_capacity = 1000;

struct user_book_privilege {
  std::string username;
  std::string selected_book;
  int privilege;
};

class stack {
public:
  stack() = default;
  void LogIn(std::string name, std::string book, int privilege) {
    log_stack[size] = {name, book, privilege};
    ++size;
  }
  void LogOut() {
    --size;
  }
  user_book_privilege log_stack[stack_capacity];
  int size = 0;
};

#endif //LOG_IN_AND_OUT_H
