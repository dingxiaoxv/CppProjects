#include <iostream>
#include <list>
#include <string>

struct CustomData {
  int value1;
  std::string value2;
};

void testInsert(std::list<CustomData> &_list) {
  for (auto it = _list.begin(); it != _list.end(); it++) {
    if (it->value1 == 2) {
      CustomData new1 = {2, "insert1"};
      CustomData new2 = {3, "insert2"};
      it = _list.insert(it, new1);
      it = _list.insert(it, new2);
      ++it;
    }
  }
}

void testErase(std::list<CustomData> &_list) {
  auto it = _list.end();
  std::advance(it, -3);
  _list.erase(it, _list.end());
}

void testEraseAndInsert() {
  std::list<int> myList = {1, 2, 3, 4, 5, 6, 7};

  auto it = myList.begin();
  while (it != myList.end()) {
    if (*it == 3) {
      // 找到值为3的元素
      auto next = it;
      ++next;

      if (next != myList.end()) {
        // 删除值为3的元素以及之后的一个元素
        it = myList.erase(it, ++next);
        // 在原位置插入值为2的元素
        myList.insert(it, 8);
      }
      break; // 退出循环，因为已经删除和插入了所需元素
    }
    ++it;
  }

  std::cout << "std::list中的元素：";
  for (const auto &value : myList) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
}

int main(int argc, const char *argv[]) {
  // std::list<CustomData> list;
  // list.push_back({1, "1"});
  // list.push_back({2, "2"});
  // list.push_back({3, "3"});
  // testInsert(list);
  // testErase(list);

  testEraseAndInsert();

  return 0;
}