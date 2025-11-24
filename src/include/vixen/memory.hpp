#ifndef VIXEN_MEMORY_HPP
#define VIXEN_MEMORY_HPP
#include "vixen/types.hpp"

// #define MEMORY_SIZE 4096
#define MEMORY_SIZE 32768

#define CODE_FRACTION 8  // 1/8 of Memory
#define DATA_FRACTION 8  // 1/8 of Memory
#define HEAP_FRACTION 2  // 1/2 of Memory [RUNTIME]
#define STACK_FRACTION 4 // 1/4 of Memory [RUNTIME]

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

ADDR hp = HEAP_END;          // Heap Pointer
ADDR sp = STACK_START;       // Stack Pointer
ADDR data_base = DATA_START; // Starting of the data segment
ADDR dp = data_base;         // Data Pointer

WORD fetch_word(ADDR address)
{
    if (address >= MEMORY_SIZE)
    {
        throw std::out_of_range("Memory access out of bounds");
    }
    return memory[address];
}

WORD read_word_from_file(std::ifstream &infile)
{
    WORD value = 0;
    infile.read(reinterpret_cast<char *>(&value), sizeof(WORD));
    return value;
}

#endif // VIXEN_MEMORY_HPP