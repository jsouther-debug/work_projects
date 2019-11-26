#ifndef VALUE_SLICE_H
#define VALUE_SLICE_H
#include <string>
#include "LogicUnit.h"

template<int in, int out>
class ValueSlice : public Listener<in>, public Reporter<out> {
	// private members here
	int sta, sto;
	Value<out> output;
	std::string n;

	public:
		ValueSlice(int start, int stop, const std::string& name = "");
//		~ValueSlice();

		virtual void update_input(int input, Value<in> value) override;
		virtual Value<out> get_output() const override;
};


template<int in, int out>
ValueSlice<in, out>::ValueSlice(int start, int stop, const std::string& name){
	this->sta = start;
	this->sto = stop;
	this->output = 0;
	this->n = name;
}

template<int in, int out>
void ValueSlice<in, out>::update_input(int input, Value<in> value){
	output = (value >> sta) & ((1 << (sto - sta +1)) - 1);
	this->update_listeners();
}


template<int in, int out>
Value<out> ValueSlice<in, out>::get_output() const{
	return output;
}

#endif
