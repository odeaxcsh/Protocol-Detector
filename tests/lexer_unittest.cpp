#include "lexer.hpp"

#include "gtest/gtest.h"

#include <string>

namespace {
    TEST(Lexer_get_int, Basic)
    {
        const char *text = "123";
        Lexer lexer(text);
        std::string int_ = lexer.get_int();
        EXPECT_STREQ(text,int_.c_str());
    }

    TEST(Lexer_get_int, NotMatch)
    {
        const char *text = "123A";
        Lexer lexer(text);
        std::string first_int = lexer.get_int();
        std::string empty = lexer.get_int();
        EXPECT_STREQ("", empty.c_str());
    }

    TEST(Lexer_get_name, Basic)
    {
        const char *text = "a_c_valid_name";
        Lexer lexer(text);
        std::string name = lexer.get_name();
        EXPECT_STREQ(text, name.c_str());
    }

    TEST(Lexer_get_name, NumberIncluded)
    {
        {
            const char *text = "th1s_name_have_123";
            Lexer lexer(text);
            std::string name = lexer.get_name();
            EXPECT_STREQ(text, name.c_str());
        }
        {
            const char *text = "123Have_num_at_first";
            Lexer lexer(text);
            std::string name = lexer.get_name();
            EXPECT_STREQ("", name.c_str());
        }
    }

    TEST(Lexer_get_str, Basic)
    {
        const char *text = "An astring with [] and even 1 and";
        Lexer lexer(text);
        std::string name = lexer.get_str();
        EXPECT_STREQ(text, name.c_str());
    }

    TEST(Lexer_get_str, MidBreak)
    {
        const char *text = "An a string with [] $and even 1 and";
        const char *expected = "An a string with [] ";
        Lexer lexer(text);
        std::string name = lexer.get_str();
        EXPECT_STREQ(expected, name.c_str());
    }
};
