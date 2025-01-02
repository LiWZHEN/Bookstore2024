#ifndef TOKENSCANNER_H
#define TOKENSCANNER_H

#include <iostream>
class TokenScanner {
public:
  TokenScanner() = default;
  explicit TokenScanner(const std::string &str) : line(str) {}

  bool line_valid() const;
  std::string nextToken();
  [[nodiscard]] bool hasMoreTokens() const;

private:
  std::string line;
  int pos = 0;
};

bool valid_int(const std::string &);
bool valid_double(const std::string &);
bool valid_string(const std::string &, int);

#endif //TOKENSCANNER_H
