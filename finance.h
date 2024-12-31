#ifndef FINANCE_H
#define FINANCE_H

#include <fstream>
#include <iomanip>

namespace finance {
  const std::string finance_file = "finance.txt";
  void CreatFileIfNotExist(const std::string &);

  struct add_minus {
    double add = 0;
    double minus = 0;
    add_minus() = default;
    add_minus(const double a, const double m) {
      add = a;
      minus = m;
    }
    add_minus &operator=(const add_minus &other) {
      add = other.add;
      minus = other.minus;
      return *this;
    }
    add_minus &operator+=(const add_minus &other) {
      add += other.add;
      minus += other.minus;
      return *this;
    }
  };

  void insert(double add, double minus);
  void print(int count);
  void print_all();
  void print_table();
}
#endif //FINANCE_H
