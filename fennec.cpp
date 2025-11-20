#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "opcodes.hpp"
#include "utils.hpp"

int main(int argc, char const *argv[])
{
    std::unordered_map<std::string, int> instr_map = {
        {"PSH", PSH},
        {"POP", POP},
        {"ADD", ADD},
        {"OUT", OUT},
        {"HLT", HLT},

    };
    std::unordered_map<std::string, int> registor_map = {
        {"RA", RA},
        {"RB", RB},
        {"RC", RC},
        {"RD", RD},
    };
    if (argc < 2)
    {
        std::cerr << "Usage: fennec <file.fnc>" << std::endl;
        return 1;
    }
    std::ifstream sourceFile(argv[1]);
    if (!sourceFile.is_open())
    {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }
    std::vector<int> bytecode;
    std::string line;
    int line_no = 1;
    while (std::getline(sourceFile, line))
    {
        std::istringstream iss(line);
        std::string token;
        if (line.empty() || line.at(0) == ';')
            continue;
        while (iss >> token)
        {
            auto it = instr_map.find(token);
            auto reg = registor_map.find(token);

            if (it == instr_map.end() && reg == registor_map.end() && !is_number(token))
            {
                std::cerr << "Unknown Instruction or Register or Invalid value \"" << token << "\" at line " << line_no << std::endl;
                continue;
            }
            else
            {
                if (it != instr_map.end())
                {
                    bytecode.push_back(it->second);
                }
                else if (reg != registor_map.end())
                {
                    bytecode.push_back(reg->second);
                }
                else if (is_number(token))
                {
                    int value = std::stoi(token);
                    bytecode.push_back(value);
                }
            }
        }
        line_no++;
    }
    std::cout << "const int program[] = {";
    for (size_t i = 0; i < bytecode.size(); ++i)
    {
        std::cout << bytecode[i];
        if (i + 1 < bytecode.size())
            std::cout << ", ";
    }
    std::cout << "};\n";

    sourceFile.close();
    return 0;
}
