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
            new Bytecode{Opcode::Split, {"1", "3"}},
            new Bytecode{Opcode::Match, {"THIS"}},
            new Bytecode{Opcode::Jump, {"2"}},
            new Bytecode{Opcode::Match, {"THAT"}},
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test2)
    {
        const char *text = "$(variable).";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            new Bytecode{Opcode::Split, {"3", "1"}},
            new Bytecode{Opcode::Match, {ANY_STRING, "1"}},
            new Bytecode{Opcode::Jump, {"-2"}},
            new Bytecode{Opcode::Match, {"."}}
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test3)
    {
        const char *text = "$name(variable)";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            new Bytecode{Opcode::Save_start, {"name"}},
            new Bytecode{Opcode::Split, {"3", "1"}},
            new Bytecode{Opcode::Match, {ANY_STRING, "1"}},
            new Bytecode{Opcode::Jump, {"-2"}},
            new Bytecode{Opcode::Save_end, {"name"}}
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test4)
    {
        const char *text = "<$Protocol(PUT|GET) on $version(1.1|1.2)>";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            new Bytecode{Opcode::Save_start, {"Protocol"}},
            new Bytecode{Opcode::Split, {"1", "3"}},
            new Bytecode{Opcode::Match, {"PUT"}},
            new Bytecode{Opcode::Jump, {"2"}},
            new Bytecode{Opcode::Match, {"GET"}},
            new Bytecode{Opcode::Save_end, {"Protocol"}},
            new Bytecode{Opcode::Match, {" on "}},
            new Bytecode{Opcode::Save_start, {"version"}},
            new Bytecode{Opcode::Split, {"1", "3"}},
            new Bytecode{Opcode::Match, {"1.1"}},
            new Bytecode{Opcode::Jump, {"2"}},
            new Bytecode{Opcode::Match, {"1.2"}},
            new Bytecode{Opcode::Save_end, {"version"}},
            new Bytecode{Opcode::Add_iterate, {"Protocol", "version"}},
            new Bytecode{Opcode::Split, {"-14", "1"}}
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test5)
    {
        const char *text = "<$(A|B)> !";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            new Bytecode{Opcode::Split, {"6", "1"}},
            new Bytecode{Opcode::Split, {"1", "3"}},
            new Bytecode{Opcode::Match, {"A"}},
            new Bytecode{Opcode::Jump, {"2"}},
            new Bytecode{Opcode::Match, {"B"}},
            new Bytecode{Opcode::Jump, {"-5"}},
            new Bytecode{Opcode::Match, {" !"}}
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test6)
    {
        const char *text = "<@(b10)@(32)>";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            new Bytecode{Opcode::Split, {"4", "1"}},
            new Bytecode{Opcode::Bit_match, {"b10"}},
            new Bytecode{Opcode::Bit_match, {"ANY", "32"}},
            new Bytecode{Opcode::Jump, {"-3"}},
        };

        EXPECT_EQ(expected, result);
    }

    TEST(Parser, Test7)
    {
        const char *text = "<@binary1(b10)@binary2(32)>";
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            new Bytecode{Opcode::Save_start, {"binary1"}},
            new Bytecode{Opcode::Bit_match, {"b10"}},
            new Bytecode{Opcode::Save_end, {"binary1"}},
            new Bytecode{Opcode::Save_start, {"binary2"}},
            new Bytecode{Opcode::Bit_match, {"ANY", "32"}},
            new Bytecode{Opcode::Save_end, {"binary2"}},
            new Bytecode{Opcode::Add_iterate, {"binary1", "binary2"}},
            new Bytecode{Opcode::Split, {"-7", "1"}}
        };

        EXPECT_EQ(expected, result);
    }
}
