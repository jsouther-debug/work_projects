#include <iostream>

#include "Clock.h"
#include "common.h"

Clock::Clock(std::string n) : name(n) { }

Value<1> Clock::get_output() const {
    return 0;
}

ClockPeriodic::ClockPeriodic(long per, std::string name)
    : Clock(name), period(per)
{
    events.push(*clock_tick());
}

long ClockPeriodic::next_clock_tick() const {
    return period - current_time % period;
}

std::optional<Event> ClockPeriodic::clock_tick() const {
//std::cout << "TICK start (" << name << ")\n";
    update_listeners();
//std::cout << "TICK stop (" << name << ")\n";

    Event next_event{next_clock_tick(), [this]() -> std::optional<Event> {
	return clock_tick();
    } };

    return next_event;
}
