#if !defined (BYTECODE_HPP)
#define BYTECODE_HPP

#include <iostream>
#include <string>
#include <vector>

enum class Opcode
{
    Match = 1,
    Bit_match,
    Match_num,
    Jump,
    Split,
    Save_start,
    Save_end,
    Add_iterate,
};

typedef struct
{
    Opcode opcode;
    std::vector<std::string> args;
} Bytecode;

typedef std::vector<Bytecode> Pattern;

std::ostream &operator<<(std::ostream&, const Opcode&);
std::ostream &operator<<(std::ostream&, const Bytecode&);

extern const std::string opcodes_name_map [];

#endif
