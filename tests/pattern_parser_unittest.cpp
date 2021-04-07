#include "pattern_parser.hpp"
#include "lexer.hpp"

#include "unittesting_utils.hpp"
#include "gtest/gtest.h"

#include <string>

namespace {
    TEST(Parser, Test1)
    {
        const char *text = "$(THIS|THAT)";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            {Opcode::Split, {"1", "3"}},
            {Opcode::Match, {"THIS"}},
            {Opcode::Jump, {"2"}},
            {Opcode::Match, {"THAT"}},
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test2)
    {
        const char *text = "$(variable).";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            {Opcode::Split, {"3", "1"}},
            {Opcode::Match, {ANY_STRING, "1"}},
            {Opcode::Jump, {"-2"}},
            {Opcode::Match, {"."}}
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test3)
    {
        const char *text = "$name(variable)";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            {Opcode::Save_start, {"name"}},
            {Opcode::Split, {"3", "1"}},
            {Opcode::Match, {ANY_STRING, "1"}},
            {Opcode::Jump, {"-2"}},
            {Opcode::Save_end, {"name"}}
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test4)
    {
        const char *text = "<$Protocol(PUT|GET) on $version(1.1|1.2)>";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            {Opcode::Save_start, {"Protocol"}},
            {Opcode::Split, {"1", "3"}},
            {Opcode::Match, {"PUT"}},
            {Opcode::Jump, {"2"}},
            {Opcode::Match, {"GET"}},
            {Opcode::Save_end, {"Protocol"}},
            {Opcode::Match, {" on "}},
            {Opcode::Save_start, {"version"}},
            {Opcode::Split, {"1", "3"}},
            {Opcode::Match, {"1.1"}},
            {Opcode::Jump, {"2"}},
            {Opcode::Match, {"1.2"}},
            {Opcode::Save_end, {"version"}},
            {Opcode::Add_iterate, {"Protocol", "version"}},
            {Opcode::Split, {"-14", "1"}}
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test5)
    {
        const char *text = "<$(A|B)> !";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            {Opcode::Split, {"6", "1"}},
            {Opcode::Split, {"1", "3"}},
            {Opcode::Match, {"A"}},
            {Opcode::Jump, {"2"}},
            {Opcode::Match, {"B"}},
            {Opcode::Jump, {"-5"}},
            {Opcode::Match, {" !"}}
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test6)
    {
        const char *text = "<@(b10)@(32)>";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            {Opcode::Split, {"4", "1"}},
            {Opcode::Bit_match, {"b10"}},
            {Opcode::Bit_match, {"ANY", "32"}},
            {Opcode::Jump, {"-3"}},
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test7)
    {
        const char *text = "<@binary1(b10)@binary2(32)>";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            {Opcode::Save_start, {"binary1"}},
            {Opcode::Bit_match, {"b10"}},
            {Opcode::Save_end, {"binary1"}},
            {Opcode::Save_start, {"binary2"}},
            {Opcode::Bit_match, {"ANY", "32"}},
            {Opcode::Save_end, {"binary2"}},
            {Opcode::Add_iterate, {"binary1", "binary2"}},
            {Opcode::Split, {"-7", "1"}}
        };

        EXPECT_EQ(expected, result);
    }
}
