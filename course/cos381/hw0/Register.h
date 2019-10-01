#ifndef REGISTER_H
#define REGISTER_H

#include <iostream>
#include <queue>

#include "LogicUnit.h"
#include "Clock.h"
#include "Event.h"
#include "common.h"

template<int N>
class Register : public LogicUnit<N>, public Listener<1> {
    Clock *clock;

    Value<N> value, next_value;
    Value<1> enable;

    void handle_rising_edge();
    void set_enable(Value<1> new_enable);

  public:
    Register(Clock *clock, Value<N> start_value = 0);
    ~Register();

    Register(const Register& other);
    Register& operator=(const Register& other);

    Register(Register&& other);
    Register& operator=(Register&& other);

    virtual void update_input(int num, Value<N> value) override;
    virtual void update_input(int num, Value<1> value) override;
    virtual Value<N> get_output() const override;
};

template<int N>
Register<N>::Register(Clock *c, Value<N> start_value)
    : clock(c), value(start_value), next_value(start_value), enable(0)
{
    clock->register_listener(this, -1);
}

template<int N>
Register<N>::~Register<N>() {
    clock->unregister_listener(this);
}

template<int N>
Register<N>::Register(const Register<N>& other)
    : clock(other.clock), value(other.value),
      next_value(other.next_value), enable(other.enable)
{
    clock->register_listener(this, -1);
}

template<int N>
Register<N>& Register<N>::operator=(const Register<N>& other) {
    clock->unregister_listener(this);
    clock = other.clock;
    value = other.value;
    next_value = other.next_value;
    enable = other.enable;
    clock->register_listener(this, -1);

    return *this;
}


template<int N>
Register<N>::Register(Register<N>&& other)
    : clock(other.clock), value(other.value),
      next_value(other.next_value), enable(other.enable)
{
    // other will unregister in its destructor
    clock->register_listener(this, -1);
}

template<int N>
Register<N>& Register<N>::operator=(Register<N>&& other) {
    // other will unregister in its destructor
    clock->unregister_listener(this);
    other.clock->unregister_listener(&other);
    clock = other.clock;
    value = other.value;
    next_value = other.next_value;
    enable = other.enable;
    clock->register_listener(this, -1);

    return *this;
}

template<int N>
void Register<N>::update_input(int num, Value<1> value) {
    if (num == -1) { // clock
	handle_rising_edge();
    } else {
	set_enable(value);
    }
}

template<int N>
void Register<N>::update_input(int num, Value<N> value) {
    next_value = value;
}

template<int N>
void Register<N>::handle_rising_edge() {
    if (enable && value != next_value) {
//std::cout << this << ": writing value " << next_value << "\n";
	value = next_value;
	Event propogate_value{1, [this]() -> std::optional<Event> {
	    this->update_listeners();

	    return {};
	} };

	events.push(propogate_value);
    }
}

template<int N>
Value<N> Register<N>::get_output() const {
    return value;
}

template<int N>
void Register<N>::set_enable(Value<1> new_enable) {
    enable = new_enable;
}

#endif
