#ifndef ALU_CONTROL_UNIT_H
#define ALU_CONTROL_UNIT_H

#include<vector>
#include<array>
#include"LogicUnit.h"
#include"ALU.h"

class ALUControlUnit: public Listener<6>, public Listener<2>{
    Value<6> func;
    Value<2> control_in;
    ALU *alu;
    ALUop get_op() const;

    public:
    ALUControlUnit(ALU *alu);

    virtual void update_input(int num, Value<6> value) override;
    virtual void update_input(int num, Value<2> value) override;

};

#endif