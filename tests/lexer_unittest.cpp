#include "lexer.hpp"

#include "gtest/gtest.h"

#include <string>

namespace {
    TEST(Lexer, get_int)
    {
        {
            const char *text = "123";
            Lexer lexer(text);
            std::string int_ = lexer.get_int();
            EXPECT_STREQ(text,int_.c_str());
        }
        {
            const char *text = "123A";
            Lexer lexer(text);
            std::string first_int = lexer.get_int();
            std::string empty = lexer.get_int();
            EXPECT_STREQ("", empty.c_str());
        }
    }

    TEST(Lexer, get_name)
    {
        {
            const char *text = "a_c_valid_name";
            Lexer lexer(text);
            std::string name = lexer.get_name();
            EXPECT_STREQ(text, name.c_str());
        }
        {
            const char *text = "th1s_name_have_123";
            Lexer lexer(text);
            std::string name = lexer.get_name();
            EXPECT_STREQ(text, name.c_str());
        }
        {
            const char *text = "123Have_num_at_begin";
            Lexer lexer(text);
            std::string name = lexer.get_name();
            EXPECT_STREQ("", name.c_str());
        }
    }

    TEST(Lexer, get_str)
    {
        {
            const char *text = "A string with [] and even 1 and";
            Lexer lexer(text);
            std::string name = lexer.get_str();
            EXPECT_STREQ(text, name.c_str());
        }
        {
            const char *text = "a string with [] $and even 1 and";
            const char *expected = "a string with [] ";
            Lexer lexer(text);
            std::string name = lexer.get_str();
            EXPECT_STREQ(expected, name.c_str());
        }
    }

    TEST(Lexer, get_token)
    {
        const char *text = "A STRING $NAME(THIS)|@THAT";
        Lexer lexer(text);
        std::vector<Token> expected = {
            {Token_type::String, "A STRING "},
            {Token_type::Save_match, "$"},
            {Token_type::String, "NAME"},
            {Token_type::Open_paran, "("},
            {Token_type::String, "THIS"},
            {Token_type::Close_paran, ")"},
            {Token_type::Or_operator, "|"},
            {Token_type::Save_binary, "@"},
            {Token_type::String, "THAT"},
            {Token_type::String, "EOT"}
        };

        for(auto token : expected) {
            Token tok = lexer.get_token();
            EXPECT_EQ(tok, token);
        }
    }

    TEST(Lexer, is_eot)
    {
        const char *text = "123ABC_D";
        Lexer lexer(text);

        EXPECT_FALSE(lexer.is_eot(lexer.get_int()));
        EXPECT_FALSE(lexer.is_eot(lexer.get_name()));
        EXPECT_TRUE(lexer.is_eot(lexer.get_int()));
        EXPECT_TRUE(lexer.is_eot(lexer.get_binary()));
        EXPECT_TRUE(lexer.is_eot(lexer.get_name()));
        EXPECT_TRUE(lexer.is_eot(lexer.get_token().value));
        EXPECT_FALSE(lexer.is_eot(std::string()));
    }
};
