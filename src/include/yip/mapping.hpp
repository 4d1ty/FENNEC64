#ifndef VIXEN_MAPPING_HPP
#define VIXEN_MAPPING_HPP
#include <unordered_map>
#include <string>
#include "vixen/types.hpp"
#include "vixen/opcodes.hpp"

std::unordered_map<std::string, InstrInfo> instr_map = {
    {"PSH", {
                PSH,
                1,
                1,
            }},
    {"POP", {
                POP,
                0,
                1,
            }},
    {"ADD", {
                ADD,
                2,
                2,
            }},
    {"OUT", {
                OUT,
                0,
                1,
            }},
    {"LOAD", {
                 LOAD,
                 2,
                 2,
             }},
    {"JMP", {
                 JMP,
                 1,
                 1,
             }},
    {"OUTC", {
                 OUTC,
                 1,
                 1,
             }},
    {"HLT", {
                HLT,
                0,
                1,
            }}

};

std::unordered_map<std::string, int> register_map = {
    {"RA", RA},
    {"RB", RB},
    {"RC", RC},
    {"RD", RD},
};

ADDR ENTRY_POINT_ADDRESS = CODE_START; // Default entry point address (Might be overridden in the assembler)



std::unordered_map<std::string, ADDR> procedure_map = {
    {"MAIN", ENTRY_POINT_ADDRESS},
};

/*

The program starts at the address defined by ENTRY_POINT_ADDRESS
The procedure_map maps procedure names to their starting addresses in memory.

In FENNEC Assembly, we can simple CALL PROC_NAME to jump to the procedure defined in procedure_map
Or JMP PROC_NAME to unconditionally jump to that address.

The program starts at the MAIN procedure by default, and its memory address is stored in ENTRY_POINT_ADDRESS.

*/

#endif // VIXEN_MAPPING_HPP