#include "code_generation_utils.hpp"
#include "definitions.hpp"

#include "gtest/gtest.h"

#include <string>

bool operator== (const Bytecode &first, const  Bytecode &second)
{
    if((int)first.opcode != (int)second.opcode)
        return false;
    if(first.args.size() != second.args.size())
        return false;
    for(int i = 0; i < second.args.size(); ++i)
        if(first.args[i] != second.args[i])
            return false;
    return true;
}

namespace {
    using std::to_string;



    bool operator== (const Pattern &first, const Pattern &second)
    {
        for(int i = 0; i < first.size(); ++i)
            if(!(first[i] == second[i]))
                return false;
        return true;
    }

    TEST(CodeGenerator, match_any_char)
    {
        Bytecode bytecode = code_generation_utils.match_any_char();
        Bytecode expected = {Opcode::Match, {ANY_STRING, to_string(1)}};
        EXPECT_EQ(bytecode, expected);
    }

    TEST(CodeGenerator, any_string_pattern)
    {
        Pattern pattern = code_generation_utils.any_string_pattern();
        Pattern expected = {
            {Opcode::Split, {to_string(3), to_string(1)}},
            {Opcode::Match, {ANY_STRING, to_string(1)}},
            {Opcode::Jump, {to_string(-2)}}
        };

        EXPECT_EQ(expected, pattern);
    }

    TEST(CodeGenerator, concatanation)
    {
        Pattern e1 = {Bytecode{Opcode::Match, {"e1"}}};
        Pattern e2 = {Bytecode{Opcode::Match, {"e2"}}};
        Pattern expected = {
            {Bytecode{Opcode::Match, {"e1"}}},
            {Bytecode{Opcode::Match, {"e2"}}}
        };

        auto e1e2  = code_generation_utils.execute_concatanation_operator({e1, e2});

        EXPECT_EQ(e1e2, expected);
    }

    TEST(CodeGenerator, match_string)
    {
        {
            Pattern expected = code_generation_utils.any_string_pattern();
            Pattern any_string = code_generation_utils.match_string_pattern("variable");

            EXPECT_EQ(expected, any_string);
        }
        {
            int n = 5;
            Pattern expected = {
                {Opcode::Match, {ANY_STRING, to_string(n)}}
            };
            Pattern match_n_char = code_generation_utils.match_string_pattern(to_string(n));

            EXPECT_EQ(match_n_char, expected);
        }
        {
            Pattern expected = {
                {Opcode::Match, {"A TEST"}}
            };

            Pattern match_e1_or_e2 = code_generation_utils.match_string_pattern("A TEST");

            EXPECT_EQ(match_e1_or_e2, expected);
        }
    }

    TEST(CodeGenerator, match_binary)
    {
        {
            int n = 5;
            Pattern expected = {{Opcode::Bit_match, {ANY_STRING, to_string(n)}}};
            Pattern match_n_bin = code_generation_utils.match_binary_pattern(to_string(n));

            EXPECT_EQ(match_n_bin, expected);
        }
        {
            Pattern match_0011 = code_generation_utils.match_binary_pattern("b0011");
            Pattern expected = {{Opcode::Bit_match, {"b0011"}}};

            EXPECT_EQ(match_0011, expected);
        }
    }

    TEST(CodeGenerator, Or)
    {
        Pattern e1 = {Bytecode{Opcode::Match, {"e1"}}};
        Pattern e2 = {Bytecode{Opcode::Match, {"e2"}}};
        Pattern expected = {
            {Opcode::Split, {to_string(1), to_string(3)}},
            {Opcode::Match, {"e1"}},
            {Opcode::Jump, {to_string(2)}},
            {Opcode::Match, {"e2"}}
        };

        Pattern e1_or_e2 = code_generation_utils.execute_or_operator(e1, e2);
        EXPECT_EQ(e1_or_e2, expected);
    }

    TEST(CodeGenerator, Multiple_or)
    {
        Pattern e1 = {Bytecode{Opcode::Match, {"e1"}}};
        Pattern e2 = {Bytecode{Opcode::Match, {"e2"}}};
        Pattern e3 = {Bytecode{Opcode::Match, {"e3"}}};
        Pattern expected = {
            {Opcode::Split, {to_string(1), to_string(6)}},
            {Opcode::Split, {to_string(1), to_string(3)}},
            {Opcode::Match, {"e1"}},
            {Opcode::Jump, {to_string(2)}},
            {Opcode::Match, {"e2"}},
            {Opcode::Jump, {to_string(2)}},
            {Opcode::Match, {"e3"}}
        };

        Pattern e1_or_e2 = code_generation_utils.execute_or_operator(e1, e2);
        Pattern e1_or_e2_or_e3 = code_generation_utils.execute_or_operator(e1_or_e2, e3);

        EXPECT_EQ(e1_or_e2_or_e3, expected);
    }

    TEST(CodeGenerator, Save_vatiable)
    {
        std::string str = "TO MATCH",
                    name = "a_string";
        Pattern to_match = {Bytecode{Opcode::Match, {str}}};
        Pattern expected = {
            {Opcode::Save_start, {name}},
            {Opcode::Match, {str}},
            {Opcode::Save_end, {name}}
        };

        Pattern saved = code_generation_utils.execute_save_variable(to_match, name);
        EXPECT_EQ(expected, saved);
    }

    TEST(CodeGenerator, kleene_star)
    {
        Pattern e1 = {Bytecode{Opcode::Match, {"e1"}}};
        Pattern expected = {
            {Opcode::Split, {to_string(3), to_string(1)}},
            {Opcode::Match, {"e1"}},
            {Opcode::Jump, {to_string(-2)}},
        };

        Pattern e1_star = code_generation_utils.execute_kleene_star(e1);
        EXPECT_EQ(expected, e1_star);
    }

    TEST(CodeGenerator, iteration)
    {
        std::string str = "TO MATCH",
                    name = "a_string";

        Pattern saved_string = {
            {Opcode::Save_start, {name}},
            {Opcode::Match, {str}},
            {Opcode::Save_end, {name}}
        };

        Pattern expected = {
            {Opcode::Save_start, {name}},
            {Opcode::Match, {str}},
            {Opcode::Save_end, {name}},
            {Opcode::Add_iterate, {name}},
            {Opcode::Split, {to_string(-4), to_string(1)}}
        };

        Pattern repeat_saved_string = code_generation_utils.execute_iteration(saved_string, {name});
        EXPECT_EQ(expected, repeat_saved_string);
    }
};
