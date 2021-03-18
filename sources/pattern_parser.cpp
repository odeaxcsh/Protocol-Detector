
#include "bytecode.hpp"
#include "definitions.hpp"
#include "lexer.hpp"
#include "pattern_parser.hpp"
#include "exceptions.hpp"
#include "code_generation_utils.hpp"

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(),
        [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

Pattern_parser::Pattern_parser(Lexer &lex) : lexer(lex)
{

}

void Pattern_parser::except_paran_open()
{
    Token token = lexer.get_token();
    if(token.type != Token_type::Open_paran)
        throw ParanthesisExpected();
}

void Pattern_parser::except_paran_close()
{
    Token token = lexer.get_token();
    if(token.type != Token_type::Close_paran)
        throw ParanthesisExpected();
}

Pattern Pattern_parser::match_string()
{
    Token first_tok = lexer.get_token();
    Token second_tok = lexer.get_token();

    Pattern result = code_generation_utils.match_string_pattern(first_tok.value);

    if(second_tok.type == Token_type::Close_paran) {
        return result;
    }

    Token current = second_tok;

    while(current.type == Token_type::Or_operator) {
        second_tok = lexer.get_token();

        if(second_tok.type != Token_type::String) {
            throw std::runtime_error("Only | seperated string are accpeted in the match");
        }

        Pattern string = code_generation_utils.match_string_pattern(second_tok.value);
        result = code_generation_utils.execute_or_operator(result, string);

        current = lexer.get_token();
    }

    if(current.type != Token_type::Close_paran)
        throw UnbalancedParanthesis();

    return result;
}

Pattern Pattern_parser::save_string()
{
    std::string name = lexer.get_name();
    except_paran_open();
    auto pattern = match_string();

    if(not name.empty())
        return code_generation_utils.execute_save_variable(pattern, name);
    else return pattern;
}

Pattern Pattern_parser::save_binary()
{
    std::string name = lexer.get_name();
    except_paran_open();
    std::string value = lexer.get_binary();
    except_paran_close();

    if(value.empty())
        throw std::runtime_error("value of bit match cannot be empty");

    auto result = code_generation_utils.match_binary_pattern(value);

    if(not name.empty())
        return code_generation_utils.execute_save_variable(result, name);
    else return result;
}


Pattern Pattern_parser::parse()
{
    return iteration();
}

Pattern Pattern_parser::iteration(bool itermatch)
{
    Pattern result;
    Pattern this_level;

    Token token;
    while(not (lexer.is_eot(token.value) or (token.type == Token_type::Iteration_stop and itermatch)))
    {
        bool currect = true;
        switch(token.type)
        {
            case Token_type::Save_match:
                this_level = save_string();
                break;

            case Token_type::Save_binary:
                this_level = save_binary();
                break;

            case Token_type::Iteration_start: {
                this_level = parse();
                std::vector<std::string> names;
                for(const Bytecode &bytecode : this_level) {
                    if(bytecode.opcode == Opcode::Save_start) {
                        names.insert(names.end(), bytecode.args.begin(), bytecode.args.end());
                    }
                }
                this_level = names.empty() ?  code_generation_utils.execute_kleene_star(this_level) : code_generation_utils.execute_iteration(this_level, names);
            } break;

            case Token_type::String:
                this_level = code_generation_utils.match_string_pattern(token.value);
                break;

            default:
                currect = false;
        }

        if(currect)
            result = code_generation_utils.execute_concatanation_operator({result, this_level});
        token = lexer.get_token();
    };

    if(itermatch and token.type != Token_type::Iteration_stop) {
        throw std::runtime_error("Iteration signs(<>) doen't match together, '>' expected!");
    }

    return result;
}
