#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include<vector>
#include<array>
#include"Clock.h"
#include"Memory.h"
#include"LogicUnit.h"

enum state{
    instr_f,
    wait_for_instr,
    instr_d,
    instr_e,
    wait_for_r,
    wait_for_w
};

static const int one_bit_outs = 9;
static const int two_bit_outs = 5;

class ControlUnit : public Listener<6>, public Listener<1>,
public MultiReporter<1, one_bit_outs>,
public MultiReporter<2, two_bit_outs>
{

    Clock * clock;
    Value<6> opcode, func;
    std::array<Value<1>, one_bit_outs> out1;
    std::array<Value<2>, two_bit_outs> out2;
    state st;

    void send_sigs();


    public:
    ControlUnit(Clock* clock);
    void reset();
    
    virtual void update_input(int num, Value<6> value) override;
    virtual void update_input(int num, Value<1> value) override;
    virtual Value<1> get_output(int output, Value<1> ignore) const override;
    virtual Value<2> get_output(int output, Value<2> ignore) const override;
    
    using MultiReporter<2, two_bit_outs>::register_listener;
    using MultiReporter<1, one_bit_outs>::register_listener;
    
    // single bit outputs
    static const int Branch = 0;
    static const int DataRequest = 1;
    static const int DataReadWrite = 2;
    static const int RegWrite = 3;
    static const int SignExtend = 4;
    static const int ALUSrc1 = 5;
    static const int InstRequest = 6;
    static const int InstReadWrite = 7;
    static const int PCEnable = 8;
    
    // two bit outputs
    static const int ALUSrc2 = 0;
    static const int MemToReg = 1;
    static const int WriteRegDst = 2;
    static const int PCSrc = 3;
    static const int ALUOpType = 4;

    // 6-bit inputs
    static const int OpCode = 0;
    static const int Function = 1;
    
    // single bit inputs
    static const int InstWait = 0;
    static const int DataWait = 1;
};
#endif
