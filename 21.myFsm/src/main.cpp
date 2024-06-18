#include "fsmlist.hpp"
#include <iostream>


int main() {
  fsm_list::start();
  Call call;

  while(1) {
    char c;

    std::cout << "c=Call, a=Alarm, q=Quit ? " << std::endl;
    std::cin >> c;
    switch(c) {
    case 'c':
      std::cout << "Floor ? ";
      std::cin >> call.floor;
      send_event(call);
      break;
    case 'a':
      send_event(Alarm());
      break;
    case 'q':
      std::cout << "Thanks for playing!" << std::endl;
      return 0;
    default:
      std::cout << "Invalid input" << std::endl;
    };
  }
}
