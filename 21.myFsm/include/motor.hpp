#ifndef MOTOR_HPP_INCLUDED
#define MOTOR_HPP_INCLUDED

#include <tinyfsm.hpp>

// 定义事件
struct MotorUp   : tinyfsm::Event { };
struct MotorDown : tinyfsm::Event { };
struct MotorStop : tinyfsm::Event { };
struct Sensor : tinyfsm::Event {
  double height = 0.0;
};

// 定义基类
class Motor : public tinyfsm::Fsm<Motor> {
public:
  /* default reaction for unhandled events */
  void react(tinyfsm::Event const &) { };

  /* non-virtual declaration: reactions are the same for all states */
  void react(const MotorUp &);
  void react(const MotorDown &);
  void react(const MotorStop &);

  virtual void entry(void) = 0;  /* pure virtual: enforce implementation in all states */
  void exit(void);

protected:
  static double height_;
  bool running_ = false;
};

#endif