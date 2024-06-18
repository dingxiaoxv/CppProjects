#include "elevator.hpp"

#include <iostream>
#include <cmath>
#include "fsmlist.hpp"

const double EPSILON = 1e-9;

static void CallMaintenance() {
  std::cout << "*** calling maintenance ***" << std::endl;
}

static void CallFirefighters() {
  std::cout << "*** calling firefighters ***" << std::endl;
}

static bool doubleEqual(double a, double b, double epsilon = EPSILON) {
  return std::fabs(a - b) < epsilon;
}

// 定义状态类
class Idle;

class Panic : public Elevator {
public:
  void entry() override {
    send_event(MotorStop());
  }
};

class Moving : public Elevator {
public:
  void react(const Sensor &e) override {
    double expected = (double)dest_floor_ * 3.0;
    if(!doubleEqual(expected, e.height)) {
      std::cout << "Floor sensor defect (expected " << expected << ", got " << e.height << ")" << std::endl;
    } else {
      std::cout << "Reached floor " << dest_floor_ << std::endl;
      current_floor_ = dest_floor_;
      transit<Idle>();
    }
  }
};

class Idle : public Elevator {
public:
  void entry() override {
    send_event(MotorStop());
  }

  void react(const Call &e) override {
    dest_floor_ = e.floor;

    if(dest_floor_ == current_floor_) {
      std::cout << "invalid floor" << std::endl;
      return;
    }

    /* lambda function used for transition action */
    auto action = [] { 
      if(dest_floor_ > current_floor_)
        send_event(MotorUp());
      else if(dest_floor_ < current_floor_)
        send_event(MotorDown());
    };

    transit<Moving>(action);
  };
};

void Elevator::react(const Call &) {
  std::cout << "Call event ignored" << std::endl;
}

void Elevator::react(const Sensor &) {
  std::cout << "Sensor event ignored" << std::endl;
}

void Elevator::react(const Alarm &) {
  transit<Panic>(CallFirefighters);
}

int Elevator::current_floor_ = Elevator::initial_floor_;
int Elevator::dest_floor_ = Elevator::initial_floor_;
FSM_INITIAL_STATE(Elevator, Idle)
