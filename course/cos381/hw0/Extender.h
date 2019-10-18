#include "LogicUnit.h"

template<int in, int out>
class Extender : public Listener<in>, public Reporter<out>, public Listener<1>
{
	// private members here

	public:
		virtual void update_input(int input, Value<in> value) override;
		virtual void update_input(int input, Value<1> value) override;
		virtual Value<out> get_output() const override;
}
