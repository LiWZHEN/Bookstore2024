#include <iostream>
#include <vector>
#include "StanfordCPPLib/tokenscanner.h"
#include "hash.h"

int main() {
  std::vector<db_hash> logged_users;
  std::vector<db_hash> selected_books;

  while (true) {
    std::string command;
    std::getline(std::cin, command);
    if (command == "quit" || command == "exit") {
      break;
    }
    TokenScanner line(command);
    std::string token;
    token = line.nextToken();
    if (token == "su") {

    } else if (token == "logout") {

    } else if (token == "register") {

    } else if (token == "passwd") {

    } else if (token == "useradd") {

    } else if (token == "delete") {

    } else if (token == "show") {
      token = line.nextToken();
      if (token == "finance") {

      } else {

      }
    } else if (token == "buy") {

    } else if (token == "select") {

    } else if (token == "modify") {

    } else if (token == "import") {

    } else if (token == "log") {

    } else if (token == "report") {
      token = line.nextToken();
      if (token == "finance") {

      } else if (token == "employee") {

      } else {
        std::cout << "Invalid\n";
      }
    }
  }
  return 0;
}