#ifndef TOKENSCANNER_H
#define TOKENSCANNER_H

#include <iostream>
class TokenScanner {
public:
  TokenScanner() = default;
  explicit TokenScanner(const std::string &str) : line(str){}

  std::string nextToken();
  [[nodiscard]] bool hasMoreTokens() const;

private:
  std::string line;
  // const std::string WordCharacters = "+-*/|,._=!@#$%^&()~`{}[]\\:;\"'<>?";
  int pos = 0;
};
#endif //TOKENSCANNER_H
