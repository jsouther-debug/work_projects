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

  private:
	int input_values[size];
	int index;
};

// implementation of Mux<size, width> follows
template<int size, int width>
Mux<size,width>::Mux(const std::string& n) {
    // code to construct Mux hereinput_values[size];
	for (int i = 0; i < size; i++){
		input_values[i] = i;
	}
	index = 0;
	std::string name = n;
}

template<int size, int width>
void Mux<size,width>::update_input(int input, Value<width> value) {
    // code to handle input updates here
	input_values[input] = value;
	Reporter<width>::update_listeners();
}

template<int size, int width>
void Mux<size,width>::update_input(int input, Value<log2(size)> value) {
    // code to handle select updates here
	index = value;
	Reporter<width>::update_listeners();
}

template<int size, int width>
Value<width> Mux<size,width>::get_output() const {
    // code to handle output here
	return input_values[index];
}

#endif
