#ifndef MUX_H
#define MUX_H

#include <iostream>
#include <string>
#include <array>

#include "LogicUnit.h"

constexpr int log2(int n) {
    int i = 0;
    while (n > 1) {
	n /= 2;
	++i;
    }

    return i;
}

template<int size, int width>
class Mux : public LogicUnit<width>, public Listener<log2(size)> {
    // private data members necessary to keep track of inputs
    // private data members necessary to keep track of select bits

  public:
    Mux(const std::string& name = "");
    virtual void update_input(int input, Value<width> value) override;
    virtual void update_input(int input, Value<log2(size)> value) override;
    virtual Value<width> get_output() const override;
};

// implementation of Mux<size, width> follows
template<int size, int width>
Mux<size,width>::Mux(const std::string& n) {
    // code to construct Mux here
}

template<int size, int width>
void Mux<size,width>::update_input(int input, Value<width> value) {
    // code to handle input updates here
}

template<int size, int width>
void Mux<size,width>::update_input(int input, Value<log2(size)> value) {
    // code to handle select updates here
}

template<int size, int width>
Value<width> Mux<size,width>::get_output() const {
    // code to handle output here
}

#endif
