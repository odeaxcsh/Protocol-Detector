/*#include "print_format.hpp"
#include "lexer.hpp"
#include "definitions.hpp"

#include <map>
#include <stdexcept>

Base_print_format *Base_print_format::parse(const std::string &expr)
{
    static const std::map<char, Token_type> defalut_operators =  {
        {SAVE_MATCH_SIGN, Token_type::Save_match},
        {ITERATION_START_SIGN, Token_type::Iteration_start},
        {ITERATION_STOP_SIGN, Token_type::Iteration_stop},
        {'|', Token_type::Or_operator},
        {')', Token_type::Close_paran},
        {'(', Token_type::Open_paran}
    };

    bool in_loop = false;
    Lexer lexer(expr.c_str(), defalut_operators);
    std::vector<Base_print_format *> this_level;
    Root_print_format *root = new Root_print_format();
    
    Token token = lexer.get_token();

    while(lexer.is_eot(token.value)) {
        switch(token.type) {
            case Token_type::Iteration_start:
                if(in_loop) {
                    throw std::runtime_error("nested loop in format print!");
                }
                in_loop = true;
                this_level = {};
                break;
            case Token_type::Save_match: {
                std::string name = lexer.get_name();
                if(in_loop) {

                }
            }

        }

        token = lexer.get_token(); 
    }
    return nullptr;
}
*/