#include <fstream>
#include <iostream>
#include "book_data.h"

// -- 20
book::fixed_char_20::fixed_char_20() {
  for (int i = 0; i < length; ++i) {
    fcg[i] = '\0';
  }
}
book::fixed_char_20::fixed_char_20(const std::string &str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
}

book::fixed_char_20 &book::fixed_char_20::operator=(const fixed_char_20 &other) {
  if (this == &other) {
    return *this;
  }
  for (int i = 0; i < length; ++i) {
    fcg[i] = other.fcg[i];
  }
  return *this;
}
book::fixed_char_20 &book::fixed_char_20::operator=(const std::string &str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
  return *this;
}
book::fixed_char_20 &book::fixed_char_20::operator=(std::string str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
  return *this;
}

bool book::fixed_char_20::operator==(const fixed_char_20 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return false;
    }
    if (fcg[i] == '\0' && other.fcg[i] == '\0') {
      break;
    }
  }
  return true;
}
bool book::fixed_char_20::operator!=(const fixed_char_20 &other) const {
  return !(*this == other);
}
bool book::fixed_char_20::operator>(const fixed_char_20 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return fcg[i] > other.fcg[i];
    }
  }
  return false;
}
bool book::fixed_char_20::operator<(const fixed_char_20 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return fcg[i] < other.fcg[i];
    }
  }
  return false;
}
bool book::fixed_char_20::operator>=(const fixed_char_20 &other) const {
  return !(*this < other);
}
bool book::fixed_char_20::operator<=(const fixed_char_20 &other) const {
  return !(*this > other);
}
char &book::fixed_char_20::operator[](const int i) {
  if (i < 0 || i >= length) {
    throw std::out_of_range("Index out of range for fixed_char");
  }
  return fcg[i];
}

const std::string book::fixed_char_20::ToString() {
  std::string ans;
  for (int i = 0; i < length; ++i) {
    ans += fcg[i];
    if (fcg[i] == '\0') {
      return ans;
    }
  }
  ans += '\0';
  return ans;
}
// -- 60
book::fixed_char_60::fixed_char_60() {
  for (int i = 0; i < length; ++i) {
    fcg[i] = '\0';
  }
}
book::fixed_char_60::fixed_char_60(const std::string &str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
}

book::fixed_char_60 &book::fixed_char_60::operator=(const fixed_char_60 &other) {
  if (this == &other) {
    return *this;
  }
  for (int i = 0; i < length; ++i) {
    fcg[i] = other.fcg[i];
  }
  return *this;
}
book::fixed_char_60 &book::fixed_char_60::operator=(const std::string &str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
  return *this;
}
book::fixed_char_60 &book::fixed_char_60::operator=(std::string str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
  return *this;
}

bool book::fixed_char_60::operator==(const fixed_char_60 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return false;
    }
    if (fcg[i] == '\0' && other.fcg[i] == '\0') {
      break;
    }
  }
  return true;
}
bool book::fixed_char_60::operator!=(const fixed_char_60 &other) const {
  return !(*this == other);
}
bool book::fixed_char_60::operator>(const fixed_char_60 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return fcg[i] > other.fcg[i];
    }
  }
  return false;
}
bool book::fixed_char_60::operator<(const fixed_char_60 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return fcg[i] < other.fcg[i];
    }
  }
  return false;
}
bool book::fixed_char_60::operator>=(const fixed_char_60 &other) const {
  return !(*this < other);
}
bool book::fixed_char_60::operator<=(const fixed_char_60 &other) const {
  return !(*this > other);
}
char &book::fixed_char_60::operator[](const int i) {
  if (i < 0 || i >= length) {
    throw std::out_of_range("Index out of range for fixed_char");
  }
  return fcg[i];
}

const std::string book::fixed_char_60::ToString() {
  std::string ans;
  for (int i = 0; i < length; ++i) {
    ans += fcg[i];
    if (fcg[i] == '\0') {
      return ans;
    }
  }
  ans += '\0';
  return ans;
}
// -- 10
book::fixed_char_10::fixed_char_10() {
  for (int i = 0; i < length; ++i) {
    fcg[i] = '\0';
  }
}
book::fixed_char_10::fixed_char_10(const std::string &str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
}
book::fixed_char_10::fixed_char_10(long a) {
  if (a < 0 || a > 2147483647) {
    std::cout << "Invalid\n";
    return;
  }
  int i = 0;
  while ((a && i < 10) || (a == 0 && i == 0)) {
    fcg[i] = '0' + a % 10;
    a /= 10;
    ++i;
  }
  if (i < 10) {
    for (; i < 10; ++i) {
      fcg[i] = '\0';
    }
  } else if (a) {
    std::cout << "Invalid\n";
    return;
  }
}

book::fixed_char_10 &book::fixed_char_10::operator=(const fixed_char_10 &other) {
  if (this == &other) {
    return *this;
  }
  for (int i = 0; i < length; ++i) {
    fcg[i] = other.fcg[i];
  }
  return *this;
}
book::fixed_char_10 &book::fixed_char_10::operator=(const std::string &str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
  return *this;
}
book::fixed_char_10 &book::fixed_char_10::operator=(std::string str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
  return *this;
}

bool book::fixed_char_10::operator==(const fixed_char_10 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return false;
    }
    if (fcg[i] == '\0' && other.fcg[i] == '\0') {
      break;
    }
  }
  return true;
}
bool book::fixed_char_10::operator!=(const fixed_char_10 &other) const {
  return !(*this == other);
}
bool book::fixed_char_10::operator>(const fixed_char_10 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return fcg[i] > other.fcg[i];
    }
  }
  return false;
}
bool book::fixed_char_10::operator<(const fixed_char_10 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return fcg[i] < other.fcg[i];
    }
  }
  return false;
}
bool book::fixed_char_10::operator>=(const fixed_char_10 &other) const {
  return !(*this < other);
}
bool book::fixed_char_10::operator<=(const fixed_char_10 &other) const {
  return !(*this > other);
}
char &book::fixed_char_10::operator[](const int i) {
  if (i < 0 || i >= length) {
    throw std::out_of_range("Index out of range for fixed_char");
  }
  return fcg[i];
}

const std::string book::fixed_char_10::ToString() {
  std::string ans;
  for (int i = 0; i < length; ++i) {
    ans += fcg[i];
    if (fcg[i] == '\0') {
      return ans;
    }
  }
  ans += '\0';
  return ans;
}
const int book::fixed_char_10::ToInt() {
  int ans = 0;
  for (int i = 9; i >= 0; --i) {
    if (fcg[i] == '\0') {
      continue;
    }
    ans = ans * 10 + fcg[i] - '0';
  }
  return ans;
}
// -- 13
book::fixed_char_13::fixed_char_13() {
  for (int i = 0; i < length; ++i) {
    fcg[i] = '\0';
  }
}
book::fixed_char_13::fixed_char_13(const std::string &str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
}
book::fixed_char_13::fixed_char_13(double a) {
  if (a < 0) {
    std::cout << "Invalid\n";
    return;
  }
  int ap = (int)a; // int part
  double flt = a - ap;
  flt *= 100;
  int fl = (int)flt; // the double part
  int n_int = 9; // start from the position in front of '.'
  while ((ap && n_int >= 0) || (ap == 0 && n_int == 9)) {
    fcg[n_int] = '0' + ap % 10;
    ap /= 10;
    --n_int;
  }
  if (n_int >= 0) {
    for (; n_int >= 0; --n_int) {
      fcg[n_int] = '\0';
    }
  }
  fcg[10] = '.';
  fcg[12] = '0' + fl % 10;
  fcg[11] = '0' + fl / 10;
}

book::fixed_char_13 &book::fixed_char_13::operator=(const fixed_char_13 &other) {
  if (this == &other) {
    return *this;
  }
  for (int i = 0; i < length; ++i) {
    fcg[i] = other.fcg[i];
  }
  return *this;
}
book::fixed_char_13 &book::fixed_char_13::operator=(const std::string &str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
  return *this;
}
book::fixed_char_13 &book::fixed_char_13::operator=(std::string str) {
  int len = std::min(length, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < length) {
    for (int i = len; i < length; ++i) {
      fcg[i] = '\0';
    }
  }
  return *this;
}

bool book::fixed_char_13::operator==(const fixed_char_13 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return false;
    }
    if (fcg[i] == '\0' && other.fcg[i] == '\0') {
      break;
    }
  }
  return true;
}
bool book::fixed_char_13::operator!=(const fixed_char_13 &other) const {
  return !(*this == other);
}
bool book::fixed_char_13::operator>(const fixed_char_13 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return fcg[i] > other.fcg[i];
    }
  }
  return false;
}
bool book::fixed_char_13::operator<(const fixed_char_13 &other) const {
  for (int i = 0; i < length; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return fcg[i] < other.fcg[i];
    }
  }
  return false;
}
bool book::fixed_char_13::operator>=(const fixed_char_13 &other) const {
  return !(*this < other);
}
bool book::fixed_char_13::operator<=(const fixed_char_13 &other) const {
  return !(*this > other);
}
char &book::fixed_char_13::operator[](const int i) {
  if (i < 0 || i >= length) {
    throw std::out_of_range("Index out of range for fixed_char");
  }
  return fcg[i];
}
double book::fixed_char_13::ToDouble() const {
  double ans = 0;
  for (int i = 0; i < 10; ++i) {
    if (fcg[i] == '\0') {
      continue;
    }
    ans = ans * 10 + (fcg[i] - '0');
  }
  ans += (fcg[11] - '0') * 0.1 + (fcg[12] - '0') * 0.01;
  return ans;
}

// -- end

void book::CreatFileIfNotExist(const std::string &fn) {
  bool exist = false;
  std::fstream file(fn);
  if (file.is_open()) {
    exist = true;
  }
  file.close();
  if (!exist) {
    std::ofstream new_file(fn);
    Menu empty_menu;
    new_file.seekp(0);
    new_file.write(reinterpret_cast<char *>(& empty_menu), sizeof(empty_menu));
    new_file.close();
  }
}
unsigned long long book::IfExist(const std::string &ISBN) {
  fixed_char_20 target_ISBN = ISBN;
  CreatFileIfNotExist(book_file);
  std::fstream file(book_file);
  Menu menu;
  file.seekg(0);
  file.read(reinterpret_cast<char *> (&menu), sizeof(menu));
  if (menu.size == 0) {
    file.close();
    return 0;
  }
  int m = menu.FindUpper(target_ISBN.ToString());
  if (m == -1) {
    file.close();
    return 0;
  }
  unsigned long long block_pos = menu.menu[m].block_pos;
  Block block;
  file.seekg(block_pos);
  file.read(reinterpret_cast<char *> (& block), sizeof(block));
  int target_ID_index = block.FindUpper(target_ISBN.ToString());
  if (target_ID_index == -1) {
    file.close();
    return 0;
  }
  if (block.block[target_ID_index].ISBN == target_ISBN) {
    file.close();
    return block_pos;
  }
  file.close();
  return 0;
}

int book::Menu::FindLower(const std::string &p) const {
  if (menu[0].ISBN >= p) {
    return -1;
  }
  if (menu[size - 1].ISBN < p) {
    return size - 1;
  }
  int l = 0;
  int h = size - 1;
  while (l + 1 < h) {
    int m = (l + h) / 2;
    if (menu[m].ISBN < p) {
      l = m;
    } else {
      h = m;
    }
  }
  return l;
}
int book::Menu::FindUpper(const std::string &p) const {
  if (menu[0].ISBN > p) {
    return -1;
  }
  if (!(menu[size - 1].ISBN > p)) {
    return size - 1;
  }
  int l = 0;
  int h = size - 1;
  while (l + 1 < h) {
    int m = (l + h) / 2;
    if (!(menu[m].ISBN > p)) {
      l = m;
    } else {
      h = m;
    }
  }
  return l;
}
void book::Menu::insert(const ISBN_pos &u) {
  if (size == 0) {
    size = 1;
    menu[0] = u;
  }
  if (u.ISBN == menu[0].ISBN || u.ISBN == menu[size - 1].ISBN) {
    return;
  }
  if (u.ISBN < menu[0].ISBN) {
    for (int t = size; t > 0; --t) {
      menu[t] = menu[t - 1];
    }
    menu[0] = u;
    ++size;
  } else if (u.ISBN > menu[size - 1].ISBN) {
    menu[size] = u;
    ++size;
  } else {
    int l = 0, h = size - 1;
    while (h - l > 1) {
      int m = (l + h) / 2;
      if (menu[m].ISBN > u.ISBN) {
        h = m;
      } else if (menu[m].ISBN == u.ISBN) {
        return;
      } else {
        l = m;
      }
    }
    for (int t = size; t > h; --t) {
      menu[t] = menu[t - 1];
    }
    menu[h] = u;
    ++size;
  }
}
void book::Menu::erase(const ISBN_pos &u) {
  if (size == 0 || menu[0].ISBN > u.ISBN || menu[size - 1].ISBN < u.ISBN) {
    return;
  }
  if (menu[0].ISBN == u.ISBN) {
    for (int t = 0; t < size - 1; ++t) {
      menu[t] = menu[t + 1];
    }
    --size;
    return;
  }
  if (menu[size - 1].ISBN == u.ISBN) {
    --size;
    return;
  }
  int l = 0, h = size - 1;
  while(h > l + 1) {
    int m = (l + h) / 2;
    if (menu[m].ISBN == u.ISBN) {
      for (int t = m; t < size - 1; ++t) {
        menu[t] = menu[t + 1];
      }
      --size;
      return;
    }
    if (menu[m].ISBN > u.ISBN) {
      h = m;
    } else if (menu[m].ISBN < u.ISBN) {
      l = m;
    }
  }
}

int book::Block::FindLower(const std::string &p) const {
  if (block[0].ISBN >= p) {
    return -1;
  }
  if (block[size - 1].ISBN < p) {
    return size - 1;
  }
  int l = 0;
  int h = size - 1;
  while (l + 1 < h) {
    int m = (l + h) / 2;
    if (block[m].ISBN < p) {
      l = m;
    } else {
      h = m;
    }
  }
  return l;
}
int book::Block::FindUpper(const std::string &p) const {
  if (block[0].ISBN > p) {
    return -1;
  }
  if (!(block[size - 1].ISBN > p)) {
    return size - 1;
  }
  int l = 0;
  int h = size - 1;
  while (l + 1 < h) {
    int m = (l + h) / 2;
    if (!(block[m].ISBN > p)) {
      l = m;
    } else {
      h = m;
    }
  }
  return l;
}
void book::Block::insert(const book_data &u) {
  const std::string ID = u.ISBN.fcg;
  if (ID == block[0].ISBN.ToString() || ID == block[size - 1].ISBN.ToString()) {
    return;
  }
  if (ID > block[size - 1].ISBN.ToString()) {
    block[size] = u;
    ++size;
  } else {
    int l = 0, h = size - 1;
    while (h - l > 1) {
      int m = (l + h) / 2;
      if (block[m].ISBN > ID) {
        h = m;
      } else if (block[m].ISBN == ID) {
        return;
      } else {
        l = m;
      }
    }
    for (int t = size; t > h; --t) {
      block[t] = block[t - 1];
    }
    block[h] = u;
    ++size;
  }
}
void book::Block::erase(const std::string &ISBN) {
  if (ISBN > block[size - 1].ISBN.ToString() || ISBN < block[0].ISBN.ToString()) {
    return;
  }
  if (ISBN == block[size - 1].ISBN.ToString()) {
    --size;
    return;
  }
  if (ISBN == block[0].ISBN.ToString()) {
    for (int t = 0; t < size - 1; ++t) {
      block[t] = block[t + 1];
    }
    --size;
    return;
  }
  int l = 0, h = size - 1;
  while (h > l + 1) {
    int m = (l + h) / 2;
    if (block[m].ISBN == ISBN) {
      for (int t = m; t < size - 1; ++t) {
        block[t] = block[t + 1];
      }
      --size;
      return;
    }
    if (block[m].ISBN > ISBN) {
      h = m;
    } else if (block[m].ISBN < ISBN) {
      l = m;
    }
  }
}
void book::Block::erase(const book_data &u) {
  const std::string id = u.ISBN.fcg;
  erase(id);
}


void book::AddBook(const book_data &u) {
  const std::string isbn = u.ISBN.fcg;
  unsigned long long bp = IfExist(isbn);
  if (bp != 0) {
    std::cout << "Invalid\n";
    return;
  }
  std::fstream file(book_file);
  Menu menu;
  file.seekg(0);
  file.read(reinterpret_cast<char *>(& menu), sizeof(menu)); // read the menu
  if (menu.size == 0) { // if the menu is empty

    Block bl;
    bl.size = 1;
    bl.block[0] = u;

    file.seekp(0, std::ios::end);
    long block_position = file.tellp();
    file.write(reinterpret_cast<char *>(& bl), sizeof(bl));

    // revise the first one
    ++menu.size;
    menu.menu[0].ISBN = u.ISBN;
    menu.menu[0].block_pos = block_position;

    file.seekp(0);
    file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
    file.close();
    return;
  }

  // if the menu is not empty
  int insert_block = menu.FindLower(isbn); // try to find out where should we insert p

  if (insert_block == -1) { // can't put p in any existed block

    // now we have to create a new block, a new unit_menu and put p into them
    Block new_block;
    new_block.size = 1;
    new_block.block[0] = u;

    // write down the block and record its position
    file.seekp(0, std::ios::end);
    long block_position = file.tellp();
    file.write(reinterpret_cast<char *>(& new_block), sizeof(new_block));

    // creat a related unit_menu
    ISBN_pos new_unit_menu;
    new_unit_menu.ISBN = u.ISBN;
    new_unit_menu.block_pos = block_position;

    // then insert it into the whole menu
    menu.insert(new_unit_menu);
    file.seekp(0);
    file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
    file.close();
    return;;
  }

  // we should place p in the insert_block
  Block bl;
  unsigned long long block_position = menu.menu[insert_block].block_pos;
  file.seekg(block_position);
  file.read(reinterpret_cast<char *>(& bl), sizeof(bl)); // read out the block
  bl.insert(u);

  int bls = bl.size;
  if (bls > max_index) {
    // we need to split the block first
    Block new_bl;
    new_bl.size = bls - bls / 2;
    for (int t = 0; t < bls - bls / 2; ++t) {
      new_bl.block[t] = bl.block[t + bls / 2];
    }
    bl.size = bls / 2;
    // ok, now generate a new unit_menu
    ISBN_pos new_unit_menu;
    file.seekp(0, std::ios::end);
    long new_u_pos = file.tellp();
    file.write(reinterpret_cast<char *> (& new_bl), sizeof(new_bl));
    new_unit_menu.block_pos = new_u_pos;
    new_unit_menu.ISBN = new_bl.block[0].ISBN;
    // now insert back
    menu.insert(new_unit_menu);

    file.seekp(0);
    file.write(reinterpret_cast<char *> (& menu), sizeof(menu));
    file.seekp(block_position);
    file.write(reinterpret_cast<char *> (& bl), sizeof(bl));
  } else {
    file.seekp(block_position);
    file.write(reinterpret_cast<char *>(& bl), sizeof(bl));
  } // just write the block back
  file.close();
}
void book::AddBook(const std::string &ISBN, const std::string &BookName,
    const std::string &Author, const std::string &KeyWord, int Storage, double Price) {
  const book_data b = {ISBN, BookName, Author, KeyWord, Storage, Price};
  AddBook(b);
}
void book::Delete(const std::string &ISBN) {
  unsigned long long bp = IfExist(ISBN);
  if (bp == 0) { // can't find the target
    std::cout << "Invalid\n";
    return;
  }

  // there is the target in the block whose position is bp
  std::fstream file(book_file);

  Menu menu;
  file.seekg(0);
  file.read(reinterpret_cast<char *>(& menu), sizeof(menu));
  int erase_block = menu.FindUpper(ISBN);

  Block block;
  file.seekg(bp);
  file.read(reinterpret_cast<char *>(& block), sizeof(block)); // read the block
  if (block.block[0].ISBN == ISBN) { // if we remove ID, the smallest will change
    if (block.size == 1) { // empty after removal, simply ignore the whole block
      menu.erase(menu.menu[erase_block]);
      file.seekp(0);
      file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
      file.close();
      return;
    }
    // revise the unit_menu and remove p
    std::string second = block.block[1].ISBN.ToString(); // it'll replace current smallest
    menu.menu[erase_block].ISBN = fixed_char_20(second);
    block.erase(ISBN);
  } else {
    block.erase(ISBN);
  }
  // check whether we can merge this block with the one in front
  if (erase_block > 0) {
    Block front;
    unsigned long long front_pos = menu.menu[erase_block - 1].block_pos;
    file.seekg(front_pos);
    file.read(reinterpret_cast<char *>(& front), sizeof(front));
    int bls = block.size;
    int frs = front.size;
    if (bls + frs <= max_index) { // we can merge bl into front, undoubtedly we should delete the unit_menu of bl
      // merge
      for (int t = 0; t < bls; ++t) {
        front.block[frs + t] = block.block[t];
      }
      front.size = bls + frs;
      menu.erase(menu.menu[erase_block]);
      file.seekp(0);
      file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
      file.seekp(front_pos);
      file.write(reinterpret_cast<char *>(& front), sizeof(front));
      file.close();
      return;
    }
  }
  file.seekp(0);
  file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
  file.seekp(bp);
  file.write(reinterpret_cast<char *>(& block), sizeof(block));
}
void book::Delete(const book_data &u) {
  const std::string isbn = u.ISBN.fcg;
  Delete(isbn);
}
void book::Edit(const std::string &ISBN, bool edit_ISBN, bool edit_name, bool edit_author,
    bool edit_keyword, bool edit_storage, bool edit_price, const std::string &new_ISBN, const std::string &new_name,
    const std::string &new_author, const std::string &new_keyword, const std::string &new_storage, const std::string &new_price) {

  unsigned long long bp = IfExist(ISBN);
  if (bp == 0) {
    std::cout << "Invalid\n";
    return;
  }

  // target book was found
  std::fstream file(book_file);
  file.seekg(bp);
  Block block;
  file.read(reinterpret_cast<char *> (& block), sizeof(block)); // get block
  int index = block.FindUpper(ISBN);
  if (edit_ISBN) {
    if (block.block[index].ISBN == new_ISBN) {
      std::cout << "Invalid\n";
      file.close();
      return;
    }
    block.block[index].ISBN = fixed_char_20(new_ISBN);
  }
  if (edit_name) {
    block.block[index].BookName = fixed_char_60(new_name);
  }
  if (edit_author) {
    block.block[index].Author = fixed_char_60(new_author);
  }
  if (edit_keyword) {
    block.block[index].Keyword = fixed_char_60(new_keyword);
  }
  if (edit_storage) {
    block.block[index].Storage = fixed_char_10(new_storage);
  }
  if (edit_price) {
    block.block[index].Price = fixed_char_13(new_price);
  }
  file.seekp(bp);
  file.write(reinterpret_cast<char *>(& block), sizeof(block));
  file.close();
}

const book::book_data &book::Get_book(const std::string &ISBN) {
  unsigned long long bp = IfExist(ISBN);
  Block block;
  book_data target;
  if (bp == 0) {
    std::cout << "Invalid\n";
    throw;
  }
  std::fstream file(book_file);
  file.seekg(bp);
  file.read(reinterpret_cast<char *>(& block), sizeof(block));
  int index = block.FindUpper(ISBN);
  target = block.block[index];
  file.close();
  return target;
}
