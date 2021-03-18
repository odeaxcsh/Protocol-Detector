#if !defined (PATTERN_PARSER_HPP)
#define PATTERN_PARSER_HPP

#include "bytecode.hpp"
#include "definitions.hpp"
#include "lexer.hpp"

class Pattern_parser
{

public:
    Pattern_parser(Lexer &lexer);
    Pattern parse();

private:
    Lexer &lexer;

    Pattern iteration(bool=false);
    Pattern match_string();
    Pattern save_string();
    Pattern save_binary();

    void except_paran_open();
    void except_paran_close();
};

#endif
