#include "elicitation_utils.hpp"

#include <map>
#include <stack>
#include <vector>
#include <string>
#include <functional>
#include <type_traits>
#include <iostream>

std::string trim(const std::string& str,
                 const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return "";

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

int get_int(const std::vector<unsigned char> &bytes)
{
    int value = 0;
    for(auto byte : bytes) {
        value = value * 256 + byte;
    }
    return value;
}

std::string get_str(const std::vector<unsigned char> &bytes)
{
    std::string str;
    for(auto byte : bytes)
        str += static_cast<char>(byte);
    return str;
}

const std::map<char, Token_type> Formula_parser::defalut_operators =
{
    {'*', Token_type::Mul_op},
    {'+', Token_type::Sum_op},
    {'/', Token_type::Div_op},
    {')', Token_type::Close_paran},
    {'(', Token_type::Open_paran}
};

const std::map<Token_type, int> Formula_parser::priority = 
{ 
    {Token_type::Div_op, 1},
    {Token_type::Mul_op, 1},
    {Token_type::Sum_op, 2},
    {Token_type::Open_paran, 3},
};

template<typename dtype>
Base_expression<dtype> *Formula_parser::parse(std::string expression) const
{
}

template <>
Base_expression<int> *Formula_parser::parse(std::string expression) const
{
    expression = "(" + expression + ")";
    Lexer lexer = Lexer(expression.c_str(), defalut_operators);
    std::stack<Base_expression<int> *> vars_stack;
    std::stack<Token_type> op_stack;

    auto pop = [](auto &stack) { 
            auto &&a = stack.top();
            stack.pop();
            return a;
        };

    auto handle = [&] (Token_type &tok) {
        if(tok == Token_type::Open_paran)
            return;

        auto &&a = pop(vars_stack), &&b = pop(vars_stack);
        switch(tok)
        {

            case Token_type::Div_op:
                vars_stack.push(new Formula_parser::Int_Div(a, b));
                break;

            case Token_type::Mul_op:
                vars_stack.push(new Formula_parser::Int_Mul(a, b));
                break;

            case Token_type::Sum_op:
                vars_stack.push(new Formula_parser::Int_Sum(a, b));
                break;

            default:
                throw "Unexpected token in prser.handle!";
        }
    };

    Token token = lexer.get_token();

    while(not (lexer.is_eot(token.value))) {
        if(token.type == Token_type::String) {
            std::string spaceless = trim(token.value);
            if(spaceless != "")
                vars_stack.push(new Ref_Value<int>(spaceless));
        } else if(token.type == Token_type::Close_paran) {
            Token_type tok;
            do {
                tok = pop(op_stack);
                handle(tok);
            } while(tok != Token_type::Open_paran);
        } else if(token.type == Token_type::Open_paran){ 
            op_stack.push(token.type);
        } else {
            while(op_stack.size() and Formula_parser::priority.at(token.type) >= Formula_parser::priority.at(op_stack.top())) {
                auto &&tok = pop(op_stack);
                handle(tok);
            }
            op_stack.push(token.type);
        }
        token = lexer.get_token();
    }

    return pop(vars_stack);
}