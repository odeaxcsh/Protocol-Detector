#if !defined(LEXER_HPP)
#define LEXER_HPP

#include <map>
#include <string>
#include <vector>

#include "definitions.hpp"

enum class Token_type {
    Save_match,
    Save_binary,
    Open_paran,
    Close_paran,
    Iteration,
    Or_operator,
    String,
    Integer,
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

    std::string get_int();
    std::string get_str();
    std::string get_name();
    bool is_eot(const std::string &);

    Token get_token();

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
