#pragma once
#include "types.hpp"
#include "ram.hpp"

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