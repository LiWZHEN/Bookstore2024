#include "book_name_processor.h"
namespace book_name {
  hash_pair db_hash(const std::string &str) {
    unsigned long hash1 = 0;
    unsigned long hash2 = 0;
    for (char c : str) {
      if (c == '\0') {
        break;
      }
      hash1 = (hash1 * P + c) % M;
      hash2 = (hash2 * Q + c) % M;
    }
    return {hash1, hash2};
  }
  void block::insert(const index_value &p) {
    if (p == pairs[0] || p == pairs[size - 1]) {
      return;
    }
    if (p > pairs[size - 1]) {
      if (size < MAX_BLOCK_SIZE) {
        pairs[size] = p;
        ++size;
        return;
      }
      pairs[size]=p;
      ++size;
    } else {
      int l = 0, h = size - 1;
      while (h - l > 1) {
        int m = (l + h) / 2;
        if (pairs[m] > p) {
          h = m;
        } else if (pairs[m] == p) {
          return;
        } else {
          l = m;
        }
      }
      for (int t = size; t > h; --t) {
        pairs[t] = pairs[t - 1];
      }
      pairs[h] = p;
      ++size;
    }
  }
  void block::erase(const index_value &p) {
    if (p > pairs[size - 1] || p < pairs[0]) {
      return;
    }
    if (p == pairs[size - 1]) {
      --size;
      return;
    }
    if (p == pairs[0]) {
      for (int t = 0; t < size - 1; ++t) {
        pairs[t] = pairs[t + 1];
      }
      --size;
      return;
    }
    int l = 0, h = size - 1;
    while (h > l + 1) {
      int m = (l + h) / 2;
      if (pairs[m] == p) {
        for (int t = m; t < size - 1; ++t) {
          pairs[t] = pairs[t + 1];
        }
        --size;
        return;
      }
      if (pairs[m] > p) {
        h = m;
      } else if (pairs[m] < p) {
        l = m;
      }
    }
  }
  [[nodiscard]] int Menu::FindLower(const index_value &p) const {
    if (!(menu[0].smallest < p)) {
      return -1;
    }
    if (menu[size - 1].smallest < p) {
      return size - 1;
    }
    int l = 0;
    int h = size - 1;
    while (l + 1 < h) {
      int m = (l + h) / 2;
      if (menu[m].smallest < p) {
        l = m;
      } else {
        h = m;
      }
    }
    return l;
  } // return the last unit whose smallest index_value pair is smaller than p (all >=:-1)
  [[nodiscard]] int Menu::FindUpper(const index_value &p) const {
    if (menu[0].smallest > p) {
      return -1;
    }
    if (!(menu[size - 1].smallest > p)) {
      return size - 1;
    }
    int l = 0;
    int h = size - 1;
    while (l + 1 < h) {
      int m = (l + h) / 2;
      if (!(menu[m].smallest > p)) {
        l = m;
      } else {
        h = m;
      }
    }
    return l;
  } // return the last unit whose smallest index_value pair is smaller than or equal to p (all >:-1)
  void Menu::insert(const unit_menu &u) {
    if (size == 0) {
      size = 1;
      menu[0] = u;
    }
    if (u == menu[0] || u == menu[size - 1]) {
      return;
    }
    if (u < menu[0]) {
      for (int t = size; t > 0; --t) {
        menu[t] = menu[t - 1];
      }
      menu[0] = u;
      ++size;
    } else if (u > menu[size - 1]) {
      menu[size] = u;
      ++size;
    } else {
      int l = 0, h = size - 1;
      while (h - l > 1) {
        int m = (l + h) / 2;
        if (menu[m] > u) {
          h = m;
        } else if (menu[m] == u) {
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
  void Menu::erase(const unit_menu &u) {
    if (size == 0 || menu[0] > u || menu[size - 1] < u) {
      return;
    }
    if (menu[0] == u) {
      for (int t = 0; t < size - 1; ++t) {
        menu[t] = menu[t + 1];
      }
      --size;
      return;
    }
    if (menu[size - 1] == u) {
      --size;
      return;
    }
    int l = 0, h = size - 1;
    while(h > l + 1) {
      int m = (l + h) / 2;
      if (menu[m] == u) {
        for (int t = m; t < size - 1; ++t) {
          menu[t] = menu[t + 1];
        }
        --size;
        return;
      }
      if (menu[m] > u) {
        h = m;
      } else if (menu[m] < u) {
        l = m;
      }
    }
  }
  void insert(const std::string &BookName, const std::string &ISBN) {
    bool exist = false;
    std::fstream file("book_name.txt");
    if (file.is_open()) {
      exist = true;
    }
    file.close();
    if (!exist) {
      std::ofstream new_file("book_name.txt");
      new_file.close();
    }

    file.open("book_name.txt");
    file.seekg(0, std::ios::end);

    if (file.tellg() == 0) {
      Menu menu;
      file.seekp(0);
      file.write(reinterpret_cast<char *>(&menu), sizeof(menu));
      file.flush();
    } // if the file is empty, add the menu

    const std::string &index_str = BookName;
    const std::string &value = ISBN;
    hash_pair index = db_hash(index_str);
    index_value p{index, value}; // get what we really want to insert
    Menu menu;
    file.seekg(0);
    file.read(reinterpret_cast<char *>(& menu), sizeof(menu)); // read the menu
    if (menu.size == 0) { // if the menu is empty
      ++menu.size;
      block bl;
      bl.size = 1;
      bl.pairs[0] = p;
      file.seekp(0, std::ios::end);
      unsigned long long block_position = file.tellp();
      file.write(reinterpret_cast<char *>(& bl), sizeof(bl));
      menu.menu[0] = {block_position, p}; // revise the first one
      file.seekp(0);
      file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
      file.close();
      return;
    }
    // if the menu is not empty
    int insert_block = menu.FindLower(p); // try to find out where should we insert p
    if (insert_block == -1) {
      if (menu.FindUpper(p) != -1) {
        file.close();
        return;
      } // p and the first index_value are exactly the same, just ignore p;
      // now we have to create a new block, a new unit_menu and put p into them
      block new_block;
      new_block.size = 1;
      new_block.pairs[0] = p;
      file.seekp(0, std::ios::end);
      unsigned long long block_position = file.tellp();
      file.write(reinterpret_cast<char *>(& new_block), sizeof(new_block));
      unit_menu new_unit_menu{block_position, p};
      // then insert them into where they should belong
      menu.insert(new_unit_menu);
      file.seekp(0);
      file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
      file.close();
      return;;
    } // can't put p in any existed block
    // we should place p in the insert_block
    block bl;
    unsigned long long block_position = menu.menu[insert_block].block_pos;
    file.seekg(block_position);
    file.read(reinterpret_cast<char *>(& bl), sizeof(bl)); // read out the block
    bl.insert(p);
    int bls = bl.size;
    if (bls > MAX_BLOCK_SIZE) {
      // we need to split the block first
      block new_bl;
      new_bl.size = bls - bls / 2;
      for (int t = 0; t < bls - bls / 2; ++t) {
        new_bl.pairs[t] = bl.pairs[t + bls / 2];
      }
      bl.size = bls / 2;
      // ok, now generate a new unit_menu
      unit_menu new_unit_menu;
      file.seekp(0, std::ios::end);
      unsigned long long new_u_pos = file.tellp();
      file.write(reinterpret_cast<char *> (& new_bl), sizeof(new_bl));
      new_unit_menu.block_pos = new_u_pos;
      new_unit_menu.smallest = new_bl.pairs[0];
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
  void erase(const std::string &BookName, const std::string &ISBN) {
    bool exist = false;
    std::fstream file("book_name.txt");
    if (file.is_open()) {
      exist = true;
    }
    file.close();
    if (!exist) {
      std::ofstream new_file("book_name.txt");
      new_file.close();
    }

    file.open("book_name.txt");
    file.seekg(0, std::ios::end);

    if (file.tellg() == 0) {
      Menu menu;
      file.seekp(0);
      file.write(reinterpret_cast<char *>(&menu), sizeof(menu));
      file.flush();
    } // if the file is empty, add the menu

    const std::string &index_str = BookName;
    const std::string &value = ISBN;
    hash_pair index = db_hash(index_str);
    index_value p{index, value}; // get what we really want to erase
    Menu menu;
    file.seekg(0);
    file.read(reinterpret_cast<char *>(& menu), sizeof(menu)); // read the menu
    if (menu.size == 0) { // if the menu is empty, can't erase anything
      file.close();
      return;;
    }
    int erase_block = menu.FindUpper(p);
    if (erase_block == -1) { // improbable to have p
      file.close();
      return;;
    }
    block bl;
    unsigned long long block_position = menu.menu[erase_block].block_pos;
    file.seekg(block_position);
    file.read(reinterpret_cast<char *>(& bl), sizeof(bl));
    if (bl.pairs[0] == p) { // if we remove p, the smallest will change
      if (bl.size == 1) { // empty after removal, simply ignore the whole block
        menu.erase(menu.menu[erase_block]);
        file.seekp(0);
        file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
        file.close();
        return;;
      } else { // revise the unit_menu and remove p
        index_value second = bl.pairs[1]; // it'll replace current smallest
        menu.menu[erase_block] = {block_position, second};
        bl.erase(p);
      }
    } else {
      bl.erase(p);
    }
    // check whether we can merge this block with the one in front
    if (erase_block > 0) {
      block front;
      unsigned long long front_pos = menu.menu[erase_block - 1].block_pos;
      file.seekg(front_pos);
      file.read(reinterpret_cast<char *>(& front), sizeof(front));
      int bls = bl.size;
      int frs = front.size;
      if (bls + frs <= MAX_BLOCK_SIZE) { // we can merge bl into front, undoubtedly we should delete the unit_menu of bl
        // merge
        for (int t = 0; t < bls; ++t) {
          front.pairs[frs + t] = bl.pairs[t];
        }
        front.size = bls + frs;
        menu.erase(menu.menu[erase_block]);
        file.seekp(0);
        file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
        file.seekp(front_pos);
        file.write(reinterpret_cast<char *>(& front), sizeof(front));
        file.close();
        return;;
      }
    }
    file.seekp(0);
    file.write(reinterpret_cast<char *>(& menu), sizeof(menu));
    file.seekp(block_position);
    file.write(reinterpret_cast<char *>(& bl), sizeof(bl));
    file.close();
  }
  std::vector<std::string> find(const std::string &BookName) {
    std::vector<std::string> ans;
    bool exist = false;
    std::fstream file("book_name.txt");
    if (file.is_open()) {
      exist = true;
    }
    file.close();
    if (!exist) {
      std::ofstream new_file("book_name.txt");
      new_file.close();
    }

    file.open("book_name.txt");
    file.seekg(0, std::ios::end);

    if (file.tellg() == 0) {
      Menu menu;
      file.seekp(0);
      file.write(reinterpret_cast<char *>(&menu), sizeof(menu));
      file.flush();
    } // if the file is empty, add the menu

    const std::string &index_str = BookName;
    hash_pair index = db_hash(index_str);

    index_value small;
    small.index = index;
    index_value big;
    if (index.hash2 == 18446744073709551615) {
      big.index = {index.hash1 + 1, 0};
    } else {
      big.index = {index.hash1, index.hash2 + 1};
    }
    Menu menu;
    file.seekg(0);
    file.read(reinterpret_cast<char *>(& menu), sizeof(menu)); // read the menu
    if (menu.size == 0) {
      file.close();
      return ans;
    }
    int probable_first_block = menu.FindUpper(small);
    int probable_last_block = menu.FindUpper(big);
    if (probable_last_block == -1) {
      file.close();
      return ans;
    }
    if (probable_first_block == -1) {
      probable_first_block = 0;
    }
    while (probable_first_block <= probable_last_block) {
      unsigned long long block_pos = menu.menu[probable_first_block].block_pos;
      file.seekg(block_pos);
      block bl;
      file.read(reinterpret_cast<char *>(& bl), sizeof(bl));
      for (int k = 0; k < bl.size; ++k) {
        if (bl.pairs[k] < small) {
          continue;
        }
        if (bl.pairs[k] > big) {
          break;
        }
        ans.push_back(bl.pairs[k].value.ToString());
      }
      ++probable_first_block;
    }
    return ans;
  }
}