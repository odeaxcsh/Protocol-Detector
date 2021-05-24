#include "bytecode.hpp"
#include "virtual_machine.hpp"

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

void Bytecode_add_iterate::execute(machine_state &state)
{
    ++state.pc;


    bool valid_iteration = false;
    // In counted iteration there is but cause implementation is like this
    /*
    Add iteration if condition
    matching and capturing 
    jump
    */
    // so there is an extra iteration with empty values which must be ignored
    for(const auto &name : args) {
        if(state.variables.find(name) != state.variables.end()) {
            valid_iteration = true;
            break;
        }
    }

    if(not valid_iteration)
        return;

    auto get_iteration_couter = [](int pc) {
        return "__iterator_counter_" + std::to_string(pc);
    };

    int iteration_counter = 0;

    if(state.iteration_counter.find(get_iteration_couter(state.pc)) != state.iteration_counter.end())
        iteration_counter = state.iteration_counter[get_iteration_couter(state.pc)];

    state.iteration_counter[get_iteration_couter(state.pc)] = iteration_counter + 1;
    
    //returns index of where variable iteration counter placed
    auto get_item_name = [this](const std::string &name, int count) -> std::string {
        return name + "[" + std::to_string(count) + "]";
    };

    //Set counters at VAR STARTS NAME
    for(const auto &name : args) {
        int counter = 
            state.iteration_counter.find(name) == state.iteration_counter.end() ? 
            0 : state.iteration_counter[name];
        
        std::string this_item_name = get_item_name(name, counter);

        state.variables[this_item_name] = state.variables[name];
        state.vars_last_update.erase(name);
        state.variables.erase(name);

        state.iteration_counter[name] = counter + 1;
    }
    
    if(limit_expr != nullptr and limit_expr->eval(state.variables) == iteration_counter + 1)
        ++state.pc;
}