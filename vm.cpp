#include <iostream>
#include "opcodes.hpp"

int fetch();
void eval(int);

int stack[256];
int sp = -1;


int ip = 0;

const int program[] = {
    PSH,
    67,
    PSH,
    21,
    ADD,
    OUT,
    HLT,
};

bool running = true;

int main(int argc, char const *argv[])
{
    while (running)
    {
        eval(fetch());
    }
    return 0;
}

int fetch()
{
    return program[ip++];
}

void eval(int instruction)
{
    switch (instruction)
    {
    case HLT:
        running = false;
        break;
    case PSH:
        stack[++sp] = fetch();
        break;
    case POP:
        break;
    case ADD:
    {

        int sum = 0;
        while (sp > -1)
        {
            sum += stack[sp--];
        }
        stack[++sp] = sum;
        break;
    }
    case OUT:
    {
        std::cout << stack[sp] << std::endl;
        break;
    }
    default:
        break;
    }
}
