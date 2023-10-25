#include <iostream>
#include <list>
#include <string>

struct CustomData
{
  int value1;
  std::string value2;
};


void testInsert(std::list<CustomData> &_list) {
  for (auto it = _list.begin(); it != _list.end(); it++)
  {
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

int main(int argc, const char* argv[]) {
  std::list<CustomData> list;
  list.push_back({1, "1"});
  list.push_back({2, "2"});
  list.push_back({3, "3"});
  // testInsert(list);
  testErase(list);

  return 0;
}