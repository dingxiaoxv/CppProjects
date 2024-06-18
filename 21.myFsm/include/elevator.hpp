#ifndef ELEVATOR_H_INCLUDED
#define ELEVATOR_H_INCLUDED

#include <tinyfsm.hpp>
#include "motor.hpp"

// 定义事件
struct FloorEvent : tinyfsm::Event {
  int floor;
};

struct Call : FloorEvent { };
struct Alarm : tinyfsm::Event { };

// 定义基类
class Elevator : public tinyfsm::Fsm<Elevator> {
public:
  /* default reaction for unhandled events */
  void react(tinyfsm::Event const &) { };

  virtual void react(const Call &);
  virtual void react(const Sensor &);
  void react(const Alarm &);

  virtual void entry(void) { };  /* entry actions in some states */
  void exit(void)  { };  /* no exit actions at all */

protected:
  static constexpr int initial_floor_ = 0;
  static int current_floor_;
  static int dest_floor_;
};

#endif