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
    JZ,
    JGT,
    JLZ,
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