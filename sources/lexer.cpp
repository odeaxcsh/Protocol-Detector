#include "definitions.hpp"
#include "lexer.hpp"

#include <cctype>
#include <cstring>
#include <map>
#include <string>
#include <vector>

const std::string Lexer::EOT_sign = "EOT";

bool operator==(const Token &first, const Token &second)
{
    return (first.value == second.value) and ((int)first.type == (int)second.type);
}

bool operator!=(const Token &first, const Token &second)
{
    return not((first.value == second.value) and ((int)first.type == (int)second.type));
}

bool operator<(const Token &first, const Token &second)
{
    if((int)first.type == (int)second.type)
        return first.value < second.value;
    else return (int)first.type < (int)second.type;
}

std::map<char, Token_type> defalut_operators =
{
    {SAVE_MATCH_SIGN, Token_type::Save_match},
    {SAVE_BINARY_SIGN, Token_type::Save_binary},
    {ITERATION_START_SIGN, Token_type::Iteration_start},
    {ITERATION_STOP_SIGN, Token_type::Iteration_stop},
    {'|', Token_type::Or_operator},
    {')', Token_type::Close_paran},
    {'(', Token_type::Open_paran}
};

Lexer::Lexer(const char *text, std::map<char, Token_type> operators_map)
{
    this->text = text;
    this->text_ptr = text;
    this->tokens = operators_map;
}

char Lexer::peek()
{
    return *text_ptr;
}

template <typename lambda>
std::string Lexer::get_till(lambda condition)
{
    if(*text_ptr == '\0') {
        return this->EOT_sign;
    }

    bool is_first = true;
    const char *begin = text_ptr;
    while(*text_ptr and condition(*text_ptr, is_first)){
        is_first = false;
        ++text_ptr;
    }

    int len = text_ptr - begin;
    std::strncpy(global_buffer, begin, len);
    global_buffer[len] = '\0';

    return std::string(global_buffer);
}

std::string Lexer::get_int()
{
    return get_till(
        [](char c, bool is_first) -> bool {
            return std::isdigit(c);
        });
}

std::string Lexer::get_binary()
{
    return get_till(
        [](char c, bool is_first) -> bool {
            return std::isdigit(c) or
                (is_first and c == 'b');
        });
}

std::string Lexer::get_name()
{
    return get_till(
        [](char c, bool is_first) -> bool {
            return std::isalpha(c) or
                (c == '_') or
                (not is_first and std::isdigit(c));
        });
}

std::string Lexer::get_str()
{
    return get_till(
        [this](char c, bool is_first){
            return (this->tokens.find(c) == this->tokens.end());
        });
}

std::string Lexer::get_operand()
{
    get_till([](char c, bool is_first) {
        return (c == ' ') or (c == '\t');
    });

    return get_till(
        [this](char c, bool is_first){
            return (this->tokens.find(c) == this->tokens.end());
        });
}

bool Lexer::is_eot(const std::string &str)
{
    return EOT_sign == str;
}

Token Lexer::get_token(bool spaceless)
{
    const char *begin = text_ptr;
    if(tokens.find(*begin) == tokens.end()) {
        return Token {
            Token_type::String,
            spaceless ? get_operand() : get_str()
        };
    } else {
        ++text_ptr;
        return Token {
            this->tokens[*begin],
            std::string(1, *begin)
        };
    }
}
