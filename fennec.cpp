#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

#include "opcodes.hpp"
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

bool is_memory(const std::string &s, const std::unordered_map<std::string, int32_t> &data_map)
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
        std::cerr << "Usage: fennec <file.fnc> <output.bin>" << std::endl;
        return 1;
    }
    std::ifstream sourceFile(argv[1]);
    if (!sourceFile.is_open())
    {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }
    std::vector<int32_t> bytecode;
    std::unordered_map<std::string, int32_t> data_segments;
    int32_t memory[] = {0};
    int32_t addr = 0;

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
                        data_segments[token] = addr;
                        memory[addr] = value;
                        addr++;
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
            while(iss >> token){
                tokens.push_back(token);
            }

            // Doing the actual thing
            std::string instr = tokens.at(0);
            auto info = instr_map.find(instr);
            int32_t mode;
            std::vector<int32_t> ops;
            if(tokens.size() - 1 > info->second.operands){
                std::cerr << "Error: Too many operands for instruction " << instr << " at line " << line_no << std::endl;
                return -1;
            }
            if(info == instr_map.end()){
                std::cerr << "Error: Unknown instruction " << instr << " at line " << line_no << std::endl;
                return -1;
            }
            if(info->second.has_mode){
                if(tokens.size() == 1){
                    mode = 0; // Default mode
                } else {
                    std::string op = tokens.at(1);
                    if(is_register(op, register_map)){
                        mode = 1 ; // Register mode
                        ops.push_back(register_map[op]);
                    } else if(is_number(op)){
                        mode = 2; // Immediate mode
                        ops.push_back(std::stoi(op));
                    } else if(is_memory(op, data_segments)){
                        mode = 3; // Memory mode
                        ops.push_back(data_segments.find(op)->second);
                    } else {
                        std::cerr << "Error: Invalid operand " << op << " at line " << line_no << std::endl;
                        return -1;
                    }
                }
            }
            bytecode.push_back(info->second.opcode);
            if(info->second.has_mode){
                bytecode.push_back(mode);
            }
            for(auto &op : ops){
                bytecode.push_back(op);
            }
            // std::cout << line << std::endl;
        }
    }

    sourceFile.close();

    std::cout << sizeof(bytecode) << std::endl;
    std::cout << sizeof(memory) << std::endl;
    std::cout << (memory[1]) << std::endl;

    std::ofstream outputFile(argv[2], std::ios::binary | std::ios::out);

    if (!outputFile)
    {
        std::cerr << "Failed to open file: " << argv[2] << std::endl;
        return -1;
    }

    for (int byte : bytecode)
    {
        int32_t v = byte;
        outputFile.write(reinterpret_cast<const char *>(&v), sizeof(v));
    }
    outputFile.close();
    std::cout << "Binary bytecode written to " << argv[2] << std::endl;
    return 0;
}
