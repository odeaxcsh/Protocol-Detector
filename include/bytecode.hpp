#if !defined (BYTECODE_HPP)
#define BYTECODE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>

#include "elicitation_utils.hpp"

typedef struct {
    int bit = 0,
        byte = 0;

    void inc_bit()
    {
        bit = (bit + 1) % 8;
        if(bit == 0)
            ++byte;
    }

    void inc_byte()
    {
        ++byte;
    }

} Index;

typedef struct {
    bool match = false;

    Index index = {0, 0};

    int pc = 0;

    std::map<std::string, std::vector<unsigned char>> variables;
    std::map<std::string, Index> vars_last_update;

    std::map<std::string, int> iteration_counter;
} machine_state;

enum class Opcode
{
    Match = 1,
    Bit_match,
    Jump,
    Split,
    Save_start,
    Save_end,
    Add_iterate,
    Return,
    Halt
};

struct Bytecode
{
    Opcode opcode;
    std::vector<std::string> args;

    Bytecode(Opcode opcode, std::vector<std::string> args) : args{args}
    {
        this->opcode = opcode;
    }

    virtual void execute(machine_state &)
    {

    }
};

struct Bytecode_add_iterate : public Bytecode
{
    virtual void execute(machine_state &);

    Bytecode_add_iterate(Opcode opcode, std::vector<std::string> args, const std::string &limit_expr) : Bytecode{opcode, args}
    {
        this->limit_expr = Formula_parser().parse<int>(limit_expr);
    }

private:
    Base_expression<int> *limit_expr = nullptr;
};

struct Pattern : std::vector<Bytecode *>
{
    Pattern() = default;
    Pattern(const Pattern &) = default;

    Pattern(std::initializer_list<Bytecode *> v) : std::vector<Bytecode *>(v)
    {

    }
};

std::ostream &operator<<(std::ostream&, const Opcode&);
std::ostream &operator<<(std::ostream&, const Bytecode&);

extern const std::string opcodes_name_map [];

#endif
