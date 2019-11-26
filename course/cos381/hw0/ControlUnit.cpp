//#include<iostream>
#include"ControlUnit.h"

ControlUnit::ControlUnit(Clock* c) : clock(c), opcode(0), func(0), st(instr_f){clock->register_listener(this, -1)}

Value<1>ControlUnit::get_output(int output, Value<1> ignore) const {
    return out1[output];
}

Value<2>ControlUnit::get_output(int output, Value<2> ignore) const {
    return out2[output];
}

void ControlUnit::send_sigs(){
    if (st == state::instr_d){
        switch (opcode)
        {
        case 0x00: //rtype instruction
            out1[Branch] = 0;
            out1[ALUSrc1] = 0;
            out2[MemToReg] = 0b00;
            out2[WriteRegDst] = 0b01;
            out2[ALUOpType] = 0b10;

            if (func == 0x08){
                out1[RegWrite] = 0;
                out2[PCSrc] = 0b10;
            }
            else{
                out1[RegWrite] = 1;
                out2[PCSrc] = 0b00;
            }

            if (func == 0x00 || func == 0x02){
                out2[ALUSrc2] = 0b10;
            }
            else{
                out2[ALUSrc2] = 0b00;
            }
            break;

        case 0x02: //jump
            out1[RegWrite] = 0;
            out2[PCSrc] = 0b01;
            break;

        case 0x03: //jump and link
            out1[RegWrite] = 1;
            out2[MemToReg] = 0b10;
            out2[WriteRegDst] = 0b10;
            out2[PCSrc] = 0b01;
            break;

        case 0x04: //branch equal
        case 0x05: //branch not equal
            out1[Branch] = 1;
            out1[RegWrite] = 0;
            out1[SignExtend] = 1;
            out1[ALUSrc1] = 0;
            out2[ALUSrc2] = 0b00;
            out2[PCSrc] = 0b00;
            out2[ALUOpType] = 0b01;
            break;

        case 0x08: //add immediate
            out1[Branch] = 0;
            out1[RegWrite] = 1;
            out1[SignExtend] = 1;
            out1[ALUSrc1] = 0;
            out2[ALUSrc2] = 0b01;
            out2[MemToReg] = 0b00;
            out2[WriteRegDst] = 0b00;
            out2[PCSrc] = 0b00;
            out2[ALUOpType] = 0b00;
            break;

        case 0x09: //add immediate unsigned
            out1[Branch] = 0;
            out1[RegWrite] = 1;
            out1[SignExtend] = 0;
            out1[ALUSrc1] = 0;
            out2[ALUSrc2] = 0b01;
            out2[MemToReg] = 0b00;
            out2[WriteRegDst] = 0b00;
            out2[PCSrc] = 0b00;
            out2[ALUOpType] = 0b00;

        case 0x0c: //and immediate
            out1[Branch] = 0;
            out1[RegWrite] = 1;
            out1[SignExtend] = 0;
            out1[ALUSrc1] = 0;
            out2[ALUSrc2] = 0b01;
            out2[MemToReg] = 0b00;
            out2[WriteRegDst] = 0b00;
            out2[PCSrc] = 0b00;
            out2[ALUOpType] = 0b11;
            break;

        case 0x2b: //store word
            out1[Branch] = 0;
            out1[RegWrite] = 0;
            out1[SignExtend] = 1;
            out1[ALUSrc1] = 0;
            out2[ALUSrc2] = 0b01;
            out2[PCSrc] = 0b00;
            out2[ALUOpType] = 0b00;
            out1[DataReadWrite] = 0;
            out1[DataRequest] = 1;
            st = state::wait_for_w;
            break;

        case 0x23: //load word
            out1[Branch] = 0;
            out1[RegWrite] = 1;
            out1[SignExtend] = 1;
            out1[ALUSrc1] = 0;
            out2[ALUSrc2] = 0b01;
            out2[MemToReg] = 0b01;
            out2[PCSrc] = 0b00;
            out2[ALUOpType] = 0b00;
            out1[DataReadWrite] = 1;
            out1[DataRequest] = 1;
            st = state::wait_for_r;
            break;

        case 0x3f: //Halt
            exit(0);
            break;
                    
        default:
            out1[RegWrite] = 0;
            break;
        }

        MultiReporter<2, two_bit_outs>::update_listeners();
        MultiReporter<1, one_bit_outs>::update_listeners();
    }
}

void ControlUnit::update_listeners(int num, Value<1> value){
    switch (num)
    {
    case -1: //clock
        if (st == state::instr_f){
            out1[InstReadWrite] = 1;
            out1[InstRequest] = 1;
            st = state::wait_for_instr;
            MultiReporter<1, one_bit_outs>::update_listeners();
        }
        else if (st = state::wait_for_instr){
            out1[PCEnable] = 0;
            out1[RegWrite] = 0;
            MultiReporter<1, one_bit_outs>::update_listeners();
        }
        break;
    
    case InstWait:
        if (value == 0){
            st = state::instr_d;
            send_sigs();
            if (st == state::instr_d){
                st = state::instr_e;
                out1[InstRequest] = 0;
                MultiReporter<1, one_bit_outs>::update_listeners();
            }
        }
        else {
            st = state::instr_f;
            out1[PCEnable] = 1;
            MultiReporter<1, one_bit_outs>::update_listeners();
        }
        break;

    case DataWait:
        if (value == 0){
            out1[DataRequest] = 0;
        }
        else{
            st = state::instr_e;
            out1[InstRequest] = 0;
        }
        MultiReporter<1, one_bit_outs>::update_listeners();
        break;
    }
}


void ControlUnit::update_input(int num, Value<1> value){
    if (num == 0){ //change op
        opcode = value;
        send_sigs();
    }
    else{
        func = value;
        if (opcode == 0){
            send_sigs();
        }
    }
}




void ControlUnit::reset(){
    out1[InstReadWrite] = 1;
    out1[InstRequest] = 1;
    st = state::wait_for_instr;
    MultiReporter<1, one_bit_outs>::update_listeners();
}

















