#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include "opcodes.hpp"

int fetch();
void eval(int);
std::vector<int32_t> load_bytecode(const std::string &);

int32_t stack[256];
int32_t sp = -1;

int registers[REG_COUNT] = {0};
int memory[256] = {0};

int32_t ip = 0;

std::vector<int32_t> program;

bool running = true;

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: vm <bytecode.bin>" << std::endl;
        return 1;
    }
    program = load_bytecode(argv[1]);
    std::cout << sizeof(program) << std::endl;

    // Debug: Print loaded bytecode, print the pure binary
    std::cout << "Loaded bytecode:" << std::endl;
    int i = 0;
    for (i = 0; i < program.size(); ++i)
    {
        std::cout << i << ": " << program[i] << std::endl;
    }
    std::cout <<  "I" << i << std::endl;

    while (running)
    {
        eval(fetch());
    }
    std::cout << stack[--sp] << std::endl;
    return 0;
}

int fetch()
{
    if (ip >= program.size())
    {
        return HLT;
    }
    return program[ip++];
}

std::vector<int32_t> load_bytecode(const std::string &filename)
{
    std::ifstream infile(filename, std::ios::binary);
    if (!infile)
    {
        std::cerr << "Failed to open bytecode file: " << filename << std::endl;
        return {};
    }

    std::vector<int32_t> bytecode;
    int32_t value;
    while (infile.read(reinterpret_cast<char *>(&value), sizeof(value)))
    {
        bytecode.push_back(value);
    }
    infile.close();
    return bytecode;
}

void eval(int instruction)
{
    switch (instruction)
    {
    case HLT:
        running = false;
        break;
    case PSH:
    {

        int mode = fetch();
        int dest = fetch();
        int value;
        switch (mode)
        {
        case 0:
            std::cerr << "Invalid Mode";
            break;
        case 1:
            value = registers[dest];
            break;
        case 2:
            value = dest;
            break;
        case 3:
            value = memory[dest];
            break;
        default:
            break;
        }
        stack[++sp] = value;
        break;
    }
    case POP:
        break;
    case ADD:
    {

        int sum = 0;
        while (sp > -1)
        {
            sum += stack[sp--];
        }
        stack[++sp] = sum;
        break;
    }

    case LOAD:
    {
        int mode = fetch();
        int dest = fetch();
        int src = fetch();
        registers[dest] = src;
        break;
    }
    case OUT:
    {
        // Mode 0: top of the stack
        // Mode 1: from the registor
        // Mode 2: Memory
        // Mode 3: Immediate
        int mode = fetch();
        if (mode == 0)
        {
            std::cout << stack[sp] << std::endl;
        }
        else if (mode == 1)
        {
            std::cout << registers[fetch()] << std::endl;
        }
        break;
    }
    default:
        break;
    }
}
