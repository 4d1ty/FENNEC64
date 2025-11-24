#ifndef VIXEN_CPU_HPP
#define VIXEN_CPU_HPP

#include "vixen/memory.hpp"

UWORD ip = CODE_START;

enum Registers : UWORD
{
    RA,
    RB,
    RC,
    RD,
    REG_COUNT,
};

WORD registers[REG_COUNT] = {0};

FLAG ZERO = 0;
FLAG CARRY = 0;

#endif // VIXEN_CPU_HPP