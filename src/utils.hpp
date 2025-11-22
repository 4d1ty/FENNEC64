#include <string>
#pragma once

bool is_number(const std::string &s)
{
    if (s.empty()) return false;

    size_t start = 0;
    if (s[0] == '-') start = 1; // allow leading minus

    for (size_t i = start; i < s.size(); ++i)
    {
        if (!std::isdigit(s[i]))
            return false;
    }

    return true;
}
