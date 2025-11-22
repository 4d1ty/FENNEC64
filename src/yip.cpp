#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <iomanip>
#include "opcodes.hpp"
#include "memory.hpp"
#include "utils.hpp"

bool is_register(const std::string &s, const std::unordered_map<std::string, int> &reg_map)
{
    auto it = reg_map.find(s);
    if (it != reg_map.end())
    {
        return true;
    }
    return false;
}

bool is_memory(const std::string &s, const std::unordered_map<std::string, ADDR> &data_map)
{
    auto it = data_map.find(s);
    if (it != data_map.end())
    {

        return true;
    }
    return false;
}


int main(int argc, char const *argv[])
{

    if (argc < 3)
    {
        std::cerr << "Usage: yip <input.fnc> <output.bin>" << std::endl;
        return 1;
    }
    std::ifstream sourceFile(argv[1]);
    if (!sourceFile.is_open())
    {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }
    std::vector<WORD> bytecode;
    std::unordered_map<std::string, ADDR> data_segments;

    Section currentSection = TEXT;

    std::string line;
    int line_no = 0;
    while (std::getline(sourceFile, line))
    {
        ++line_no;

        std::istringstream iss(line);
        std::string token;
        if (line.empty() || line.at(0) == ';')
            continue;
        if (line == ".text")
        {
            currentSection = TEXT;
            continue;
        }
        else if (line == ".data")
        {
            currentSection = DATA;
            continue;
        }

        if (currentSection == DATA)
        {
            std::string token;
            int32_t value;
            while (iss >> token)
            {
                if (token.back() == ':')
                {
                    token.pop_back();
                    if (iss >> value)
                    {
                        data_segments[token] = dp;
                        memory[dp] = value;
                        dp++;
                    }
                }
            }
        }

        if (currentSection == TEXT)
        {
            // Handeling the text section
            auto comment_position = line.find(";");
            if (comment_position != std::string::npos)
            {
                line = line.substr(0, comment_position);
            }
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            std::string token;
            while (iss >> token)
            {
                tokens.push_back(token);
            }

            // Doing the actual thing
            std::string instr = tokens.at(0);
            auto info = instr_map.find(instr);
            int32_t mode;
            std::vector<int32_t> ops;
            if (tokens.size() - 1 > static_cast<size_t>(info->second.operands))
            {
                std::cerr << "Error: Too many operands for instruction " << instr << " at line " << line_no << std::endl;
                return -1;
            }
            if (info == instr_map.end())
            {
                std::cerr << "Error: Unknown instruction " << instr << " at line " << line_no << std::endl;
                return -1;
            }
            if (info->second.has_mode)
            {
                if (tokens.size() == 1)
                {
                    mode = 0; // Default mode
                }
                else
                {
                    std::string op = tokens.at(1);
                    if (is_register(op, register_map))
                    {
                        mode = 1; // Register mode
                        ops.push_back(register_map[op]);
                    }
                    else if (is_number(op))
                    {
                        mode = 2; // Immediate mode
                        ops.push_back(std::stoi(op));
                    }
                    else if (is_memory(op, data_segments))
                    {
                        mode = 3; // Memory mode
                        ops.push_back(data_segments.find(op)->second);
                    }
                    else
                    {
                        std::cerr << "Error: Invalid operand " << op << " at line " << line_no << std::endl;
                        return -1;
                    }
                }
            }
            bytecode.push_back(info->second.opcode);
            if (info->second.has_mode)
            {
                bytecode.push_back(mode);
            }
            for (auto &op : ops)
            {
                bytecode.push_back(op);
            }
            // std::cout << line << std::endl;
        }
    }

    sourceFile.close();
    std::ofstream outputFile(argv[2], std::ios::binary | std::ios::out);

    if (!outputFile)
    {
        std::cerr << "Failed to open file: " << argv[2] << std::endl;
        return -1;
    }
    // First write the bytecode size and segment then the data size and segment
    WORD code_size = static_cast<WORD>(bytecode.size());
    // print the bytecode
    std::cout << "Code size: " << std::hex << std::setfill('0') << std::setw(sizeof(WORD)*2) << code_size << " words" << std::endl;
    outputFile.write(reinterpret_cast<const char *>(&code_size), sizeof(code_size));
    for (int byte : bytecode)
    {
        WORD v = byte;
        std::cout << std::hex << std::setfill('0') << std::setw(sizeof(WORD)) << v << " ";
        outputFile.write(reinterpret_cast<const char *>(&v), sizeof(v));
    }

    WORD data_size = static_cast<WORD>(data_segments.size());
    outputFile.write(reinterpret_cast<const char *>(&data_size), sizeof(data_size));
    
    // I am stupid, the unordered_map does not guarantee order, so we need to write the data segments in the order they were defined
    for(ADDR addr = DATA_START; addr < dp; addr++)
    {
        WORD v = memory[addr];
        std::cout << std::hex << std::setfill('0') << std::setw(sizeof(WORD)) << v << " ";
        outputFile.write(reinterpret_cast<const char *>(&v), sizeof(v));
    }

    // Writing empty heap???
    // Should we write the heap and stack segments as well?
    // Nope, they will be initialized at runtime

    outputFile.close();
    std::cout << "Binary bytecode written to " << argv[2] << std::endl;
    return 0;
}
