#include "bytecode.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& out, const Opcode& opcode)
{
    return out << opcodes_name_map[(int)opcode];
}

std::ostream& operator<<(std::ostream& out, const Bytecode& bytecode)
{
    out << bytecode.opcode << " ";

    for(auto arg : bytecode.args) {
        out << arg << " ";
    }

    return out;
}

const std::string opcodes_name_map[]
{
    "Unknown",
    "Match",
    "Bit_match",
    "Jump",
    "Split",
    "Save-Start",
    "Save-end",
    "Add-iterate",
    "Return",
    "Halt",
};
