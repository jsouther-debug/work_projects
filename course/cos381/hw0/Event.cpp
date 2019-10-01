#include "Event.h"
#include "common.h" // for current_time

Event::Event(long when, std::function<std::optional<Event>(void)> callback)
    : start_time(current_time + when), handler(callback) {
}

std::optional<Event> Event::handle() {
    return handler();
}

long Event::start() const {
    return start_time;
}

bool Event::operator<(const Event& other) const {
    return start_time > other.start_time; // order in reverse (HACK!)
}
