#include <iostream>

#include "book_data.h"
#include "StanfordCPPLib/tokenscanner.h"
#include "log_in_and_out.h"
#include "user_data.h"
// #include "MemoryRiver.h"

int main() {
  stack logging_stack;
  while (true) {
    std::string command;
    std::getline(std::cin, command);
    if (command == "quit" || command == "exit") {
      while (logging_stack.size > 0) {
        user::Logout(logging_stack.log_stack[logging_stack.size - 1].username);
        --logging_stack.size;
      }
      // logging_stack.exit();
      break;
    }
    TokenScanner line(command);
    line.ignoreWhitespace();
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
        logging_stack.LogIn(user_ID, "null", block.block[index].privilege);
        user::Login(user_ID);
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
      logging_stack.LogIn(user_ID, "null", block.block[index].privilege);
      user::Login(user_ID);
    } else if (token == "logout") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      user::Logout(logging_stack.log_stack[logging_stack.size - 1].username);
      --logging_stack.size;
      // logging_stack.LogOut();
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
      if (!line.hasMoreTokens()) { // has no more token
        // todo: out put all books
      }
      token = line.nextToken();
      if (token == "finance") {
        if (!line.hasMoreTokens()) {
          // todo: out put overall finance
        }
        std::string count = line.nextToken();
        if (count.length() > 10) {
          std::cout << "Invalid\n";
          continue;
        }
        if (book::fixed_char_10(count).ToInt() > 2147483647) {
          std::cout << "Invalid\n";
          continue;
        }
        if (line.hasMoreTokens()) {
          std::cout << "Invalid\n";
          continue;
        }
        // now we get count
        // todo: output the sum of latest "count" number of finance
      } else {
        int len = token.length();
        if (token[0] != '-') {
          std::cout << "Invalid\n";
          continue;
        }
        std::string chop; // to see the category
        int it = 1;
        while(token[it] != '=' && it < len) {
          chop += token[it];
          ++it;
        }
        ++it; // skip '='
        if (it >= len) {
          std::cout << "Invalid\n";
          continue;
        }

        if (chop == "ISBN") {
          std::string isbn;
          for (; it < len; ++it) {
            isbn += token[it];
          }
          book::ShowBook(isbn);
        } else if (chop == "name") {
          std::string name;
          for (; it < len; ++it) {
            name += token[it];
          }
          // todo: create a "name->ISBN" file manager to handle this
        } else if (chop == "author") {
          std::string author;
          for (; it < len; ++it) {
            author += token[it];
          }
          // todo: create a "author->ISBN" file manager to handle this
        } else if (chop == "keyword") {
          std::string keyword;
          for (; it < len; ++it) {
            keyword += token[it];
          }
          // todo: create a "keyword->ISBN" file manager to handle this
        } else {
          std::cout << "Invalid\n";
          continue;
        }
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
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string ISBN = line.nextToken();
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      // now we get ISBN and promise there is someone who is online
      unsigned long long bp = book::IfExist(ISBN);
      if (bp == 0) { // not exist currently, add the book to book_file
        book::AddBook(ISBN, "", "", "", 0, 0);
      }
      logging_stack.log_stack[logging_stack.size - 1].selected_book = ISBN;
    } else if (token == "modify") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string selected_ISBN = logging_stack.log_stack[logging_stack.size - 1].selected_book;
      if (selected_ISBN.empty()) { // select no book
        std::cout << "Invalid\n";
        continue;
      }
      // there is a book selected, whose ID is selected_ISBN
      if (!line.hasMoreTokens()) { // empty command, invalid
        std::cout << "Invalid\n";
        continue;
      }
      bool edit_ISBN = false, edit_name = false, edit_author = false, edit_keyword = false, edit_price = false;
      std::string new_ISBN, new_name, new_author, new_keyword, new_price;
      bool repeated = false;

      // read commands until there is no more commands or the same category occurs more than one time
      while (line.hasMoreTokens() && !repeated) {

        std::string cmd = line.nextToken(); // the whole command for one time
        if (cmd[0] != '-') {
          std::cout << "Invalid\n";
          repeated = true; // the variable name makes no sense here
          break;
        }
        int len = cmd.length();
        std::string chop; // to see the category
        int it = 1;
        while(cmd[it] != '=' || it < len) {
          chop += cmd[it];
          ++it;
        }
        ++it; // skip '='
        if (it >= len) {
          repeated = true; // the variable name makes no sense here, too
          break;
        }

        if (chop == "ISBN") {
          if (edit_ISBN) {
            repeated = true;
          }
          edit_ISBN = true;
          for (; it < len; ++it) {
            new_ISBN += cmd[it];
          }
        } else if (chop == "name") {
          if (edit_name) {
            repeated = true;
          }
          edit_name = true;
          for (; it < len; ++it) {
            new_name += cmd[it];
          }
        } else if (chop == "author") {
          if (edit_author) {
            repeated = true;
          }
          edit_author = true;
          for (; it < len; ++it) {
            new_author += cmd[it];
          }
        } else if (chop == "keyword") {
          if (edit_keyword) {
            repeated = true;
          }
          edit_keyword = true;
          for (; it < len; ++it) {
            new_keyword += cmd[it];
          }
        } else if (chop == "price") {
          if (edit_price) {
            repeated = true;
          }
          edit_price = true;
          for (; it < len; ++it) {
            new_price += cmd[it];
          }
        } else {
          repeated = true;
          break;
        }
      }
      if (repeated) {
        std::cout << "Invalid\n";
        continue;
      }
      if (edit_ISBN) {
        unsigned long long bp = book::IfExist(new_ISBN); // check if the ISBN has existed
        if (bp != 0) {
          std::cout << "Invalid\n";
          continue;
        }
        // only when the ISBN repetition occurs, the check will have a real impact
      }
      if (edit_keyword) {
        // todo: check whether the key words have repetition, if repeated, print invalid and continue

      }
      book::Edit(selected_ISBN, edit_ISBN, edit_name, edit_author, edit_keyword, false, edit_price,
          new_ISBN, new_name, new_author, new_keyword, "", new_price);
    } else if (token == "import") {

    } else if (token == "log") {

    } else if (token == "report") {
      token = line.nextToken();
      if (token == "finance") {

      } else if (token == "employee") {

      } else {
        std::cout << "Invalid\n";
        continue;
      }
    } else {
      std::cout << "Invalid\n";
      continue;
    }
  }
  return 0;
}