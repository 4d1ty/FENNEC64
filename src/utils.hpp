#pragma once
#include <string>
#include "cpu.hpp"

bool is_number(const std::string &s)
{
    if (s.empty())
        return false;

    size_t start = 0;
    if (s[0] == '-')
        start = 1; // allow leading minus

    for (size_t i = start; i < s.size(); ++i)
    {
        if (!std::isdigit(s[i]))
            return false;
    }

    return true;
}

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
    if(s.size() >= 2 && s.front() == '[' && s.back() == ']')
    {        
        // Extract content between brackets
        std::string inner = s.substr(1, s.size() - 2);
        
        auto it = data_map.find(inner);
        if (it != data_map.end())
        {
            return true;
        }

        if(is_number(inner)){
            return true;
        }

    }
    return false;
}

bool is_immediate(const std::string &s)
{
    return is_number(s);
}

