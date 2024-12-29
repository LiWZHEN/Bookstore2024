#include <iostream>

#include "book_data.h"
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
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string user_ID = line.nextToken();
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Password = line.nextToken();
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Username = line.nextToken();
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      // the input format is valid, now we have: user_ID && Password && Username
      unsigned long long bp = user::IfExist(user_ID);
      if (bp != 0) { // repeat
        std::cout << "Invalid\n";
        continue;
      }
      // not repeat
      user::AddUser(user_ID, Username, Password, 1, false);
    } else if (token == "passwd") {
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string UserID = line.nextToken();
      if (!line.hasMoreTokens()) { // anyway the input should have one password
        std::cout << "Invalid\n";
        continue;
      }
      std::string password1 = line.nextToken();
      if (!line.hasMoreTokens()) { // need to check the current privilege
        if (logging_stack.size == 0) {
          std::cout << "Invalid\n";
          continue;
        }
        if (logging_stack.log_stack[logging_stack.size - 1].privilege != 7) {
          std::cout << "Invalid\n";
          continue;
        }
        // it is the manager who omit the old password
        user::EditPassword(UserID, password1);
        continue;
      }
      std::string password2 = line.nextToken();
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      // now the input is valid, we have: UserID && password1 && password2
      unsigned long long bp = user::IfExist(UserID);
      if (bp == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      // user exist
      if (user::CheckPassword(UserID, password1) != 1) {
        std::cout << "Invalid\n";
        continue;
      }
      // current password correct
      user::EditPassword(UserID, password2);
    } else if (token == "useradd") {
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string UserID = line.nextToken();
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Password = line.nextToken();
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Privilege = line.nextToken();
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Username = line.nextToken();
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      // input is valid
      // check the privilege
      int privilege = 0;
      for (char c : Privilege) {
        if (c - '0' > 9 || c - '0' < 0) {
          std::cout << "Invalid\n";
          continue;
        }
        privilege = privilege * 10 + (c - '0');
      }
      if (privilege >= logging_stack.log_stack[logging_stack.size - 1].privilege) {
        std::cout << "Invalid\n";
        continue;
      }
      // created privilege is lower than current privilege
      if (user::IfExist(UserID) != 0) {
        std::cout << "Invalid\n";
        continue;
      }
      // do not repeat
      user::AddUser(UserID, Username, Password, privilege, false);
    } else if (token == "delete") {
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string UserID = line.nextToken();
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      unsigned long long bp = user::IfExist(UserID);
      if (bp == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      // user can be found
      if (user::IfOnline(UserID)) {
        std::cout << "Invalid\n";
        continue;
      }
      // not online
      user::Delete(UserID);
    } else if (token == "show") {
      token = line.nextToken();
      if (token == "finance") {

      } else {

      }
    } else if (token == "buy") {
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string ISBN = line.nextToken();
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Quantity = line.nextToken();
      if (Quantity[0] == '-') {
        std::cout << "Invalid\n";
        continue;
      }
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      // we have got ISBN && Quantity
      int quantity = 0;
      for (char c : Quantity) {
        quantity = quantity * 10 + (c - '0');
      }
      unsigned long long bp = book::IfExist(ISBN);
      if (bp == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      // target book exist
      book::book_data target = book::Get_book(ISBN);
      if (target.Storage < Quantity) { // storage insufficient
        std::cout << "Invalid\n";
        continue;
      }
      // can buy
      int new_storage = target.Storage.ToInt() - quantity;
      std::string str_new_storage = book::fixed_char_10(new_storage).ToString();
      book::Edit(ISBN, false, false, false, false,
          true, false, "", "", "",
          "", str_new_storage,"");
      double price = target.Price.ToDouble() * quantity;
      std::cout << price << "\n";
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