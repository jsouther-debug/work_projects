#include "LogicUnit.h"

template<int N>
	class AndGate : public LogicUnit<N> {
		Value<N> v1 = 0;
		Value<N> v2 = 0;
		

		public:
			virtual void update_input(int num, Value<N> value) override;
			virtual Value<N> get_output() const override;
};


template<int N>
void AndGate<N>:: update_input(int num, Value<N> value) {
	if (num == 0){
		v1 = value;
		this->update_listeners();
	}
	else if(num == 1){
		v2 = value;
		this->update_listeners();
	}
	return;
}


template<int N>
Value<N> AndGate<N>::get_output() const {
	return v1 & v2;
}

