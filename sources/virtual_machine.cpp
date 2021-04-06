#include "definitions.hpp"
#include "virtual_machine.hpp"

#include <cstring>
#include <exception>
#include <iostream>

int i_th_bit(int value, int pos)
{
    return (value >> pos) & (1);
}

const std::string VM::counter_name = "X";

std::vector<char> to_string(Index start, Index end, const char *text)
{
    std::vector<char> result;

    int byte_s = start.byte,
        bit_s = start.bit;
    int byte_e = end.byte,
        bit_e = end.bit;

    for(int i = byte_s; i < byte_e; ++i) {
        result.push_back(text[i]);
    }

    result[0] &= (-1 << bit_s);
    result[result.size() -1] &= (-1 >> (8 - bit_e));

    return result;
}

VM::VM(const Pattern &pattern) : VM()
{
    this->load_code(pattern);
}

VM &VM::load_code(const Pattern &pattern)
{
    this->code = pattern;
    return *this;
}

std::map<std::string, std::vector<char>> VM::run(const char * text, int len)
{
    this->stack = {};
    this->text = text;

    machine_state initial_state;
    this->stack.push(initial_state);

    while(!stack.empty()) {
        machine_state state = stack.top();
        Bytecode instruction;

        if(state.pc < this->code.size())
            instruction = this->code[state.pc];
        else break;

        stack.pop();

        bool failed = false;
        switch (instruction.opcode) {
            case Opcode::Match:
                if(not match(state))
                    failed = true;
                break;

            case Opcode::Bit_match:
                if(not bit_match(state))
                    failed = true;
                break;

            case Opcode::Jump:
                jump(state);
                break;

            case Opcode::Split:
                split(state);
                break;

            case Opcode::Save_end:
                save_end(state);
                break;

            case Opcode::Save_start:
                save_start(state);
                break;

            case Opcode::Add_iterate:
                add_iterate(state);
                break;

            default:
                throw std::runtime_error("Unknown Instruction");
        }

        if(not failed) {
            stack.push(state);
        }
    }

    if(stack.empty()) {
        return {{"Failed", {}}};
    }

    machine_state state = stack.top();
    std::map<std::string, std::vector<char>> result;
    std::vector<std::string> iterations;

    for(const auto &[key, value] : state.var_starts) {

        if((key.length() > counter_name.length()) and
            (0 == key.compare(key.length() - counter_name.length(), counter_name.length(), counter_name)))
            continue;

        const auto end = state.var_ends.find(key);
        if(end != state.var_ends.end()) {
            result[key] = (to_string(value, end->second, text));
        }
    }

    return result;
}

bool VM::match(machine_state &state)
{
    int ibit = state.index.bit,
        ibyte = state.index.byte;

    Bytecode instruction = code[state.pc];

    if(ibit != 0) {
        throw std::runtime_error("Bit index must be zero on string matching");
    }

    bool any = (instruction.args[0] == ANY_STRING);
    int len = any ? std::stoi(instruction.args[1]) : instruction.args[0].size();

    int i = 0;
    for(; i < len; ++i) {
        if ((any and text[ibyte]) or ((not any) and (instruction.args[0][i] == text[ibyte])))
            ++ibyte;
        else break;
    }

    if(i != len)
        return false;

    ++state.pc;
    state.index.byte = ibyte;
    state.index.bit = ibit;
    return true;
}

bool VM::bit_match(machine_state &state)
{
    Index index = state.index;
    Bytecode instruction = code[state.pc];

    bool any = (instruction.args[0] == ANY_STRING);
    int len = any ? std::stoi(instruction.args[1]) : instruction.args[0].size() - 1;

    int i = 0;
    for(; i < len; ++i) {
        if((any && text[index.byte + (index.bit > 0)]) or
        ((not any) and ((instruction.args[0][i + 1] - '0') == i_th_bit(text[index.byte], index.bit))))
            index.inc_bit();
        else {
            std::cout << i << " " << i_th_bit(text[index.byte], index.bit);
            break;
        }
    }

    if(i != len)
        return false;

    ++state.pc;
    state.index = index;
    return true;
}

void VM::jump(machine_state &state)
{
    Bytecode instruction = code[state.pc];
    state.pc = state.pc + std::stoi(instruction.args[0]);
}

void VM::split(machine_state &state)
{
    machine_state new_thread = state;
    Bytecode instruction = code[state.pc];
    new_thread.pc = state.pc + std::stoi(instruction.args[1]);
    state.pc = state.pc + std::stoi(instruction.args[0]);
    stack.push(new_thread);
}

void VM::save_start(machine_state &state)
{
    Bytecode instruction = code[state.pc];
    state.var_starts[instruction.args[0]] = state.index;
    ++state.pc;
}

void VM::save_end(machine_state &state)
{
    Bytecode instruction = code[state.pc];
    state.var_ends[instruction.args[0]] = state.index;
    ++state.pc;
}

void VM::add_iterate(machine_state &state)
{
    Bytecode instruction = code[state.pc];

    //returns index of where variable iteration counter placed
    auto counter_name = [this](const std::string &name) -> std::string {
        return name + this->counter_name;
    };

    //Set counters at VAR STARTS NAME
    for(const auto &name : instruction.args) {
        if(state.var_starts.find(counter_name(name)) == state.var_starts.end())
            state.var_starts[counter_name(name)] = {0, 0};
    }

    for(const auto &name : instruction.args) {
        int counter_ = state.var_starts[counter_name(name)].byte;
        std::string counter = std::to_string(counter_);

        state.var_starts[name + counter] = state.var_starts[name];
        state.var_ends[name + counter] = state.var_ends[name];

        state.var_starts[counter_name(name)].inc_byte();
    }

    ++state.pc;
}
