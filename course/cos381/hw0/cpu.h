#ifndef CPU_H
#define CPU_H

#include "Mux.h"
#include "ALU.h"
#include "RegisterFile.h"
#include "Gates.h"
#include "Extender.h"
#include "Shifter.h"
#include "ValueSlice.h"
#include "DirectAddress.h"
#include "Register.h"
#include "Clock.h"
#include "Memory.h"
#include "Event.h"
#include "ControlUnit.h"
#include "ALUControlUnit.h"

#include <queue>

using std::string_literals::operator""s;

class cpu {
    RegisterFile regs;
    ALUControlUnit aluc;
    ControlUnit con;

    Mux<4, 5> mux_ind{"mux_index"s};
    Mux<2, 32> branch_or_norm{"Branch or PC + 4"s};
    Mux<4, 32> pc_input{"Progam Counter input"s};
    Mux<4, 32> alu_mux{"ALU Source 2"};
    Mux<4, 32> data{"Write register"};
    Register<32> pc;

    ALU branch_addr;
    ALU pc4;
    ALU alu;

    AndGate<1> branch;
    LeftShifter<32, 2> bshift;
    Extender<16, 32> sie;
    Extender<5, 32> shifte;
    ValueSlice<32, 6> opcode, func;
    ValueSlice<32, 16> immed;
    ValueSlice<32, 26> address;
    ValueSlice<32, 5> rs, rt, rd, shift_amount;
    DirectAddress daddr;

    public:
        cpu(Clock* clock, Memory* imem, Memory* dmem);

};

#endif