#include "code_generation_utils.hpp"

bool operator== (const Bytecode &first, const  Bytecode &second)
{
    if((int)first.opcode != (int)second.opcode)
        return false;
    if(first.args.size() != second.args.size())
        return false;
    for(int i = 0; i < second.args.size(); ++i)
        if(first.args[i] != second.args[i])
            return false;
    return true;
}

bool operator== (const Pattern &first, const Pattern &second)
{
    if(first.size() != second.size())
        return false;
  
    for(int i = 0; i < first.size(); ++i)
        if(!(*first[i] == *second[i]))
            return false;
    return true;
}
