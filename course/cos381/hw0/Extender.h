#include "LogicUnit.h"

template<int in, int out>
class Extender : public Listener<in>, public Reporter<out>, public Listener<1>
{
	// private members here
	Value<in> input1;
	Value<1> sign_bit;

	public:
		virtual void update_input(int input, Value<in> value) override;
		virtual void update_input(int input, Value<1> value) override;
		virtual Value<out> get_output() const override;
};



template<int in, int out>
void Extender<in, out>::update_input(int input, Value<in> value){
	input1 = value;
	this->update_listeners();
}

template<int in, int out>
void Extender<in, out>::update_input(int input, Value<1> value){
	sign_bit = value;
	this->update_listeners();
}



template<int in, int out>
Value<out> Extender<in, out>::get_output() const {
	int asbit = sign_bit & input1 >> (in - 1);
	Value<out> var;
	if (asbit){
		var = (1 << (out - in)) - 1;
		var = var << in;
		return var | input1;
	}
	else {
		return input1 & -1;
	}
}


