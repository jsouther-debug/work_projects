#include "ALUControlUnit.h"

ALUControlUnit::ALUControlUnit(ALU *aluc) : alu(aluc), control_in(0), func(0){}

ALUop ALUControlUnit::get_op() const {
    switch (func)
    {
    case 0x00:
        return ALUop::LeftShift;
    case 0x02:
        return ALUop::RightShift;
    case 0x22:
        return ALUop::Sub;
    case 0x26:
        return ALUop::Xor;
    case 0x27:
        return ALUop::Nor;
    case 0x2A:
        return ALUop::SetLessThan;
    default:
        return ALUop::Sum;
    }
}


void ALUControlUnit::update_input(int num, Value<2> value){
    control_in = value;
    switch (value)
    {
    case 0:
        alu->update_input(0, toValue(ALUop::Sum));
        break;
    case 1:
        alu->update_input(0, toValue(ALUop::Sub));
        break;
    case 2:
        alu->update_input(0, toValue(get_op()));
        break;
    case 3:
        alu->update_input(0, toValue(ALUop::Or));
        break;
    }
}


void ALUControlUnit::update_input(int num, Value<6> value){
    func = value;
    if (control_in == 2){
        alu->update_input(0, toValue(get_op()));
    }
}
