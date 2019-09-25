#ifndef LOGIC_UNIT_H
#define LOGIC_UNIT_H

#include <iostream>
#include <utility>
#include <vector>
#include <array>

#include "common.h"

template<int N>
class Listener {
  public:
    virtual void update_input(int num, Value<N> value) = 0;
};

template<int N>
class Reporter {
    std::vector<std::pair<Listener<N> *, int>> listeners;

  public:
    void register_listener(Listener<N> *component, int input);
    void unregister_listener(Listener<N> *component);
    void update_listeners() const;

    virtual Value<N> get_output() const = 0;
};

template<int N, int num_outputs>
class MultiReporter {
    std::array<std::vector<std::pair<Listener<N>*, int>>, num_outputs> listeners;

  public:
    void register_listener(int output, Listener<N>* component, int input);
    void unregister_listener(Listener<N>* component);
    void update_listeners() const;

    virtual Value<N> get_output(int output, Value<N> ignore) const = 0;
};

template<int N>
class LogicUnit : public Listener<N>, public Reporter<N> {
};

template<int N>
void Reporter<N>::register_listener(Listener<N> *component, int input) {
//std::cout << "Component<" << N << ">(" << component << ") registered with " << this << "\n";
    component->update_input(input, get_output());
    listeners.push_back(std::make_pair(component, input));
}

template<int N>
void Reporter<N>::unregister_listener(Listener<N> *component) {
//std::cout << "Component (" << component << ") UNregistered\n";
    for (auto i = listeners.begin(); i != listeners.end(); ++i) {
	if (i->first == component) {
	    listeners.erase(i);
	    return;
	}
    }
    // throw exception?
}

template<int N>
void Reporter<N>::update_listeners() const {
    for (const auto& listener : listeners) {
	listener.first->update_input(listener.second, get_output());
    }
}

template<int N, int num_outputs>
void MultiReporter<N, num_outputs>::register_listener(int output, Listener<N>* component, int input) {
    component->update_input(input, get_output(output, Value<N>{0}));
    listeners[output].push_back(std::make_pair(component, input));
}

template<int N, int num_outputs>
void MultiReporter<N, num_outputs>::unregister_listener(Listener<N>* component) {
    for (int i = 0; i < num_outputs; ++i) {
	for (auto l = listeners[i].begin(); l != listeners[i].end(); ++l) {
	    if (l->first == component) {
		listeners[i].erase(l);
	    }
	}
    }
}

template<int N, int num_outputs>
void MultiReporter<N, num_outputs>::update_listeners() const {
    for (int i = 0; i < num_outputs; ++i) {
	for (const auto& listener : listeners[i]) {
	    listener.first->update_input(listener.second,
					 get_output(i, Value<N>{0}) );
	}
    }
}

#endif
