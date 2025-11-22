#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <iomanip>
#include "memory.hpp"
#include "mapping.hpp"
#include "utils.hpp"

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

            // Remove the comments
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

            // Skip empty lines (whitespace only or comments)
            if (tokens.empty())
                continue;

            // Doing the actual thing
            std::string instr = tokens.at(0);
            auto instr_info = instr_map.find(instr);

            if (instr_info == instr_map.end())
            {
                std::cerr << "Error: Unknown instruction " << instr << " at line " << line_no << std::endl;
                return -1;
            }

            tokens.erase(tokens.begin());
            std::vector<OperandType> ops_t;
            std::vector<WORD> ops;
            // Number of operands check
            if (tokens.size() < static_cast<size_t>(instr_info->second.min_operands) ||
                tokens.size() > static_cast<size_t>(instr_info->second.max_operands))
            {
                std::cerr << "Error: Incorrect number of operands for instruction " << instr << " at line " << line_no << std::endl;
                return -1;
            }

            // Handling no operand case
            if (tokens.size() == OperandType::NONE)
            {
                ops_t.push_back(NONE);
            }
            else
            {

                for (auto &op_str : tokens)
                {
                    if (is_register(op_str, register_map))
                    {
                        ops_t.push_back(REGISTER);
                        ops.push_back(register_map[op_str]);
                    }
                    else if (is_immediate(op_str))
                    {
                        ops_t.push_back(INDIRECT);
                        ops.push_back(static_cast<WORD>(std::stoi(op_str)));
                    }
                    else if (is_memory(op_str, data_segments))
                    {
                        ops_t.push_back(MEMORY);
                        // Extract content between brackets
                        std::string inner = op_str.substr(1, op_str.size() - 2);
                        
                        // Check if it's a labeled address
                        auto it = data_segments.find(inner);
                        if (it != data_segments.end())
                        {
                            ops.push_back(it->second);
                        }
                        // Otherwise it's a direct numeric address
                        else if (is_number(inner))
                        {
                            ops.push_back(static_cast<WORD>(std::stoi(inner)));
                        }
                        else
                        {
                            std::cerr << "Error: Invalid memory reference " << op_str << " at line " << line_no << std::endl;
                            return -1;
                        }
                    }
                    else
                    {
                        std::cerr << "Error: Invalid operand " << op_str << " at line " << line_no << std::endl;
                        return -1;
                    }
                }
            }

            bytecode.push_back(instr_info->second.opcode); // Push the Instruction
            if(ops_t.at(0) == NONE){
                bytecode.push_back(NONE);

            }
            for (size_t idx = 0; idx < ops.size(); ++idx)
            {
                bytecode.push_back(ops_t.at(idx));
                bytecode.push_back(ops.at(idx));
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
    outputFile.write(reinterpret_cast<const char *>(&code_size), sizeof(code_size));
    for (int byte : bytecode)
    {
        WORD v = byte;
        outputFile.write(reinterpret_cast<const char *>(&v), sizeof(v));
    }

    WORD data_size = static_cast<WORD>(data_segments.size());
    outputFile.write(reinterpret_cast<const char *>(&data_size), sizeof(data_size));

    // I am stupid, the unordered_map does not guarantee order, so we need to write the data segments in the order they were defined
    for (ADDR addr = DATA_START; addr < dp; addr++) // a good'ol for loop
    {
        WORD v = memory[addr];
        outputFile.write(reinterpret_cast<const char *>(&v), sizeof(v));
    }

    // Writing empty heap???
    // Should we write the heap and stack segments as well?
    // Nope, they will be initialized at runtime

    outputFile.close();
    std::cout << "Binary bytecode written to " << argv[2] << std::endl;

    // DEBUG
    // for (auto &v : bytecode)
    // {
    //     std::cout << std::setw(4) << v << " ";
    // }

    return 0;
}
