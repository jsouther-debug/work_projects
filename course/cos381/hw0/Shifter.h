#include "LogicUnit.h"

template<int N, int size>
class LeftShifter : public LogicUnit<N> {
	// private members here
	int shift_amount = size;
	Value<N> out;

	public:
		virtual void update_input(int input, Value<N> value) override;
		virtual Value<N> get_output() const override;
}


template<int N, int size>
void LeftShifter<N>::update_input(int input, Value<N> value){
	out = value << shift_amount;
	this->update_listeners();
}

template<int N, int size>
Value<N> get_output(){
	return out;
}

