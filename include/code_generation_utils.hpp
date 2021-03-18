#if !defined(CODE_GENERATION_UTILS)
#define CODE_GENERATION_UTILS

#include "bytecode.hpp"

struct Code_generation_utils
{
    Pattern match_string_pattern(const std::string &);
    Pattern match_binary_pattern(const std::string &);

    Bytecode match_any_char();
    Pattern any_string_pattern();
    Pattern execute_concatanation_operator(const std::vector<Pattern> &);
    Pattern execute_iteration(const Pattern &, const std::vector<std::string> &);
    Pattern execute_or_operator(const Pattern &, const Pattern &);
    Pattern execute_save_variable(const Pattern &, const std::string &);
    Pattern execute_kleene_start(const Pattern &);
};

extern struct Code_generation_utils code_generation_utils;

#endif
