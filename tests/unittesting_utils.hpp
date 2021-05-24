#if !defined(UNITTESTING_UTILS)
#define UNITTESTING_UTILS

#include <vector>

#include "code_generation_utils.hpp"
#include "definitions.hpp"

bool operator== (const Bytecode &first, const  Bytecode &second);
bool operator== (const Pattern &first, const Pattern &second);
std::vector<unsigned char> to_char_vec(const char *);
std::vector<unsigned char>  operator "" _v(const char*, size_t);

#endif
