#pragma once
#include <unordered_map>
#include <string>
#include "types.hpp"
#include "opcodes.hpp"

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
