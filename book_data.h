#ifndef BOOK_DATA_H
#define BOOK_DATA_H

#include <fstream>

namespace book {

  const std::string book_file = "books.txt";
  constexpr int max_block = 500;
  constexpr int max_index = 400;

  struct fixed_char_20 {
    static constexpr int length = 20;
    char fcg[length];
    fixed_char_20();
    fixed_char_20(const std::string &);
    fixed_char_20 &operator=(const fixed_char_20 &);
    fixed_char_20 &operator=(const std::string &);
    fixed_char_20 &operator=(std::string);
    // fixed_char &operator=(const std::string &&);
    bool operator==(const fixed_char_20 &) const;
    bool operator!=(const fixed_char_20 &) const;
    bool operator>(const fixed_char_20 &) const;
    bool operator<(const fixed_char_20 &) const;
    bool operator>=(const fixed_char_20 &) const;
    bool operator<=(const fixed_char_20 &) const;
    friend std::istream &operator>>(std::istream &in, fixed_char_20 &fc) {
      for (int i = 0; i < length; ++i) {
        if (in) {
          in >> fc.fcg[i];
        } else {
          for (int j = i; j < length; ++j) {
            fc.fcg[j] = '\0';
          }
          break;
        }
      }
      return in;
    }
    friend std::ostream &operator<<(std::ostream &out, const fixed_char_20 &fc) {
      for (int i = 0; i < length; ++i) {
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
  struct fixed_char_60 {
    static constexpr int length = 60;
    char fcg[length];
    fixed_char_60();
    fixed_char_60(const std::string &);
    fixed_char_60 &operator=(const fixed_char_60 &);
    fixed_char_60 &operator=(const std::string &);
    fixed_char_60 &operator=(std::string);
    // fixed_char &operator=(const std::string &&);
    bool operator==(const fixed_char_60 &) const;
    bool operator!=(const fixed_char_60 &) const;
    bool operator>(const fixed_char_60 &) const;
    bool operator<(const fixed_char_60 &) const;
    bool operator>=(const fixed_char_60 &) const;
    bool operator<=(const fixed_char_60 &) const;
    friend std::istream &operator>>(std::istream &in, fixed_char_60 &fc) {
      for (int i = 0; i < length; ++i) {
        if (in) {
          in >> fc.fcg[i];
        } else {
          for (int j = i; j < length; ++j) {
            fc.fcg[j] = '\0';
          }
          break;
        }
      }
      return in;
    }
    friend std::ostream &operator<<(std::ostream &out, const fixed_char_60 &fc) {
      for (int i = 0; i < length; ++i) {
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
  struct fixed_char_10 {
    static constexpr int length = 10;
    char fcg[length];
    fixed_char_10();
    fixed_char_10(const std::string &);
    fixed_char_10(long);
    fixed_char_10 &operator=(const fixed_char_10 &);
    fixed_char_10 &operator=(const std::string &);
    fixed_char_10 &operator=(std::string);
    // fixed_char &operator=(const std::string &&);
    bool operator==(const fixed_char_10 &) const;
    bool operator!=(const fixed_char_10 &) const;
    bool operator>(const fixed_char_10 &) const;
    bool operator<(const fixed_char_10 &) const;
    bool operator>=(const fixed_char_10 &) const;
    bool operator<=(const fixed_char_10 &) const;
    friend std::istream &operator>>(std::istream &in, fixed_char_10 &fc) {
      for (int i = 0; i < length; ++i) {
        if (in) {
          in >> fc.fcg[i];
        } else {
          for (int j = i; j < length; ++j) {
            fc.fcg[j] = '\0';
          }
          break;
        }
      }
      return in;
    }
    friend std::ostream &operator<<(std::ostream &out, const fixed_char_10 &fc) {
      for (int i = 0; i < length; ++i) {
        if (fc.fcg[i] == '\0') {
          break;
        }
        out << fc.fcg[i];
      }
      return out;
    }
    char &operator[](int);
    const std::string ToString();
    const int ToInt();
  };
  struct fixed_char_13 {
    static constexpr int length = 13;
    char fcg[length];
    fixed_char_13();
    fixed_char_13(const std::string &);
    fixed_char_13(double);
    fixed_char_13 &operator=(const fixed_char_13 &);
    fixed_char_13 &operator=(const std::string &);
    fixed_char_13 &operator=(std::string);
    // fixed_char &operator=(const std::string &&);
    bool operator==(const fixed_char_13 &) const;
    bool operator!=(const fixed_char_13 &) const;
    bool operator>(const fixed_char_13 &) const;
    bool operator<(const fixed_char_13 &) const;
    bool operator>=(const fixed_char_13 &) const;
    bool operator<=(const fixed_char_13 &) const;
    friend std::istream &operator>>(std::istream &in, fixed_char_13 &fc) {
      for (int i = 0; i < length; ++i) {
        if (in) {
          in >> fc.fcg[i];
        } else {
          for (int j = i; j < length; ++j) {
            fc.fcg[j] = '\0';
          }
          break;
        }
      }
      return in;
    }
    friend std::ostream &operator<<(std::ostream &out, const fixed_char_13 &fc) {
      for (int i = 0; i < length; ++i) {
        if (fc.fcg[i] == '\0') {
          break;
        }
        out << fc.fcg[i];
      }
      return out;
    }
    char &operator[](int);
    double ToDouble() const;
  };

  struct ISBN_pos {
    fixed_char_20 ISBN;
    unsigned long long block_pos = 0;
    ISBN_pos &operator=(const ISBN_pos &other) {
      ISBN = other.ISBN;
      block_pos = other.block_pos;
      return *this;
    }
  };
  struct book_data {
    fixed_char_20 ISBN;
    fixed_char_60 BookName;
    fixed_char_60 Author;
    fixed_char_60 Keyword;
    fixed_char_10 Storage;
    fixed_char_13 Price;

    book_data &operator=(const book_data &other) {
      ISBN = other.ISBN;
      BookName = other.BookName;
      Author = other.Author;
      Keyword = other.Keyword;
      Storage = other.Storage;
      Price = other.Price;
      return *this;
    }
    bool operator>(const book_data &other) const {
      return ISBN > other.ISBN;
    }
    bool operator<(const book_data &other) const {
      return ISBN < other.ISBN;
    }
    bool operator==(const book_data &other) const {
      return ISBN == other.ISBN;
    }
    bool operator>=(const book_data &other) const {
      return ISBN >= other.ISBN;
    }
    bool operator<=(const book_data &other) const {
      return ISBN <= other.ISBN;
    }
    bool operator!=(const book_data &other) const {
      return ISBN != other.ISBN;
    }
  };

  struct Menu {
    int size = 0;
    ISBN_pos menu[max_block];

    [[nodiscard]] int FindLower(const std::string &) const; // return the last unit whose smallest ID is smaller than p (all >=:-1)
    [[nodiscard]] int FindUpper(const std::string &) const; // return the last unit whose smallest ID is smaller than or equal to p (all >:-1)
    void insert(const ISBN_pos &);
    void erase(const ISBN_pos &);
  };
  struct Block {
    int size = 0;
    book_data block[max_index + 10];

    // return the last unit whose smallest ID is smaller than p (all >=: -1)
    [[nodiscard]] int FindLower(const std::string &) const;

    // return the last unit whose smallest ID is smaller than or equal to p (all >: -1)
    [[nodiscard]] int FindUpper(const std::string &) const;

    void insert(const book_data &);

    void erase(const std::string &);
    void erase(const book_data &);
  };

  void CreatFileIfNotExist(const std::string &);

  // return the position of block where target ISBN can be found (not exist: 0)
  unsigned long long IfExist(const std::string &);

  void AddBook(const std::string &, const std::string &, const std::string &,
      const std::string &, int, double);
  void AddBook(const book_data &);
  void Delete(const std::string &);
  void Delete(const book_data &);

  void Edit(const std::string &ISBN, bool edit_ISBN, bool edit_name, bool edit_author,
    bool edit_keyword, bool edit_storage, bool edit_price, const std::string &new_ISBN,
    const std::string &new_name, const std::string &new_author, const std::string &new_keyword,
    const std::string &new_storage, const std::string &new_price);

  const book_data &Get_book(const std::string &);
}

#endif //BOOK_DATA_H
