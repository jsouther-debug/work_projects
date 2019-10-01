#ifndef CLOCK_H
#define CLOCK_H

#include <functional>
#include <optional>
#include <string>
#include <queue>

#include "Event.h"
#include "LogicUnit.h"
#include "common.h"

class Clock : public Reporter<1> {
  protected:
    std::string name;

  public:
    Clock(std::string name);

    virtual Value<1> get_output() const override;
};

class ClockPeriodic : public Clock {
    long period;

    long next_clock_tick() const;
    std::optional<Event> clock_tick() const;

  public:
    ClockPeriodic(long period, std::string name);
};

#endif
