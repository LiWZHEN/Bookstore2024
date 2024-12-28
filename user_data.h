#ifndef USER_DATA_H
#define USER_DATA_H

#include <fstream>

namespace user {

  const std::string user_file = "users.txt";
  constexpr int max_block = 500;
  constexpr int max_index = 400;

  struct fixed_char {
    char fcg[30];
    fixed_char();
    fixed_char(const std::string &);
    fixed_char &operator=(const fixed_char &);
    fixed_char &operator=(const std::string &);
    fixed_char &operator=(std::string);
    // fixed_char &operator=(const std::string &&);
    bool operator==(const fixed_char &) const;
    bool operator!=(const fixed_char &) const;
    bool operator>(const fixed_char &) const;
    bool operator<(const fixed_char &) const;
    bool operator>=(const fixed_char &) const;
    bool operator<=(const fixed_char &) const;
    friend std::istream &operator>>(std::istream &in, fixed_char &fc) {
      for (int i = 0; i < 30; ++i) {
        if (in) {
          in >> fc.fcg[i];
        } else {
          for (int j = i; j < 30; ++j) {
            fc.fcg[j] = '\0';
          }
          break;
        }
      }
      return in;
    }
    friend std::ostream &operator<<(std::ostream &out, const fixed_char &fc) {
      for (int i = 0; i < 30; ++i) {
        if (fc.fcg[i] == '\0') {
          break;
        }
        out << fc.fcg[i];
      }
      return out;
    }
    char &operator[](int);
    const std::string ToString();
  };

  struct ID_pos {
    fixed_char ID;
    unsigned long long block_pos = 0;
    ID_pos &operator=(const ID_pos &other) {
      ID = other.ID;
      block_pos = other.block_pos;
      return *this;
    }
  };
  struct user_data {
    fixed_char ID;
    fixed_char Password;
    fixed_char Name;
    int privilege = 0;
    bool logged = false;
    user_data &operator=(const user_data &other) {
      ID = other.ID;
      Password = other.Password;
      Name = other.Name;
      privilege = other.privilege;
      logged = other.logged;
      return *this;
    }
    bool operator>(const user_data &other) const {
      return ID > other.ID;
    }
    bool operator<(const user_data &other) const {
      return ID < other.ID;
    }
    bool operator==(const user_data &other) const {
      return ID == other.ID;
    }
    bool operator>=(const user_data &other) const {
      return ID >= other.ID;
    }
    bool operator<=(const user_data &other) const {
      return ID <= other.ID;
    }
    bool operator!=(const user_data &other) const {
      return ID != other.ID;
    }
  };

  struct Menu {
    int size = 0;
    ID_pos menu[max_block];

    [[nodiscard]] int FindLower(const std::string &) const; // return the last unit whose smallest ID is smaller than p (all >=:-1)
    [[nodiscard]] int FindUpper(const std::string &) const; // return the last unit whose smallest ID is smaller than or equal to p (all >:-1)
    void insert(const ID_pos &);
    void erase(const ID_pos &);
  };
  struct Block {
    int size = 0;
    user_data block[max_index + 10];

    [[nodiscard]] int FindLower(const std::string &) const; // return the last unit whose smallest ID is smaller than p (all >=: -1)
    [[nodiscard]] int FindUpper(const std::string &) const; // return the last unit whose smallest ID is smaller than or equal to p (all >: -1)
    void insert(const user_data &);
    void erase(const std::string &);
    void erase(const user_data &);
  };

  void CreatFileIfNotExist(const std::string &);

  unsigned long long IfExist(const std::string &); // return the position of block where target ID can be found (not exist: 0)
  void AddUser(const std::string &, const std::string &, const std::string &, int, bool);
  void AddUser(const user_data &);
  void EditPassword(const std::string &, const std::string &);
  void Delete(const std::string &);
  void Delete(const user_data &);

  int CheckPassword(const std::string &, const std::string &); // no target ID: -1; wrong: 0; right: 1
  bool IfOnline(const std::string &);
}

#endif //USER_DATA_H
