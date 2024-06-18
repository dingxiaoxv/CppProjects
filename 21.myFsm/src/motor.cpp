#include "motor.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include "fsmlist.hpp"

// 定义状态类
class Stopped : public Motor {
  void entry() override {
    std::cout << "Motor: stopped" << std::endl;
  };
};

class Up : public Motor {
  void entry() override {
    running_ = true;
    std::cout << "Motor: moving up" << std::endl;
    
    std::thread t([this] {
      while (running_) {
        height_ += 0.1;
        Sensor ev;
        ev.height = height_;
        send_event(ev);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    });
    t.detach();
  };
};

class Down : public Motor {
  void entry() override {
    running_ = true;
    std::cout << "Motor: moving down" << std::endl;
    
    std::thread t([this] {
      while (running_) {
        if (height_ > 0) {
          height_ -= 0.1;
        }
        Sensor ev;
        ev.height = height_;
        send_event(ev);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    });
    t.detach();
  };
};

void Motor::react(const MotorUp &) {
  transit<Up>();
}

void Motor::react(const MotorDown &){
  transit<Down>();
}

void Motor::react(const MotorStop &) {
  transit<Stopped>();
}

void Motor::exit(void) {
  running_ = false;
};

double Motor::height_ = 0.0;

FSM_INITIAL_STATE(Motor, Stopped)