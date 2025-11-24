#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>

#include "vixen/opcodes.hpp"
#include "vixen/memory.hpp"
#include "vixen/cpu.hpp"

#include <SFML/Graphics.hpp>
#include "vixen/vconsole.hpp"

#define VCONSOLE_FONT_PATH "FiraCode-Retina.ttf"


int fetch();
void eval(int);
void load_into_memory(const std::string &filename, WORD *memory);
void set_value(OperandType mode, WORD operand, WORD value);
WORD get_value(OperandType mode, WORD operand);

std::vector<WORD> get_words_from_memory(ADDR start = 0, ADDR end = MEMORY_SIZE, WORD memory_table[] = memory)
{
    std::vector<WORD> words;
    for (ADDR ptr = start; ptr < end; ++ptr)
    {
        words.push_back(memory_table[ptr]);
    }
    return words;
}

std::vector<WORD> program;

bool running = true;
bool debug = false;
sf::Font termFont(VCONSOLE_FONT_PATH);
VirtualConsole console(800, 600, 16, termFont);
int main(int argc, char const *argv[])
{
    std::cout << "[VIXEN]" << std::endl;
    if (argc < 2)
    {
        std::cerr << "Usage: vixen <code.bin>" << std::endl;
        return 1;
    }
    if (argc > 2)
    {
        if (std::string(argv[2]) == "-d")
        {
            debug = true;
        }
    }

    load_into_memory(argv[1], memory);
    program = get_words_from_memory(CODE_START, CODE_END, memory);
    if(program.size() == 0){
        std::cerr << "No program loaded. Exiting.\n";
        return 1;
    }
    sf::Font termFont;
    if(!termFont.openFromFile(VCONSOLE_FONT_PATH)){
        std::cerr << "Failed to load font for VirtualConsole\n";
        return 1;
    }

    console.print("[VIXEN]\n\n");
    // console.putChar('H');
    // console.putChar('i');
    // console.putChar('\n');
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Vixen VM");
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            window.close();
        }
        
        window.clear(sf::Color(28,28,28));
        // window.clear(sf::Color::Black);
        console.render(window);
        window.display();
        // Yay, we have a window now!
        if (running)
            eval(fetch());
    }
    return 0;
}

WORD fetch()
{
    if (ip >= static_cast<UWORD>(program.size()))
    {
        return HLT;
    }
    return program[ip++];
}

// Mode 0: top of the stack
// Mode 1: from the registor
// Mode 2: Immediate
// Mode 3: Memory

void eval(int instruction)
{
    switch (instruction)
    {
    case HLT:
    {

        if (debug)
        {
            std::cout << "HLT encountered\nFinal stack: [";
            console.print("HLT encountered\nFinal stack: [");
            for (ADDR hsp = STACK_START + 1; hsp <= sp; hsp++) // hsp: halt stack pointer
            {
                std::cout << memory[hsp];
                console.print(std::to_string(memory[hsp]));
                if (hsp != sp)
                {
                    std::cout << ", ";
                    console.print(", ");
                    
                }
            }
            std::cout << "]\nRegisters: RA=" << registers[RA] << " RB=" << registers[RB]
                      << " RC=" << registers[RC] << " RD=" << registers[RD];
            std::cout << "\nIP=" << ip << std::endl;
            char buffer[256];
            sprintf(buffer, "]\nRegisters: RA=%d RB=%d RC=%d RD=%d\nIP=%d\n", registers[RA], registers[RB], registers[RC], registers[RD], ip);
            console.print(buffer);
        }
        else
        {
            std::cout << "Yip! Program complete.\n";
            console.print("Yip! Program complete.\n");
            std::cout << "Top of stack : " << memory[sp] << "\n";
            console.print("Top of stack : " + std::to_string(memory[sp]) + "\n");
        }
        running = false;
        break;
    }
    case PSH:
    {
        UWORD src_mode = fetch();
        WORD src = fetch();
        memory[++sp] = get_value(static_cast<OperandType>(src_mode), src);
        break;
    }
    case POP:
    {
        UWORD dest_mode = fetch();
        WORD dest = fetch();       // We have to waste one cycle
        WORD value = memory[sp--]; // OR --sp?
        if (dest_mode != NONE)
        {
            set_value(static_cast<OperandType>(dest_mode), dest, value);
        }
        break;
    }
    case ADD:
    {
        UWORD dest_mode = fetch();
        WORD dest = fetch();
        UWORD src_mode = fetch();
        WORD src = fetch();

        WORD dest_val = get_value(static_cast<OperandType>(dest_mode), dest);
        WORD src_val = get_value(static_cast<OperandType>(src_mode), src);

        WORD sum = dest_val + src_val;
        set_value(static_cast<OperandType>(dest_mode), dest, sum);

        break;
    }

    case LOAD:
    {
        UWORD dest_mode = fetch();
        WORD dest = fetch();
        UWORD src_mode = fetch();
        WORD src = fetch();
        WORD value = get_value(static_cast<OperandType>(src_mode), src);
        set_value(static_cast<OperandType>(dest_mode), dest, value);
        break;
    }
    case OUT:
    {
        UWORD src_mode = fetch();
        WORD src = fetch();
        WORD value = get_value(static_cast<OperandType>(src_mode), src);
        std::cout << value << std::endl;
        console.print(std::to_string(value) + "\n");
        break;
    }
    case JMP:
    {
        UWORD mode = fetch();
        WORD procedure = fetch();
        ADDR addr = get_value(static_cast<OperandType>(mode), procedure);
        ip = addr; // JMP!
        break;
    }
    case OUTC:
    {
        UWORD src_mode = fetch();
        WORD src = fetch();
        WORD value = get_value(static_cast<OperandType>(src_mode), src);
        char c = static_cast<char>(value);
        std::cout << c << std::flush;
        console.putChar(c);
        break;
    }
    default:
    {
        std::cout << "UNKNOWN INSTRUCTION: " << instruction << "\n";
        console.print("UNKNOWN INSTRUCTION: " + std::to_string(instruction) + "\n");
        break;
    }
    }
}

WORD get_value(OperandType mode, WORD operand)
{
    switch (mode)
    {
    case NONE:
        return memory[sp];
    case REGISTER:
        return registers[operand];
    case INDIRECT:
        return operand;
    case MEMORY:
        return memory[operand];
    case PROCEDURE:
        return operand;
    default:
        std::cerr << "Error: Invalid operand mode " << mode << std::endl;
        running = false;
        return 0;
    }
}

void set_value(OperandType mode, WORD operand, WORD value)
{
    switch (mode)
    {
    case REGISTER:
        registers[operand] = value;
        break;
    case MEMORY:
        memory[operand] = value;
        break;
    default:
        std::cerr << "Invalid destination mode" << std::endl;
        running = false;
        break;
    }
}

void load_into_memory(const std::string &filename, WORD *memory)
{
    memory[MEMORY_SIZE] = {0}; // Clear memory
    std::ifstream infile(filename, std::ios::binary);
    if (!infile)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    ADDR ptr = -1; // A head pointer
    UWORD __code_size = read_word_from_file(infile);
    // std::cout << "Code Size: " << __code_size << std::endl;
    for (ptr = CODE_START; ptr < __code_size; ++ptr)
    {
        memory[ptr] = read_word_from_file(infile);
    }
    UWORD __data_size = read_word_from_file(infile);
    // std::cout << "Data Size: " << __data_size << std::endl;
    // std::cout << "Data start: " << DATA_START << std::endl;
    for (ptr = DATA_START; ptr < DATA_START + __data_size; ++ptr)
    {
        memory[ptr] = read_word_from_file(infile);
    }
}