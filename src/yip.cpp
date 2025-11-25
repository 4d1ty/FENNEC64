#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <iomanip>

#include "vixen/memory.hpp"
#include "yip/mapping.hpp"
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

    Section currentSection = TEXT;
    std::string line;
    int line_no = 0;

    std::vector<std::vector<std::string>> code_source; // Only have the codes
    ADDR currentProcedureAddress = procedure_map["MAIN"];
    std::unordered_map<std::string, ADDR> data_segments;

    // Tokenizing the source
    while (std::getline(sourceFile, line))
    {
        ++line_no;
        // Remove the comments
        auto comment_position = line.find(";");
        if (comment_position != std::string::npos)
        {
            line = line.substr(0, comment_position);
        }
        // Skip empty lines (whitespace only or comments)
        if (line.empty() || line.size() == 0)
        {
            continue;
        }
        std::istringstream iss(line);
        std::string word;
        std::vector<std::string> code_tokens;
        std::vector<std::string> data_tokens;
        if (line.ends_with(".text"))
        {
            currentSection = TEXT;
            continue;
        }
        else if (line.ends_with(".data"))
        {
            currentSection = DATA;
            continue;
        }
        while (iss >> word)
        {
            if (!word.empty() || word.size() != 0)
            {
                if (currentSection == TEXT)
                {
                    code_tokens.push_back(word);
                    if (is_instruction(word, instr_map))
                    {
                        auto it = instr_map[word];
                        currentProcedureAddress += 1 + (it.max_operands - it.min_operands) + (it.max_operands + it.min_operands);
                    }
                    if (is_label(word))
                    {
                        procedure_map[word.substr(0, word.size() - 1)] = currentProcedureAddress;
                        continue;
                    }
                }
            }
        }
        code_source.push_back(code_tokens);

        if (currentSection == DATA)
        {
            std::istringstream data_iss(line);
            std::string token;
            WORD value;
            while (data_iss >> token)
            {
                if (is_label(token))
                {
                    token.pop_back();
                    if (data_iss >> value)
                    {
                        data_segments[token] = dp;
                        memory[dp] = value;
                        dp++;
                    }
                }
            }
        }
    }
    sourceFile.close();

    // 2nd pass 💀
    std::vector<WORD> bytecode;

    // .text section
    for (auto &line : code_source)
    {
        if (line.size() != 0 && !is_label(line.at(0)))
        {

            std::vector<OperandType> ops_t;
            std::vector<WORD> ops;
            std::string token = line.at(0);
            line.erase(line.begin());
            if (is_instruction(token, instr_map))
            {
                auto inst = instr_map[token];
                if (line.size() < inst.min_operands || line.size() > inst.max_operands)
                {
                    std::cerr << "Error: Incorrect number of operands for instruction " << token << std::endl;
                    return -1;
                }
                bytecode.push_back(inst.opcode);
                if (line.size() == 0)
                {
                    bytecode.push_back(NONE);
                    bytecode.push_back(NONE);
                }
                for (auto &operand : line)
                {
                    if (is_register(operand, register_map))
                    {
                        bytecode.push_back(REGISTER);
                        bytecode.push_back(register_map[operand]);
                    }
                    else if (is_number(operand))
                    {
                        bytecode.push_back(INDIRECT);
                        bytecode.push_back(static_cast<WORD>(std::stoi(operand)));
                    }
                    else if (is_memory(operand, data_segments))
                    {
                        bytecode.push_back(MEMORY);
                        ops_t.push_back(MEMORY);
                        // Extract content between brackets
                        std::string inner = operand.substr(1, operand.size() - 2);

                        // Check if it's a labeled address
                        auto it = data_segments.find(inner);
                        if (it != data_segments.end())
                        {
                            bytecode.push_back(it->second);
                        }
                        // Otherwise it's a direct numeric address
                        else if (is_number(inner))
                        {
                            bytecode.push_back(static_cast<WORD>(std::stoi(inner)));
                        }
                        // Otherwise it's a value from the register
                        else if (is_register(inner, register_map))
                        {
                            bytecode.push_back(register_map[inner]);
                        }
                        else
                        {
                            std::cerr << "Error: Invalid memory reference " << operand << std::endl;
                            return -1;
                        }
                    }
                    else if (is_procedure(operand, procedure_map))
                    {
                        bytecode.push_back(PROCEDURE);
                        bytecode.push_back(procedure_map[operand]);
                    }
                    else
                    {
                        std::cerr << "Error: Invalid operand " << operand << std::endl;
                        return -1;
                    }
                }
            } else {
                std::cerr << "Error: Unknown instruction " << token << std::endl;
                return -1;
            }
        }
    }


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
    //     std::cout << std::setw(4) << std::setfill('0') << v << " ";
    // }

    return 0;
}
