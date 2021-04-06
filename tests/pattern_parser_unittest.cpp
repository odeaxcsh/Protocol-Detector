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
        const char *text = "$(variable)."
        Lexer lexer(text);
        Pattern result = Pattern_parser(lexer).parse();

        Pattern expected = {
            {Opcode::Split, {"3", "1"}},
            {Opcode::Match, {ANY_STRING, "1"}},
            {Opcode::Jump, {"2"}},
            {Opcode::Match, {"."}}
        };

        EXPECT_EQ(expected, result);
    }
}
