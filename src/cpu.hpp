#pragma once
#include "types.hpp"
#include "memory.h"

enum Registers : UWORD
{
    RA,
    RB,
    RC,
    RD,
    REG_COUNT,
};

WORD registers[REG_COUNT] = {0};
