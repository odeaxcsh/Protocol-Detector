#include <string>
#include <vector>

#include "code_generation_utils.hpp"
#include "definitions.hpp"

struct Code_generation_utils code_generation_utils;

Bytecode *Code_generation_utils::match_any_char()
{
    using std::to_string;
    return new Bytecode {
        Opcode::Match,
        {ANY_STRING, to_string(1)}
    };
}

Pattern Code_generation_utils::match_binary_pattern(const std::string &str)
{
    if(str[0] == 'b') {
        return {
            new Bytecode {
                Opcode::Bit_match,
                {str}
            }
        };
    } else if(is_number(str)) {
        return {
                new Bytecode {
                Opcode::Bit_match,
                {ANY_STRING, str}
            }
        };
    } else {
        throw std::runtime_error("an error");
    }
}

Pattern Code_generation_utils::match_string_pattern(const std::string &str)
{
    if(str == std::string("variable")) {
        return any_string_pattern();
    } else if(is_number(str)) {
        return {
            new Bytecode {
                Opcode::Match,
                {ANY_STRING, str}
            }
        };
    } else {
        return {
                new Bytecode {
                Opcode::Match,
                {str}
            }
        };
    }
}

Pattern Code_generation_utils::execute_kleene_star(const Pattern &matches_pattern)
{
    using std::end, std::begin;
    using std::to_string;

    Pattern result;
    int matches_pattern_length = matches_pattern.size();

    result.push_back(new Bytecode {
        Opcode::Split,
        {to_string(matches_pattern_length + 2), to_string(1)}
    });

    result.insert(end(result), begin(matches_pattern), end(matches_pattern));

    result.push_back(new Bytecode {
        Opcode::Jump,
        {to_string( -(matches_pattern_length + 1))}
    });
    return result;
}

Pattern Code_generation_utils::execute_iteration(const Pattern &matches_pattern, const std::vector<std::string> &names, const std::string &limit_expr)
{
    using std::end, std::begin;
    using std::to_string;

    Pattern result;
    int matches_pattern_length = matches_pattern.size();

    result.insert(end(result), begin(matches_pattern), end(matches_pattern));

    result.push_back(new Bytecode_add_iterate{
        Opcode::Add_iterate,
        names,
        limit_expr,
    });

    if(limit_expr != "") {
        result.push_back(new Bytecode{
            Opcode::Jump,
            {to_string(-matches_pattern_length - 1)}
        });
    } else {
        result.push_back(new Bytecode {
            Opcode::Split,
            {to_string(-matches_pattern_length - 1), to_string(1)}
        });
    }

    return result;
}

Pattern Code_generation_utils::execute_concatanation_operator(const std::vector<Pattern> &patterns)
{
    using std::end, std::begin;
    Pattern result;

    for(const Pattern &pattern : patterns)
        result.insert(end(result), begin(pattern), end(pattern));

    return result;
}

Pattern Code_generation_utils::any_string_pattern()
{
    using std::to_string;
    Pattern result;

    result.push_back(new Bytecode {
        Opcode::Split,
        {to_string(3), to_string(1)}
    });

    result.push_back(match_any_char());

    result.push_back(new Bytecode {
        Opcode::Jump,
        {to_string(-2)}
    });

    return result;
}

Pattern Code_generation_utils::execute_or_operator(const Pattern &first, const Pattern &second)
{
    using std::end, std::begin;
    using std::to_string;

    Pattern result;
    int first_length = first.size(), second_length = second.size();

    result.push_back(new Bytecode {
        Opcode::Split,
        {to_string(1), to_string(first_length + 2)}
    });

    result.insert(end(result), begin(first), end(first));

    result.push_back(new Bytecode {
        Opcode::Jump,
        {to_string(second_length + 1)}
    });

    result.insert(end(result), begin(second), end(second));

    return result;
}

Pattern Code_generation_utils::execute_save_variable(const Pattern &match_pattern, const std::string &name)
{
    using std::end, std::begin;
    Pattern result;

    result.push_back(new Bytecode {
        Opcode::Save_start,
        {name}
    });

    result.insert(end(result), begin(match_pattern), end(match_pattern));

    result.push_back(new Bytecode {
        Opcode::Save_end,
        {name}
    });

    return result;
}
