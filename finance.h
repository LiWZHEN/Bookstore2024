#ifndef FINANCE_H
#define FINANCE_H

#include <vector>
#include <iomanip>

namespace finance {
  struct add_minus {
    double add = 0;
    double minus = 0;
  };
  std::vector<add_minus> finance;
  void insert(double add, double minus) {
    for (int i = 0; i < finance.size(); ++i) {
      finance[i].add += add;
      finance[i].minus += minus;
    }
    finance.push_back({add, minus});
  }
  void print(int count) {
    std::cout << std::fixed << std::setprecision(2) << "+ " << finance[finance.size() - count].add
        << " - " << finance[finance.size() - count].minus << "\n";
  }
}
#endif //FINANCE_H
