#include <iostream>
#include <climits>

#include "CPU.h"

cpu::cpu(Clock* clock, Memory* imem, Memory* dmem){
    this->control(clock);
    this->regs(clock);
    this->pc(clock, 0x1000);
    this->opcode(26,31, "opcode");
    this->rs(21, 25, "rs");
    this->rt(16,20, "rt");
    this->rd(11, 15, "rd");
    this->shift_amount(6, 10);
    this->func(0, 5);
    this->immed(0, 15);
    this->address(0, 25);
    this->aluc(&alu);

    //Geislers code, still don't fully understand what this first block is doing
    control.register_listener(ControlUnit::InstRequest, imem, 0);
    control.register_listener(ControlUnit::InstReadWrite, imem, 1);
    imem->register_wait_listener(&control, ControlUnit::InstWait);
    control.register_listener(ControlUnit::DataRequest, dmem, 0);
    control.register_listener(ControlUnit::DataReadWrite, dmem, 1);
    dmem->register_wait_listener(&control, ControlUnit::DataWait);
    control.reset();

    //starting at the beginning wit pc + 4 and branch value incase we branch, and necessary components
    pc4.update_input(1, word{4});
    pc4.update_input(0, toValue(ALUop::Sum));
    sie.register_listener(&bshift, 0);
    pc4.Reporter<32>::register_listener(&branch_addr, 0);
    bshift.register_listener(&branch_addr, 1);
    branch_addr.update_input(0, toValue(ALUop::Sum));

    //set up branching structure
    control.register_listener(ControlUnit::Branch, &branch);
    alu.MultiReporter<1, num_codes>::register_listener(ALU::Zero, &branch, 1);
    pc4.Reporter<32>::register_listener(&branch_or_norm, 0);
    branch_addr.Reporter<32>::register_listener(&branch_or_norm, 1);
    branch.register_listener(&branch_or_norm, 2);
    branch_or_norm.register_listener(&pc_input, 0);
    daddr.register_lister(&pc_input, 1);
    regs.register_listener(0, &pc_input, 2);
    control.register_listener(ControlUnit::PCSrc, &pc_input, 4);
    pc_input.register_listener(&pc, 0);
    control.register_listener(ControlUnit::PCEnable, &pc, 0);
    pc.register_listener(&pc4, 0);
    pc.register_listener(imem, 0);

    //listeners on the instructions/different part of the instructions
    imem->register_listener(&opcode, 0);
    imem->register_listener(&rs, 0);
    imem->register_listener(&rt, 0);
    imem->register_listener(&rd, 0);
    imem->register_listener(&shift_amount, 0);
    imem->register_listener(&func, 0);
    imem->register_listener(&immed, 0);
    imem->register_listener(&address, 0);
    
    pc.register_listener(&daddr, 0);
    address.register_listener(&daddr, 1);
    regs.register_listener(0, &alu, 0);
    alu_mux.register_listener(&alu, 1);
    
    //registerfile setup to take instructions
    rs.register_listener(&regs, 0);
    rt.register_listener(&regs, 1);
    mux_ind.register_listener(&regs, 2);
    data.register_listener(&regs, 3);
    control.register_listener(ControlUnit::RegWrite, &regs, 4);
    rt.register_listener(&mux_ind, 0);
    rd.register_listener(&mux_ind, 1);
    mux_ind.update_input(2, Value<5>{31});
    control.register_listener(ControlUnit::WriteRegDst, &mux_ind, 2);

    // various extensions, etc
    shift_amount.register_listener(&shifte, 0);
    immed.register_listener(&sie, 0)
    control.register_listener(ControlUnit::SignExtend, &sie, 0);
    regs.register_listener(1, &alu_mux, 0);
    sie.register_listener(&alu_mux, 1);
    shifte.register_listener(&alu_mux, 2);
    pc4.Reporter<32>::register_listener(&alu_mux, 3);
    control.register_listener(ControlUnit::ALUSrc2, &alu_mux, 2);

    //write back to the register file if needed
    alu.Reporter<32>::register_listener(dmem, 0);
    regs.register_listener(1, dmem, 1);
    alu.Reporter<32>::register_listener(&data, 0);
    dmem->register_listener(&data, 1) //data not instruction
    pc4.Reporter<32>::register_listener(&data, 2);
    control.register_listener(ControlUnit::MemToReg, &data, 4);
    control.register_listener(ControlUnit::ALUOpType, &aluc, 0);

    opcode.register_listener(&control, 0);
    func.register_listener(&control, 1);
    func.register_listener(&aluc, 0);
}