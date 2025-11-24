#ifndef VIXEN_TYPES_HPP
#define VIXEN_TYPES_HPP
#include <cstdint>

// Defining out base type
using WORD = int32_t;   // Signed 4 Bytes
using UWORD = uint32_t; // Unsigned 4 Bytes
using ADDR = UWORD;     // Address type
using BYTE = uint8_t;   // Unsigned 1 Byte
using FLAG = bool;
enum OperandType : UWORD
{
    NONE,
    REGISTER,
    INDIRECT,
    MEMORY,
    PROCEDURE
};

#endif // VIXEN_TYPES_HPP