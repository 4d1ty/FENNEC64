#pragma once
#include <unordered_map>

struct InstrInfo
{
    int opcode;
    int operands;
    bool has_mode;
};

enum InstructionSet
{
    PSH,
    POP,
    ADD,
    OUT,
    LOAD,
    HLT
};

enum Registers
{
    RA,
    RB,
    RC,
    RD,
    REG_COUNT,
};

enum Section
{
    TEXT,
    DATA,
};

std::unordered_map<std::string, InstrInfo> instr_map = {
    {"PSH", {PSH, 1, true}},
    {"POP", {POP, 1, false}},
    {"ADD", {ADD, 2, true}},
    {"OUT", {OUT, 1, true}},
    {"LOAD", {LOAD, 2, true}},
    {"HLT", {HLT, 0, false}}

};
std::unordered_map<std::string, int> register_map = {
    {"RA", RA},
    {"RB", RB},
    {"RC", RC},
    {"RD", RD},
};
