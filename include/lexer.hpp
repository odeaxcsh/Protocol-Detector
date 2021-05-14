#if !defined(LEXER_HPP)
#define LEXER_HPP

#include <map>
#include <string>
#include <vector>

class Lexer;

#include "definitions.hpp"

enum class Token_type {
    Save_match,
    Save_binary,
    Open_paran,
    Close_paran,
    Iteration_start,
    Iteration_stop,
    Or_operator,
    String,
    Integer,

    Sum_op,
    Div_op,
    Mul_op,
    Sub_op,
    Les_op,
    gre_op,
    eq_op,
};

extern std::map<char, Token_type> defalut_operators;

typedef struct {
    Token_type type;
    std::string value = "";
} Token;

bool operator==(const Token &, const Token &);
bool operator!=(const Token &, const Token &);
bool operator<(const Token &, const Token &);

class Lexer
{
public:
    Lexer(const char *, std::map<char, Token_type> = defalut_operators);

    char peek();

    std::string get_binary();
    std::string get_int();
    std::string get_str();
    std::string get_name();
    std::string get_operand();
    std::string get_expression();

    bool is_eot(const std::string &);

    Token get_token(bool=false);

private:
    template <typename lambda>
    std::string get_till(lambda);

    char global_buffer[BUFFERS_MAX_LEN];
    const char *text_ptr;
    const char *text;

    std::map<char, Token_type> tokens;
    static const std::string EOT_sign; //End of Text
};

#endif
