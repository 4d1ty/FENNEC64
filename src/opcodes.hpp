#pragma once
#include <unordered_map>
#include "cpu.hpp"
#include "types.hpp"

enum InstructionSet : UWORD
{
    PSH,
    POP,
    ADD,
    OUT,
    LOAD,
    JMP,
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
