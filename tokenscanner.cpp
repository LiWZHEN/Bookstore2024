#include "tokenscanner.h"
#include <cctype>

bool TokenScanner::line_valid() const {
  for (int i = 0; i < line.length(); ++i) {
    if (line[i] != ' ' && std::isspace(line[i])) {
      return false;
    }
  }
  return true;
}

std::string TokenScanner::nextToken() {
  bool space_end = false;
  std::string next_token;
  for (; pos < line.length(); ++pos) {
    if (line[pos] == '\0') {
      return next_token;
    }
    if (line[pos] == ' ') {
      if (!space_end) {
        continue;
      }
      break;
    }
    if (line[pos] != ' ') {
      space_end = true;
      next_token += line[pos];
    }
  }
  return next_token;
}

bool TokenScanner::hasMoreTokens() const {
  int p = pos;
  for (; p < line.length(); ++p) {
    if (line[p] != ' ' && line[p] != '\0') {
      return true;
    }
  }
  return false;
}

bool valid_int(const std::string &str) {
  int ans = 0;
  int digit = 0;
  char first = '\0';
  for (char c : str) {
    if (c == '\0') {
      return first != '\0';
    }
    if (c < '0' || c > '9') {
      return false;
    }
    ans = ans * 10 + (c - '0');
    ++digit;
    if (first == '\0') {
      first = c;
    }
    if (ans < 0 || digit > 10 || digit == 10 && first > '2') {
      return false;
    }
  }
  return true;
}

bool valid_double(const std::string &str) {
  bool dot = false;
  std::string int_part;
  int float_part = 0;
  int i = 0;
  for (; i < str.length(); ++i) {
    if (str[i] == '\0') {
      return valid_int(int_part);
    }
    if (str[i] == '.') {
      if (!valid_int(int_part)) {
        return false;
      }
      dot = true;
      ++i;
      break;
    }
    if (str[i] >= '0' && str[i] <= '9') {
      int_part += str[i];
    } else {
      return false;
    }
  }
  if (!dot) {
    return valid_int(int_part);
  }
  for (; i < str.length(); ++i) {
    if (str[i] == '\0') {
      break;
    }
    if (str[i] == '.') {
      return false;
    }
    if (str[i] >= '0' && str[i] <= '9') {
      ++float_part;
    } else {
      return false;
    }
  }
  return float_part == 1 || float_part == 2;
}

bool valid_string(const std::string &str, int size) {
  int n = 0;
  for (char c : str) {
    if (c == '\0') {
      return n <= size;
    }
    ++n;
  }
  return n <= size;
}

bool valid_ID_or_pw(const std::string &str) {
  int len = 0;
  for (char c : str) {
    if (c == '\0') {
      return len != 0;
    }
    if ((c < '0' || c > '9') && (c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && c != '_') {
      return false;
    }
    ++len;
    if (len > 30) {
      return false;
    }
  }
  return len > 0;
}

bool valid_privilege(const std::string &str) {
  if (!valid_int(str)) {
    return false;
  }
  int a = 0;
  for (char c : str) {
    if (c == '\0') {
      return a == 1;
    }
    if (c != '0' && c != '1' && c != '3' && c != '7') {
      return false;
    }
    ++a;
    if (a > 1) {
      return false;
    }
  }
  return a == 1;
}

bool valid_username(const std::string &str) {
  return valid_string(str, 30);
}

bool valid_ISBN(const std::string &str) {
  return valid_string(str, 20);
}

bool valid_B_A_K(const std::string &str) {
  if(!valid_string(str, 60)) {
    return false;
  }
  for (char c : str) {
    if (c == '\"') {
      return false;
    }
  }
  return true;
}
