#include "lexer.hpp"
#include <iostream>

int main()
{
    const char *text = "this is test and $this(is not)";
    Lexer lexer(text);

    std::string s;
    Token t;

    do {
        t = lexer.get_token();
        getchar();
        std::cout << t.value << std::endl;
    } while (true);
}
