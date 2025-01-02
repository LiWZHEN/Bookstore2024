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
bool valid_ID_or_pw(const std::string &);
bool valid_username(const std::string &);
bool valid_privilege(const std::string &);
bool valid_ISBN(const std::string &);
bool valid_B_A_K(const std::string &);
#endif //TOKENSCANNER_H
