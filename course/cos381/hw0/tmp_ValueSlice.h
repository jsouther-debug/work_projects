#include <string>
#include "LogicUnit.h"

template<int in, int out>
class ValueSlice : public Listener<in>, public Reporter<out> {
	// private members here

	public:
		ValueSlice(int start, int stop, const std::string& name = "");
		~ValueSlice();

		virtual void update_input(int input, Value<in> value) override;
		virtual Value<out> get_output() const override;
}



template<int in, int out>
void ValueSlice<in, out>::update_input(int input, Value<in> value){

}


template<int in, int out>
Value<out> ValueSlice<in, out>::get_output() const{

}
