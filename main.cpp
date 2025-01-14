#include <iostream>
#include "book_data.h"
#include "log_in_and_out.h"
#include "user_data.h"
#include "finance.h"
#include "book_name_processor.h"
#include "author_processor.h"
#include "key_processor.h"
#include "key_file.h"
#include "log.h"
#include "worker.h"
#include "tokenscanner.h"

int main() {
  stack logging_stack;
  std::string command;
  while (true) {
    if (!std::getline(std::cin, command)) {
      while (logging_stack.size > 0) {
        user::Logout(logging_stack.log_stack[logging_stack.size - 1].username);
        --logging_stack.size;
      }
      break;
    }
    TokenScanner line(command);
    if (!line.line_valid()) {
      std::cout << "Invalid\n";
      continue;
    }
    std::string token;
    token = line.nextToken();
    if (token == "quit" || token == "exit") {
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      while (logging_stack.size > 0) {
        user::Logout(logging_stack.log_stack[logging_stack.size - 1].username);
        --logging_stack.size;
      }
      break;
    }
    if (token == "su") {
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string user_ID = line.nextToken();
      if (!valid_ID_or_pw(user_ID)) {
        std::cout << "Invalid\n";
        continue;
      }
      unsigned long long bp = user::IfExist(user_ID);
      if (bp == 0) { // user do not exist
        std::cout << "Invalid\n";
        continue;
      }
      // now we can confirm the user do exist
      if (!line.hasMoreTokens()) { // didn't provide the password, only when the privilege is higher, it's valid
        if (logging_stack.size == 0) {
          std::cout << "Invalid\n";
          continue;
        }
        // now we can confirm there is someone who has logged in, just compare the privilege
        user::Block block;
        int index;
        int priv;
        std::fstream file(user::user_file, std::ios::in | std::ios::binary);
        file.seekg(bp);
        file.read(reinterpret_cast<char *>(&block), sizeof(block));
        file.close();
        index = block.FindUpper(user_ID);
        priv = block.block[index].privilege;
        if (logging_stack.log_stack[logging_stack.size - 1].privilege <= priv) { // not higher
          std::cout << "Invalid\n";
          continue;
        }

        // the privilege is higher
        logging_stack.LogIn(user_ID, "", priv);
        user::Login(user_ID);
        continue;
      }

      // password provided
      std::string password = line.nextToken();
      if (!valid_ID_or_pw(password)) {
        std::cout << "Invalid\n";
        continue;
      }
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
      user::Block block;
      int index;
      int priv;
      std::fstream file(user::user_file, std::ios::in | std::ios::binary);
      file.seekg(bp);
      file.read(reinterpret_cast<char *>(&block), sizeof(block));
      file.close();

      index = block.FindUpper(user_ID);
      priv = block.block[index].privilege;
      logging_stack.LogIn(user_ID, "", priv);
      user::Login(user_ID);

    } else if (token == "logout") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      user::Logout(logging_stack.log_stack[logging_stack.size - 1].username);
      --logging_stack.size;
    } else if (token == "register") {
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string user_ID = line.nextToken();
      if (!valid_ID_or_pw(user_ID)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Password = line.nextToken();
      if (!valid_ID_or_pw(Password)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Username = line.nextToken();
      if (!valid_username(Username)) {
        std::cout << "Invalid\n";
        continue;
      }
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
      user::AddUser(user_ID, Username, Password, 1, 0);
    } else if (token == "passwd") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      if (logging_stack.log_stack[logging_stack.size - 1].privilege < 1) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string UserID = line.nextToken();
      if (!valid_ID_or_pw(UserID)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) { // anyway the input should have one password
        std::cout << "Invalid\n";
        continue;
      }
      std::string password1 = line.nextToken();
      if (!valid_ID_or_pw(password1)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) { // need to check the current privilege
        if (logging_stack.log_stack[logging_stack.size - 1].privilege != 7) {
          std::cout << "Invalid\n";
          continue;
        }
        // it is the manager who omit the old password
        if (user::IfExist(UserID) == 0) {
          std::cout << "Invalid\n";
          continue;
        }
        user::EditPassword(UserID, password1);
        continue;
      }
      std::string password2 = line.nextToken();
      if (!valid_ID_or_pw(password2)) {
        std::cout << "Invalid\n";
        continue;
      }
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
      std::string workerID = logging_stack.log_stack[logging_stack.size - 1].username;
      worker::insert(workerID, "edit password", UserID);
      log::insert(workerID, "edit_password", UserID);
    } else if (token == "useradd") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      int current_privilege = logging_stack.log_stack[logging_stack.size - 1].privilege;
      if (current_privilege < 3) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string UserID = line.nextToken();
      if (!valid_ID_or_pw(UserID)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Password = line.nextToken();
      if (!valid_ID_or_pw(Password)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Privilege = line.nextToken();
      if (!valid_privilege(Privilege)) {
        std::cout << "Invalid\n";
        continue;
      }
      int privilege;
      if (Privilege == "0") {
        privilege = 0;
      } else if (Privilege == "1") {
        privilege = 1;
      } else if (Privilege == "3") {
        if (current_privilege == 3) {
          std::cout << "Invalid\n";
          continue;
        }
        privilege = 3;
      } else {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Username = line.nextToken();
      if (!valid_username(Username)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      // input is valid && privilege is valid
      if (user::IfExist(UserID) != 0) {
        std::cout << "Invalid\n";
        continue;
      }
      // do not repeat
      user::AddUser(UserID, Username, Password, privilege, false);
      std::string workerID = logging_stack.log_stack[logging_stack.size - 1].username;
      worker::insert(workerID, "add user", UserID);
      log::insert(workerID, "add user", UserID);
    } else if (token == "delete") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      int current_privilege = logging_stack.log_stack[logging_stack.size - 1].privilege;
      if (current_privilege < 7) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string UserID = line.nextToken();
      if (!valid_ID_or_pw(UserID)) {
        std::cout << "Invalid\n";
        continue;
      }
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
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) { // has no more token
        book::ShowAll();
        continue;
      }
      token = line.nextToken();
      if (token == "finance") {
        int current_privilege = logging_stack.log_stack[logging_stack.size - 1].privilege;
        if (current_privilege < 7) {
          std::cout << "Invalid\n";
          continue;
        }
        if (!line.hasMoreTokens()) {
          finance::print_all();
          continue;
        }
        std::string count = line.nextToken();
        if (!valid_int(count)) {
          std::cout << "Invalid\n";
          continue;
        }
        int c = book::fixed_char_10(count).ToInt();
        if (line.hasMoreTokens()) {
          std::cout << "Invalid\n";
          continue;
        }
        finance::print(c);
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
          if (!valid_ISBN(isbn)) {
            std::cout << "Invalid\n";
            continue;
          }
          book::ShowBook(isbn);
        } else if (chop == "name") {
          std::string name;
          if (token[it] != '\"') {
            std::cout << "Invalid\n";
            continue;
          }
          ++it;
          int m = token.length() - 1;
          while (token[m] == '\0') {
            --m;
          }
          if (token[m] != '\"') {
            std::cout << "Invalid\n";
            continue;
          }
          if (it >= m) {
            std::cout << "Invalid\n";
            continue;
          }
          bool valid = true;
          int n = 0;
          for (; it < m; ++it) {
            if (token[it] == '\"' || token[it] == ' ') {
              valid = false;
              break;
            }
            name += token[it];
            ++n;
            if (n > 60) {
              valid = false;
              break;
            }
          }
          if (!valid) {
            std::cout << "Invalid\n";
            continue;
          }
          std::vector<std::string> ISBN_set = book_name::find(name);
          if (ISBN_set.empty()) {
            std::cout << "\n";
            continue;
          }
          for (int i = 0; i < ISBN_set.size(); ++i) {
            book::book_data bk = book::Get_book(ISBN_set[i]);
            double price = bk.Price.ToDouble();
            std::cout << bk.ISBN << "\t" << bk.BookName << "\t" << bk.Author << "\t" << bk.Keyword
                << "\t" << std::fixed << std::setprecision(2) << price << "\t" << bk.Storage << "\n";
          }
        } else if (chop == "author") {
          std::string author;
          if (token[it] != '\"') {
            std::cout << "Invalid\n";
            continue;
          }
          ++it;
          int m = token.length() - 1;
          while (token[m] == '\0') {
            --m;
          }
          if (token[m] != '\"') {
            std::cout << "Invalid\n";
            continue;
          }
          if (it >= m) {
            std::cout << "Invalid\n";
            continue;
          }
          bool valid = true;
          int n = 0;
          for (; it < m; ++it) {
            if (token[it] == '\"' || token[it] == ' ') {
              valid = false;
              break;
            }
            author += token[it];
            ++n;
            if (n > 60) {
              valid = false;
              break;
            }
          }
          if (!valid) {
            std::cout << "Invalid\n";
            continue;
          }
          std::vector<std::string> ISBN_set = author::find(author);
          if (ISBN_set.empty()) {
            std::cout << "\n";
            continue;
          }
          for (int i = 0; i < ISBN_set.size(); ++i) {
            book::book_data bk = book::Get_book(ISBN_set[i]);
            double price = bk.Price.ToDouble();
            std::cout << bk.ISBN << "\t" << bk.BookName << "\t" << bk.Author << "\t" << bk.Keyword
                << "\t" << std::fixed << std::setprecision(2) << price << "\t" << bk.Storage << "\n";
          }
        } else if (chop == "keyword") {
          std::string keyword;
          if (token[it] != '\"') {
            std::cout << "Invalid\n";
            continue;
          }
          ++it;
          int m = token.length() - 1;
          while (token[m] == '\0') {
            --m;
          }
          if (token[m] != '\"') {
            std::cout << "Invalid\n";
            continue;
          }
          if (it >= m) {
            std::cout << "Invalid\n";
            continue;
          }
          bool valid = true;
          int n = 0;
          for (; it < m; ++it) {
            if (token[it] == '\"' || token[it] == ' ') {
              valid = false;
              break;
            }
            keyword += token[it];
            ++n;
            if (n > 60) {
              valid = false;
              break;
            }
          }
          if (!valid) {
            std::cout << "Invalid\n";
            continue;
          }
          if (!valid_key(keyword)) {
            std::cout << "Invalid\n";
            continue;
          }
          std::vector<std::string> key = key::split(keyword);
          if (key.size() > 1) {
            std::cout << "Invalid\n";
            continue;
          }
          std::vector<std::string> ISBN_set = keyword::find(key[0]);
          if (ISBN_set.empty()) {
            std::cout << "\n";
            continue;
          }
          for (int i = 0; i < ISBN_set.size(); ++i) {
            book::book_data bk = book::Get_book(ISBN_set[i]);
            double price = bk.Price.ToDouble();
            int storage = bk.Storage.ToInt();
            std::cout << bk.ISBN << "\t" << bk.BookName << "\t" << bk.Author << "\t" << bk.Keyword
                << "\t" << std::fixed << std::setprecision(2) << price << "\t" << storage << "\n";
          }
        } else {
          std::cout << "Invalid\n";
          continue;
        }
      }
    } else if (token == "buy") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string ISBN = line.nextToken();
      if (!valid_ISBN(ISBN)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Quantity = line.nextToken();
      if (!valid_int(Quantity)) {
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
        if (c == '\0') {
          break;
        }
        quantity = quantity * 10 + (c - '0');
      }
      if (quantity == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      unsigned long long bp = book::IfExist(ISBN);
      if (bp == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      // target book exist
      book::book_data target = book::Get_book(ISBN);
      if (target.Storage.ToInt() < quantity) { // storage insufficient
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
      std::cout << std::fixed << std::setprecision(2) << price << "\n";
      finance::insert(price, 0);
      log::insert("sell book", "profit", book::fixed_char_13(price).fcg);
    } else if (token == "select") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      int current_privilege = logging_stack.log_stack[logging_stack.size - 1].privilege;
      if (current_privilege < 3) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string ISBN = line.nextToken();
      if (!valid_ISBN(ISBN)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      // now we get ISBN and promise there is someone who is online
      unsigned long long bp = book::IfExist(ISBN);
      if (bp == 0) { // not exist currently, add the book to book_file
        book::AddBook(ISBN, "", "", "", 0, 0);
        book_name::insert("", ISBN);
        author::insert("", ISBN);
        key::insert("", ISBN);
      }
      logging_stack.log_stack[logging_stack.size - 1].selected_book = ISBN;
    } else if (token == "modify") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      int current_privilege = logging_stack.log_stack[logging_stack.size - 1].privilege;
      if (current_privilege < 3) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string selected_ISBN = logging_stack.log_stack[logging_stack.size - 1].selected_book;
      if (!valid_string(selected_ISBN, 20)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (selected_ISBN.empty()) { // select no book
        std::cout << "Invalid\n";
        continue;
      }
      unsigned long long bp = book::IfExist(selected_ISBN);
      if (bp == 0) {
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
        while(cmd[it] != '=' && it < len) {
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
          if (!valid_ISBN(new_ISBN)) {
            repeated = true; // the variable name makes no sense here, too
            break;
          }
        } else if (chop == "name") {
          if (edit_name) {
            repeated = true;
          }
          edit_name = true;
          if (cmd[it] != '\"') {
            repeated = true; // the variable name makes no sense here, too
            break;
          }
          ++it;
          int m = cmd.length() - 1;
          while (cmd[m] == '\0') {
            --m;
          }
          if (cmd[m] != '\"') {
            repeated = true; // the variable name makes no sense here, too
            break;
          }
          if (it >= m) {
            repeated = true; // the variable name makes no sense here, too
            break;
          }
          int n = 0;
          for (; it < m; ++it) {
            if (cmd[it] == '\"' || cmd[it] == ' ') {
              repeated = true; // the variable name makes no sense here, too
              break;
            }
            new_name += cmd[it];
            ++n;
            if (n > 60) {
              repeated = true; // the variable name makes no sense here, too
              break;
            }
          }
        } else if (chop == "author") {
          if (edit_author) {
            repeated = true;
          }
          edit_author = true;
          if (cmd[it] != '\"') {
            repeated = true; // the variable name makes no sense here, too
            break;
          }
          ++it;
          int m = cmd.length() - 1;
          while (cmd[m] == '\0') {
            --m;
          }
          if (cmd[m] != '\"') {
            repeated = true; // the variable name makes no sense here, too
            break;
          }
          if (it >= m) {
            repeated = true; // the variable name makes no sense here, too
            break;
          }
          int n = 0;
          for (; it < m; ++it) {
            if (cmd[it] == '\"' || cmd[it] == ' ') {
              repeated = true; // the variable name makes no sense here, too
              break;
            }
            new_author += cmd[it];
            ++n;
            if (n > 60) {
              repeated = true; // the variable name makes no sense here, too
              break;
            }
          }
        } else if (chop == "keyword") {
          if (edit_keyword) {
            repeated = true;
          }
          edit_keyword = true;
          if (cmd[it] != '\"') {
            repeated = true; // the variable name makes no sense here, too
            break;
          }
          ++it;
          int m = cmd.length() - 1;
          while (cmd[m] == '\0') {
            --m;
          }
          if (cmd[m] != '\"') {
            repeated = true; // the variable name makes no sense here, too
            break;
          }
          if (it >= m) {
            repeated = true; // the variable name makes no sense here, too
            break;
          }
          int n = 0;
          for (; it < m; ++it) {
            if (cmd[it] == '\"' || cmd[it] == ' ') {
              repeated = true; // the variable name makes no sense here, too
              break;
            }
            new_keyword += cmd[it];
            ++n;
            if (n > 60) {
              repeated = true; // the variable name makes no sense here, too
              break;
            }
          }
          if (!valid_key(new_keyword)) {
            repeated = true; // the variable name makes no sense here, too
            break;
          }
        } else if (chop == "price") {
          if (edit_price) {
            repeated = true;
          }
          edit_price = true;
          for (; it < len; ++it) {
            new_price += cmd[it];
          }
          if (!valid_double(new_price)) {
            repeated = true;
            break;
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
        unsigned long long blp = book::IfExist(new_ISBN); // check if the ISBN has existed
        if (blp != 0) {
          std::cout << "Invalid\n";
          continue;
        }
        // only when the ISBN repetition occurs, the check will have a real impact
      }
      if (edit_keyword) {
        if (key::Repeated(new_keyword)) {
          std::cout << "Invalid\n";
          continue;
        }
      }
      book::book_data bk = book::Get_book(selected_ISBN);
      if (edit_ISBN || edit_name) {
        book::fixed_char_60 original_name(bk.BookName);
        book_name::erase(original_name.ToString(), selected_ISBN);
        if (edit_ISBN && edit_name) {
          book_name::insert(new_name, new_ISBN);
        } else if (edit_ISBN) {
          book_name::insert(original_name.ToString(), new_ISBN);
        } else {
          book_name::insert(new_name, selected_ISBN);
        }
      }
      if (edit_ISBN || edit_author) {
        book::fixed_char_60 original_author(bk.Author);
        author::erase(original_author.ToString(), selected_ISBN);
        if (edit_ISBN && edit_author) {
          author::insert(new_author, new_ISBN);
        } else if (edit_ISBN) {
          author::insert(original_author.ToString(), new_ISBN);
        } else {
          author::insert(new_author, selected_ISBN);
        }
      }
      if (edit_ISBN || edit_keyword) {
        book::fixed_char_60 original_keyword(bk.Keyword);
        std::vector<std::string> original_keys = key::split(original_keyword.ToString());
        for (int i = 0; i < original_keys.size(); ++i) {
          keyword::erase(original_keys[i], selected_ISBN);
        }
        if (edit_ISBN && edit_keyword) {
          std::vector<std::string> new_keys = key::split(new_keyword);
          for (int i = 0; i < new_keys.size(); ++i) {
            keyword::insert(new_keys[i], new_ISBN);
          }
        } else if (edit_ISBN) {
          for (int i = 0; i < original_keys.size(); ++i) {
            keyword::insert(original_keys[i], new_ISBN);
          }
        } else {
          std::vector<std::string> new_keys = key::split(new_keyword);
          for (int i = 0; i < new_keys.size(); ++i) {
            keyword::insert(new_keys[i], selected_ISBN);
          }
        }
      }
      if (edit_ISBN || edit_name || edit_author || edit_keyword || edit_price) {
        std::string workerID = logging_stack.log_stack[logging_stack.size - 1].username;
        std::string operation = "edit";
        if (edit_ISBN) {
          operation += "_I";
          for (int i = 0; i < logging_stack.size; ++i) {
            if (logging_stack.log_stack[i].selected_book == selected_ISBN) {
              logging_stack.log_stack[i].selected_book = new_ISBN;
            }
          }
        }
        if (edit_name) {
          operation += "_N";
        }
        if (edit_author) {
          operation += "_A";
        }
        if (edit_keyword) {
          operation += "_K";
        }
        if (edit_price) {
          operation += "_P";
        }
        book::Edit(selected_ISBN, edit_ISBN, edit_name, edit_author, edit_keyword, false, edit_price,
            new_ISBN, new_name, new_author, new_keyword, "", new_price);
        worker::insert(workerID, operation, selected_ISBN);
        log::insert(workerID, operation, selected_ISBN);
      }
    } else if (token == "import") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      user_book_privilege current_online = logging_stack.log_stack[logging_stack.size - 1];
      int current_privilege = current_online.privilege;
      if (current_privilege < 3) {
        std::cout << "Invalid\n";
        continue;
      }
      if (current_online.selected_book.empty()) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string Quantity = line.nextToken();
      if (!valid_int(Quantity)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string TotalCost = line.nextToken();
      if (!valid_double(TotalCost)) {
        std::cout << "Invalid\n";
        continue;
      }
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      int quantity = book::fixed_char_10(Quantity).ToInt();
      if (quantity <= 0) {
        std::cout << "Invalid\n";
        continue;
      }
      book::fixed_char_10 Storage = book::Get_book(current_online.selected_book).Storage;
      quantity += Storage.ToInt();
      std::string Q = book::fixed_char_10(quantity).ToString();
      double total_cost = book::fixed_char_13(TotalCost).ToDouble();
      if (total_cost <= 0) {
        std::cout << "Invalid\n";
        continue;
      }
      book::Edit(current_online.selected_book, false, false, false,
          false, true, false, "", "", "",
          "", Q, "");
      finance::insert(0, total_cost);
      log::insert(current_online.username, "spend money", TotalCost);
      worker::insert(current_online.username, "import book", current_online.selected_book);
      log::insert(current_online.username, "import book", current_online.selected_book);
    } else if (token == "log") {
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      int current_privilege = logging_stack.log_stack[logging_stack.size - 1].privilege;
      if (current_privilege < 7) {
        std::cout << "Invalid\n";
        continue;
      }
      log::print_all();
    } else if (token == "report") {
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      int current_privilege = logging_stack.log_stack[logging_stack.size - 1].privilege;
      if (current_privilege < 7) {
        std::cout << "Invalid\n";
        continue;
      }
      token = line.nextToken();
      if (token == "finance") {
        if (line.hasMoreTokens()) {
          std::cout << "Invalid\n";
          continue;
        }
        finance::print_table();
      } else if (token == "employee") {
        if (line.hasMoreTokens()) {
          std::cout << "Invalid\n";
          continue;
        }
        worker::print_all();
      } else {
        std::cout << "Invalid\n";
      }
    } else if (token.empty()) {
    } else {
      std::cout << "Invalid\n";
    }
  }
  return 0;
}
