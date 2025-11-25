#ifndef VIXEN_OPCODES_HPP
#define VIXEN_OPCODES_HPP
#include <unordered_map>
#include "vixen/cpu.hpp"
#include "vixen/types.hpp"

enum InstructionSet : UWORD
{
    PSH,
    POP,
    ADD,
    OUT,
    LOAD,
    JMP,
    OUTC,
    DRAWPIX,
    CMP, // compare, if l == r, set CF to 0 ZF 1, else, if l < r CF 1, ZF 0, else l > r CF 0 ZF 0
    JZ,
    JGT,
    JLZ,
    JE,
    CALL,
    RET,
    SYS, // syscall
    HLT
};

struct InstrInfo
{
    InstructionSet opcode;
    UWORD min_operands = 0;
    UWORD max_operands = 1;
};

enum Section
{
    TEXT,
    DATA,
};


#endif // VIXEN_OPCODES_HPP