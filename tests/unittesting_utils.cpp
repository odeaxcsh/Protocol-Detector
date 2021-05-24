#include "code_generation_utils.hpp"
#include "unittesting_utils.hpp"

#include <string.h>
#include <vector>

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

std::vector<unsigned char> to_char_vec(const char *text)
{
    return std::vector<unsigned char>(
        text,
        text + strlen(text)
    );
}

std::vector<unsigned char> operator "" _v(const char* text, size_t)
{
    return to_char_vec(text);
}