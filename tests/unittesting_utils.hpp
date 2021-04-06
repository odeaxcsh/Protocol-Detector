#if !defined(UNITTESTING_UTILS)
#define UNITTESTING_UTILS

#include "code_generation_utils.hpp"
#include "definitions.hpp"

bool operator== (const Bytecode &first, const  Bytecode &second);
bool operator== (const Pattern &first, const Pattern &second);

#endif
