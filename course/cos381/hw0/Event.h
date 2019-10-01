#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <optional>

class Event {
    std::function<std::optional<Event>(void)> handler;
    long start_time;

  public:
    Event(long when, std::function<std::optional<Event>(void)> callback);
    std::optional<Event> handle();
    long start() const;
    bool operator<(const Event& other) const;
};

#endif
