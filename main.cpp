#include <iostream>

#include "book_data.h"
#include "StanfordCPPLib/tokenscanner.h"
#include "log_in_and_out.h"
#include "user_data.h"
#include "finance.h"
#include "book_name_processor.h"
#include "author_processor.h"
#include "key_processor.h"
#include "key_file.h"

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
    line.addWordCharacters("+-*/|,._=!@#$%^&()~`{}[]\\:;\"<>?");
    std::string token;
    token = line.nextToken();
    if (token == "su") {
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string user_ID = line.nextToken();
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
        logging_stack.LogIn(user_ID, "null", priv);
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
      user::Block block;
      int index;
      int priv;
      std::fstream file(user::user_file, std::ios::in | std::ios::binary);
      file.seekg(bp);
      file.read(reinterpret_cast<char *>(&block), sizeof(block));
      file.close();

      index = block.FindUpper(user_ID);
      priv = block.block[index].privilege;
      logging_stack.LogIn(user_ID, "null", priv);
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
      if (!line.hasMoreTokens()) { // anyway the input should have one password
        std::cout << "Invalid\n";
        continue;
      }
      std::string password1 = line.nextToken();
      if (!line.hasMoreTokens()) { // need to check the current privilege
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
        // todo: out put all books
        continue;
      }
      token = line.nextToken();
      if (token == "finance") {
        int current_privilege = logging_stack.log_stack[logging_stack.size - 1].privilege;
        if (current_privilege < 3) {
          std::cout << "Invalid\n";
          continue;
        }
        if (!line.hasMoreTokens()) {
          finance::print(finance::finance.size());
        }
        std::string count = line.nextToken();
        if (count.length() > 10) {
          std::cout << "Invalid\n";
          continue;
        }
        bool valid_c = true;
        for (int i = 0; i < count.length(); ++i) {
          if (count[i] < '0' || count[i] > '9') {
            valid_c = false;
            break;
          }
        }
        if (!valid_c) {
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
        int c = book::fixed_char_10(count).ToInt();
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
          book::ShowBook(isbn);
        } else if (chop == "name") {
          std::string name;
          for (; it < len; ++it) {
            name += token[it];
          }
          std::vector<std::string> ISBN_set = book_name::find(name);
          if (ISBN_set.empty()) {
            std::cout << "\n";
            continue;
          }
          for (int i = 0; i < ISBN_set.size(); ++i) {
            book::book_data bk = book::Get_book(ISBN_set[i]);
            std::cout << bk.ISBN << "\t" << bk.BookName << "\t" << bk.Author << "\t" << bk.Keyword
                << "\t" << bk.Price << "\t" << bk.Storage << "\n";
          }
        } else if (chop == "author") {
          std::string author;
          for (; it < len; ++it) {
            author += token[it];
          }
          std::vector<std::string> ISBN_set = author::find(author);
          if (ISBN_set.empty()) {
            std::cout << "\n";
            continue;
          }
          for (int i = 0; i < ISBN_set.size(); ++i) {
            book::book_data bk = book::Get_book(ISBN_set[i]);
            std::cout << bk.ISBN << "\t" << bk.BookName << "\t" << bk.Author << "\t" << bk.Keyword
                << "\t" << bk.Price << "\t" << bk.Storage << "\n";
          }
        } else if (chop == "keyword") {
          std::string keyword;
          for (; it < len; ++it) {
            keyword += token[it];
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
            std::cout << bk.ISBN << "\t" << bk.BookName << "\t" << bk.Author << "\t" << bk.Keyword
                << "\t" << bk.Price << "\t" << bk.Storage << "\n";
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
      finance::insert(price, 0);
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
      if (line.hasMoreTokens()) {
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
      int current_privilege = logging_stack.log_stack[logging_stack.size - 1].privilege;
      if (current_privilege < 3) {
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
        if (key::Repeated(new_keyword)) {
          std::cout << "Invalid\n";
          continue;
        }
      }
      if (edit_ISBN || edit_name) {
        book::fixed_char_60 original_name = book::Get_book(selected_ISBN).BookName;
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
        book::fixed_char_60 original_author = book::Get_book(selected_ISBN).Author;
        author::erase(original_author.ToString(), selected_ISBN);
        if (edit_ISBN && edit_author) {
          author::insert(new_name, new_ISBN);
        } else if (edit_ISBN) {
          author::insert(original_author.ToString(), new_ISBN);
        } else {
          author::insert(new_name, selected_ISBN);
        }
      }
      if (edit_ISBN || edit_keyword) {
        book::fixed_char_60 original_keyword = book::Get_book(selected_ISBN).Keyword;
        author::erase(original_keyword.ToString(), selected_ISBN);
        if (edit_ISBN && edit_keyword) {
          author::insert(new_name, new_ISBN);
        } else if (edit_ISBN) {
          author::insert(original_keyword.ToString(), new_ISBN);
        } else {
          author::insert(new_name, selected_ISBN);
        }
      }
      book::Edit(selected_ISBN, edit_ISBN, edit_name, edit_author, edit_keyword, false, edit_price,
          new_ISBN, new_name, new_author, new_keyword, "", new_price);
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
      bool valid_Q = true;
      for (int i = 0; i < Quantity.length(); ++i) {
        if (Quantity[i] < '0' || Quantity[i] > '9') {
          valid_Q = false;
          break;
        }
      }
      if (!valid_Q) {
        std::cout << "Invalid\n";
        continue;
      }
      if (!line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      std::string TotalCost = line.nextToken();
      bool valid_T = true;
      for (int i = 0; i < TotalCost.length(); ++i) {
        if (!valid_T) {
          break;
        }
        if (i == TotalCost.length() - 3) {
          if (TotalCost[i] != '.') {
            valid_T = false;
          }
          continue;
        }
        if (TotalCost[i] < '0' || TotalCost[i] > '9') {
          valid_T = false;
          break;
        }
      }
      if (!valid_T) {
        std::cout << "Invalid\n";
        continue;
      }
      if (line.hasMoreTokens()) {
        std::cout << "Invalid\n";
        continue;
      }
      int quantity = book::fixed_char_10(Quantity).ToInt();
      book::fixed_char_10 Storage = book::Get_book(current_online.selected_book).Storage;
      quantity += Storage.ToInt();
      std::string Q = book::fixed_char_10(quantity).ToString();
      double total_cost = book::fixed_char_13(TotalCost).ToDouble();
      book::Edit(current_online.selected_book, false, false, false,
          false, true, false, "", "", "",
          "", Q, "");
      finance::insert(0, total_cost);
    } else if (token == "log") {
      if (logging_stack.size == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      int current_privilege = logging_stack.log_stack[logging_stack.size - 1].privilege;
      if (current_privilege < 7) {
        std::cout << "Invalid\n";
        continue;
      }

    } else if (token == "report") {
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

      } else if (token == "employee") {

      } else {
        std::cout << "Invalid\n";
        continue;
      }
    } else if (token.empty()) {
      continue;
    } else {
      std::cout << "Invalid\n";
      continue;
    }
  }
  return 0;
}