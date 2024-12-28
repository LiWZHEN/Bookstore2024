#include <iostream>
#include "StanfordCPPLib/tokenscanner.h"
#include "log_in_and_out.h"
#include "user_data.h"

int main() {
  stack logging_stack;
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
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string user_ID = line.nextToken();
      if (user::IfExist(user_ID) == 0) { // user do not exist
        std::cout << "Invalid\n";
        continue;
      }
      // now we can confirm the user do exist
      user::Block block;
      int index;
      if (!line.hasMoreTokens()) { // didn't provide the password, only when the privilege is higher, it's valid
        if (logging_stack.size == 0) {
          std::cout << "Invalid\n";
          continue;
        }
        // now we can confirm there is someone who has logged in, just compare the privilege
        unsigned long long bp = user::IfExist(user_ID); // get block position
        std::fstream file(user::user_file, std::ios::in | std::ios::binary);
        file.seekg(bp);
        file.read(reinterpret_cast<char *>(&block), sizeof(block));
        index = block.FindUpper(user_ID);
        if (logging_stack.log_stack[logging_stack.size - 1].privilege <= block.block[index].privilege) { // not higher
          std::cout << "Invalid\n";
          continue;
        }

        // the privilege is higher
        logging_stack.LogIn(block.block[index].Name.ToString(), "null", block.block[index].privilege);
        continue;
      }

      // password provided
      std::string password = line.nextToken();
      int result = user::CheckPassword(user_ID, password);
      if (result != 1) { // the password is wrong
        std::cout << "Invalid\n";
        continue;
      }

      // right password
      if (line.hasMoreTokens()) { // too much commands
        std::cout << "Invalid\n";
        continue;
      }
      logging_stack.LogIn(block.block[index].Name.ToString(), "null", block.block[index].privilege);
    } else if (token == "logout") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      logging_stack.LogOut();
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