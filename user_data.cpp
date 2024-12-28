#include <fstream>
#include <iostream>
#include "user_data.h"

user::fixed_char::fixed_char() {
  for (int i = 0; i < 30; ++i) {
    fcg[i] = '\0';
  }
}
user::fixed_char::fixed_char(const std::string &str) {
  int len = std::min(30, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < 30) {
    for (int i = len; i < 30; ++i) {
      fcg[i] = '\0';
    }
  }
}

user::fixed_char &user::fixed_char::operator=(const fixed_char &other) {
  if (this == &other) {
    return *this;
  }
  for (int i = 0; i < 30; ++i) {
    fcg[i] = other.fcg[i];
  }
  return *this;
}
user::fixed_char &user::fixed_char::operator=(const std::string &str) {
  int len = std::min(30, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < 30) {
    for (int i = len; i < 30; ++i) {
      fcg[i] = '\0';
    }
  }
  return *this;
}
user::fixed_char &user::fixed_char::operator=(std::string str) {
  int len = std::min(30, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < 30) {
    for (int i = len; i < 30; ++i) {
      fcg[i] = '\0';
    }
  }
  return *this;
}

/*user::fixed_char &user::fixed_char::operator=(const std::string &&str) {
  int len = std::min(30, (int)str.length());
  for (int i = 0; i < len; ++i) {
    fcg[i] = str[i];
  }
  if (len < 30) {
    for (int i = len; i < 30; ++i) {
      fcg[i] = '\0';
    }
  }
  return *this;
}*/

bool user::fixed_char::operator==(const fixed_char &other) const {
  for (int i = 0; i < 30; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return false;
    }
    if (fcg[i] == '\0' && other.fcg[i] == '\0') {
      break;
    }
  }
  return true;
}
bool user::fixed_char::operator!=(const fixed_char &other) const {
  return !(*this == other);
}
bool user::fixed_char::operator>(const fixed_char &other) const {
  for (int i = 0; i < 30; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return fcg[i] > other.fcg[i];
    }
  }
  return false;
}
bool user::fixed_char::operator<(const fixed_char &other) const {
  for (int i = 0; i < 30; ++i) {
    if (fcg[i] != other.fcg[i]) {
      return fcg[i] < other.fcg[i];
    }
  }
  return false;
}
bool user::fixed_char::operator>=(const fixed_char &other) const {
  return !(*this < other);
}
bool user::fixed_char::operator<=(const fixed_char &other) const {
  return !(*this > other);
}
char &user::fixed_char::operator[](const int i) {
  if (i < 0 || i >= 30) {
    throw std::out_of_range("Index out of range for fixed_char");
  }
  return fcg[i];
}

const std::string user::fixed_char::ToString() {
  std::string ans;
  for (int i = 0; i < 30; ++i) {
    ans += fcg[i];
    if (fcg[i] == '\0') {
      return ans;
    }
  }
  ans += '\0';
  return ans;
}

void user::CreatFileIfNotExist(const std::string &fn) {
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
unsigned long long user::IfExist(const std::string &ID) {
  fixed_char target_ID = ID;
  CreatFileIfNotExist(user_file);
  std::fstream file(user_file);
  Menu menu;
  file.seekg(0);
  file.read(reinterpret_cast<char *> (&menu), sizeof(menu));
  if (menu.size == 0) {
    file.close();
    return 0;
  }
  int m = menu.FindUpper(target_ID.ToString());
  if (m == -1) {
    file.close();
    return 0;
  }
  unsigned long long block_pos = menu.menu[m].block_pos;
  Block block;
  file.seekg(block_pos);
  file.read(reinterpret_cast<char *> (& block), sizeof(block));
  int target_ID_index = block.FindUpper(target_ID.ToString());
  if (target_ID_index == -1) {
    file.close();
    return 0;
  }
  if (block.block[target_ID_index].ID == target_ID) {
    file.close();
    return block_pos;
  }
  file.close();
  return 0;
}

int user::Menu::FindLower(const std::string &p) const {
  if (menu[0].ID >= p) {
    return -1;
  }
  if (menu[size - 1].ID < p) {
    return size - 1;
  }
  int l = 0;
  int h = size - 1;
  while (l + 1 < h) {
    int m = (l + h) / 2;
    if (menu[m].ID < p) {
      l = m;
    } else {
      h = m;
    }
  }
  return l;
}
int user::Menu::FindUpper(const std::string &p) const {
  if (menu[0].ID > p) {
    return -1;
  }
  if (!(menu[size - 1].ID > p)) {
    return size - 1;
  }
  int l = 0;
  int h = size - 1;
  while (l + 1 < h) {
    int m = (l + h) / 2;
    if (!(menu[m].ID > p)) {
      l = m;
    } else {
      h = m;
    }
  }
  return l;
}
void user::Menu::insert(const ID_pos &u) {
  if (size == 0) {
    size = 1;
    menu[0] = u;
  }
  if (u.ID == menu[0].ID || u.ID == menu[size - 1].ID) {
    return;
  }
  if (u.ID < menu[0].ID) {
    for (int t = size; t > 0; --t) {
      menu[t] = menu[t - 1];
    }
    menu[0] = u;
    ++size;
  } else if (u.ID > menu[size - 1].ID) {
    menu[size] = u;
    ++size;
  } else {
    int l = 0, h = size - 1;
    while (h - l > 1) {
      int m = (l + h) / 2;
      if (menu[m].ID > u.ID) {
        h = m;
      } else if (menu[m].ID == u.ID) {
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
void user::Menu::erase(const ID_pos &u) {
  if (size == 0 || menu[0].ID > u.ID || menu[size - 1].ID < u.ID) {
    return;
  }
  if (menu[0].ID == u.ID) {
    for (int t = 0; t < size - 1; ++t) {
      menu[t] = menu[t + 1];
    }
    --size;
    return;
  }
  if (menu[size - 1].ID == u.ID) {
    --size;
    return;
  }
  int l = 0, h = size - 1;
  while(h > l + 1) {
    int m = (l + h) / 2;
    if (menu[m].ID == u.ID) {
      for (int t = m; t < size - 1; ++t) {
        menu[t] = menu[t + 1];
      }
      --size;
      return;
    }
    if (menu[m].ID > u.ID) {
      h = m;
    } else if (menu[m].ID < u.ID) {
      l = m;
    }
  }
}

int user::Block::FindLower(const std::string &p) const {
  if (block[0].ID >= p) {
    return -1;
  }
  if (block[size - 1].ID < p) {
    return size - 1;
  }
  int l = 0;
  int h = size - 1;
  while (l + 1 < h) {
    int m = (l + h) / 2;
    if (block[m].ID < p) {
      l = m;
    } else {
      h = m;
    }
  }
  return l;
}
int user::Block::FindUpper(const std::string &p) const {
  if (block[0].ID > p) {
    return -1;
  }
  if (!(block[size - 1].ID > p)) {
    return size - 1;
  }
  int l = 0;
  int h = size - 1;
  while (l + 1 < h) {
    int m = (l + h) / 2;
    if (!(block[m].ID > p)) {
      l = m;
    } else {
      h = m;
    }
  }
  return l;
}
void user::Block::insert(const user_data &u) {
  const std::string ID = u.ID.fcg;
  if (ID == block[0].ID.ToString() || ID == block[size - 1].ID.ToString()) {
    return;
  }
  if (ID > block[size - 1].ID.ToString()) {
    block[size] = u;
    ++size;
  } else {
    int l = 0, h = size - 1;
    while (h - l > 1) {
      int m = (l + h) / 2;
      if (block[m].ID > ID) {
        h = m;
      } else if (block[m].ID == ID) {
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
void user::Block::erase(const std::string &ID) {
  if (ID > block[size - 1].ID.ToString() || ID < block[0].ID.ToString()) {
    return;
  }
  if (ID == block[size - 1].ID.ToString()) {
    --size;
    return;
  }
  if (ID == block[0].ID.ToString()) {
    for (int t = 0; t < size - 1; ++t) {
      block[t] = block[t + 1];
    }
    --size;
    return;
  }
  int l = 0, h = size - 1;
  while (h > l + 1) {
    int m = (l + h) / 2;
    if (block[m].ID == ID) {
      for (int t = m; t < size - 1; ++t) {
        block[t] = block[t + 1];
      }
      --size;
      return;
    }
    if (block[m].ID > ID) {
      h = m;
    } else if (block[m].ID < ID) {
      l = m;
    }
  }
}
void user::Block::erase(const user_data &u) {
  const std::string id = u.ID.fcg;
  erase(id);
}


void user::AddUser(const user_data &u) {
  const std::string id = u.ID.fcg;
  unsigned long long bp = IfExist(id);
  if (bp != 0) {
    std::cout << "Invalid\n";
    return;
  }
  std::fstream file(user_file);
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
    menu.menu[0].ID = u.ID;
    menu.menu[0].block_pos = block_position;

    file.seekp(0);
    file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
    file.close();
    return;
  }

  // if the menu is not empty
  int insert_block = menu.FindLower(id); // try to find out where should we insert p

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
    ID_pos new_unit_menu;
    new_unit_menu.ID = u.ID;
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
    ID_pos new_unit_menu;
    file.seekp(0, std::ios::end);
    long new_u_pos = file.tellp();
    file.write(reinterpret_cast<char *> (& new_bl), sizeof(new_bl));
    new_unit_menu.block_pos = new_u_pos;
    new_unit_menu.ID = new_bl.block[0].ID;
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
void user::AddUser(const std::string &ID, const std::string &Name, const std::string &Password, int privilege) {
  const user_data u = {ID, Password, Name, privilege};
  AddUser(u);
}
void user::Delete(const std::string &ID) {
  unsigned long long bp = IfExist(ID);
  if (bp == 0) { // can't find the target
    std::cout << "Invalid\n";
    return;
  }

  // there is the target in the block whose position is bp
  std::fstream file(user_file);

  Menu menu;
  file.seekg(0);
  file.read(reinterpret_cast<char *>(& menu), sizeof(menu));
  int erase_block = menu.FindUpper(ID);

  Block block;
  file.seekg(bp);
  file.read(reinterpret_cast<char *>(& block), sizeof(block)); // read the block
  if (block.block[0].ID == ID) { // if we remove ID, the smallest will change
    if (block.size == 1) { // empty after removal, simply ignore the whole block
      menu.erase(menu.menu[erase_block]);
      file.seekp(0);
      file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
      file.close();
      return;
    }
    // revise the unit_menu and remove p
    std::string second = block.block[1].ID.ToString(); // it'll replace current smallest
    menu.menu[erase_block].ID = fixed_char(second);
    block.erase(ID);
  } else {
    block.erase(ID);
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
void user::Delete(const user_data &u) {
  const std::string id = u.ID.fcg;
  Delete(id);
}
int user::CheckPassword(const std::string &ID, const std::string &password) {
  unsigned long long bp = IfExist(ID);
  if (bp == 0) {
    std::cout << "Invalid\n";
    return -1;
  }
  std::fstream file(user_file);
  Block block;
  file.seekg(bp);
  file.read(reinterpret_cast<char *>(& block), sizeof(block));
  int index = block.FindUpper(ID);
  file.close();
  if (block.block[index].Password == password) {
    return 1;
  }
  return 0;
}
void user::EditPassword(const std::string &ID, const std::string &NewPassword) {
  unsigned long long bp = IfExist(ID);
  if (bp == 0) {
    std::cout << "Invalid\n";
    return;
  }
  std::fstream file(user_file);
  Block block;
  file.seekg(bp);
  file.read(reinterpret_cast<char *>(& block), sizeof(block));
  int index = block.FindUpper(ID);
  block.block[index].Password = fixed_char(NewPassword);
  file.seekp(bp);
  file.write(reinterpret_cast<char *>(& block), sizeof(block));
  file.close();
}