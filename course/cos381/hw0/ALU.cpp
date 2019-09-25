#include "ALU.h"

Value<3> toValue(ALUop op) {
    return {static_cast<unsigned long>(op)};
}

ALU::ALU() {
    // implement ALU constructor here
}

Value<1> ALU::get_output(int output, Value<1> ignore) const {
    // return condition code associated with output
    return 0;
}

word ALU::get_output() const {
    // return computed value from ALU
}

void ALU::update_input(int num, Value<3> value) {
    // receive new operation
}

void ALU::update_input(int num, word value) {
    // receive new input value
}
