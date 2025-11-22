#pragma once
#include "types.hpp"

// #define MEMORY_SIZE 4096
#define MEMORY_SIZE 32768

#define CODE_FRACTION 8  // 1/8 of Memory
#define DATA_FRACTION 8  // 1/8 of Memory
#define HEAP_FRACTION 2  // 1/2 of Memory
#define STACK_FRACTION 4 // 1/4 of Memory

constexpr UWORD code_size = MEMORY_SIZE / CODE_FRACTION;
constexpr UWORD data_size = MEMORY_SIZE / DATA_FRACTION;
constexpr UWORD heap_size = MEMORY_SIZE / HEAP_FRACTION;
constexpr UWORD stack_size = MEMORY_SIZE / STACK_FRACTION;

// Segment addresses
constexpr UWORD CODE_START = 0;
constexpr UWORD CODE_END = CODE_START + code_size;

constexpr UWORD DATA_START = CODE_END;
constexpr UWORD DATA_END = DATA_START + data_size;

constexpr UWORD HEAP_START = DATA_END;
constexpr UWORD HEAP_END = HEAP_START + heap_size;

constexpr UWORD STACK_START = HEAP_END;
constexpr UWORD STACK_END = MEMORY_SIZE;

WORD memory[MEMORY_SIZE] = {0};

UWORD hp = HEAP_END;          // Heap Pointer
UWORD sp = STACK_START;       // Stack Pointer
UWORD data_base = DATA_START; // Starting of the data segment
UWORD dp = data_base;         // Data Pointer

WORD fetch_word(ADDR address)
{
    if (address >= MEMORY_SIZE)
    {
        throw std::out_of_range("Memory access out of bounds");
    }
    return memory[address];
}