#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <vector>
#include "Clock.h"
#include "Register.h"
#include "LogicUnit.h"
#include "common.h"

class RegisterFile : public Listener<5>, public Listener<32>,
		     public Listener<1>, public MultiReporter<32, 2>
{
    // private data members here
	std::vector<Register<32>> rfile;
	int	r1,	r2, rd;
	word next_val;

  public:
    RegisterFile(Clock* synch);

    virtual void update_input(int num, word value) override;
    virtual void update_input(int num, Value<5> value) override;
    virtual void update_input(int num, Value<1> value) override;
    virtual word get_output(int output, word ignore) const override;
};

#endif
