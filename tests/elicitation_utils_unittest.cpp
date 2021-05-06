#include "elicitation_utils.hpp"

#include "gtest/gtest.h"

#include <string>

typedef std::map<std::string, std::vector<unsigned char>> var_t;

namespace {
    TEST(Elicitation, Basic_int)
    {
        std::string expr = "(THIS)";
        var_t var = {{"THIS", {12}}};
        
        auto parser = Formula_parser();
        auto compiled = parser.parse<int>(expr);
        auto value = compiled->eval(var);
        
        EXPECT_EQ(value, 12);
    }

    TEST(Elicitation, Sum_int)
    {
        std::string expr = "THIS + THAT";
        var_t var = {{"THIS", {12}}, {"THAT", {15}}};
        
        auto parser = Formula_parser();
        auto compiled = parser.parse<int>(expr);
        auto value = compiled->eval(var);
        
        EXPECT_EQ(value, 27);
    }

    TEST(Elicitation, Mul_int)
    {
        std::string expr = "THIS * THAT";
        var_t var = {{"THIS", {12}}, {"THAT", {15}}};
        
        auto parser = Formula_parser();
        auto compiled = parser.parse<int>(expr);
        auto value = compiled->eval(var);
        
        EXPECT_EQ(value, 180);
    }

    TEST(Elicitation, Div_int)
    {
        std::string expr = "THIS / THAT";
        var_t var = {{"THIS", {180}}, {"THAT", {9}}};
        
        auto parser = Formula_parser();
        auto compiled = parser.parse<int>(expr);
        auto value = compiled->eval(var);
        
        EXPECT_EQ(value, 20);
    }

    TEST(Elicitation, equality)
    {
        std::string expr = "(a = b)";
        
        auto parser = Formula_parser();
        auto compiled = parser.parse<int>(expr);
        var_t var1 = {{"a", {10}}, {"b", {5}}};
        var_t var2 = {{"a", {5}}, {"b", {5}}};

        auto value1 = compiled->eval(var1);
        auto value2 = compiled->eval(var2);
        EXPECT_EQ(value1, 0);
        EXPECT_EQ(value2, 1);
    }

    TEST(Elicitation, less)
    {
        std::string expr = "(a < b)";
        
        auto parser = Formula_parser();
        auto compiled = parser.parse<int>(expr);
        var_t var1 = {{"a", {6}}, {"b", {5}}};
        var_t var2 = {{"a", {5}}, {"b", {6}}};

        auto value1 = compiled->eval(var1);
        auto value2 = compiled->eval(var2);
        EXPECT_EQ(value1, 0);
        EXPECT_EQ(value2, 1);
    }

    TEST(Elicitation, greater)
    {
        std::string expr = "(a > b)";
        
        auto parser = Formula_parser();
        auto compiled = parser.parse<int>(expr);
        var_t var1 = {{"a", {6}}, {"b", {5}}};
        var_t var2 = {{"a", {5}}, {"b", {6}}};

        auto value1 = compiled->eval(var1);
        auto value2 = compiled->eval(var2);
        EXPECT_EQ(value1, 1);
        EXPECT_EQ(value2, 0);
    }

    TEST(Elicitation, combined_int)
    {
        std::string expr = "(a + b) * c";
        var_t var = {{"a", {3}}, {"b", {4}}, {"c", {7}}};
        
        auto parser = Formula_parser();
        auto compiled = parser.parse<int>(expr);
        auto value = compiled->eval(var);
        
        EXPECT_EQ(value, 49);
    }

    TEST(Elicitation, complex_int)
    {
        std::string expr = "(a + b) * (c + d)";
        var_t var = {{"a", {3}}, {"b", {4}}, {"c", {7}}, {"d", {1}}};
        
        auto parser = Formula_parser();
        auto compiled = parser.parse<int>(expr);
        auto value = compiled->eval(var);
        
        EXPECT_EQ(value, 56);
    }

    TEST(Elicitation, complex_bool)
    {
        std::string expr = "(a + b) * (c + d) = a*c + a*d + b*c + b*d";
        var_t var = {{"a", {8}}, {"b", {1}}, {"c", {10}}, {"d", {12}}};
        
        auto parser = Formula_parser();
        auto compiled = parser.parse<int>(expr);
        auto value = compiled->eval(var);
        
        EXPECT_EQ(value, 1);
    }
};
