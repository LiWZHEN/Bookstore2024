#ifndef LOG_H
#define LOG_H
#include "user_data.h"
namespace log {
  const std::string log_file = "log.txt";
  void CreatFileIfNotExist(const std::string &);

  struct system_log {
    user::fixed_char who;
    user::fixed_char what;
    user::fixed_char how;
    system_log() = default;
    system_log(const user::fixed_char &who, const user::fixed_char &what, const user::fixed_char &how)
        : who(who), what(what), how(how){}
    system_log &operator=(const system_log &other) {
      who = other.who;
      what = other.what;
      how = other.how;
      return *this;
    }
  };

  void insert(const std::string &who, const std::string &what, const std::string &how);
  void print_all();
}
#endif //LOG_H
