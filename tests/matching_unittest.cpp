#include "virtual_machine.hpp"
#include "pattern_parser.hpp"
#include "unittesting_utils.hpp"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string.h>
#include <vector>


namespace
{
    using namespace testing;

    TEST(Matching, simple)
    {
        const char *pattern = "$first(this)";
        std::vector<unsigned char> input = {'t', 'h', 'i', 's', 'a', 'n', 'd', '.', '.'};
        Lexer lexer(pattern);
        Pattern code = Pattern_parser(lexer).parse();
        VM vm(code);

        auto result = vm.run(input, 9);

        EXPECT_EQ(result.size(), 1);
        EXPECT_THAT(result["first"], ElementsAre('t', 'h', 'i', 's'));
    }

    TEST(Matching, counted_char)
    {
        const char *pattern = "$first(5)";
        std::vector<unsigned char> input = {'t', 'h', 'i', 's', 'a', 'n', 'd', '.', '.'};
        Lexer lexer(pattern);
        Pattern code = Pattern_parser(lexer).parse();
        VM vm(code);

        auto result = vm.run(input, 9);

        EXPECT_EQ(result.size(), 1);
        EXPECT_THAT(result["first"], ElementsAre('t', 'h', 'i', 's', 'a'));
    }

    TEST(Matching, matching_or)
    {
        const char *pattern = "$(THIS|THAT).";
        Lexer lexer(pattern);
        Pattern code = Pattern_parser(lexer).parse();
        VM vm(code);

        std::vector<unsigned char> input1 = {'T', 'H', 'I', 'S', '.'};
        std::vector<unsigned char> input2 = {'T', 'H', 'A', 'T', '.'};
        std::vector<unsigned char> input3 = {'T', 'X', 'X', 'T', '.'};


        auto result = vm.run(input1, 5);
        EXPECT_EQ(result.size(), 0);

        result = vm.run(input2, 5);
        EXPECT_EQ(result.size(), 0);

        result = vm.run(input3, 5);
        EXPECT_EQ(result.size(), 1);    
    }

    TEST(Matching, iteration)
    {
        const char *pattern = "<$first(variable): $second(variable)\n>\n";

        Lexer lexer(pattern);
        Pattern code = Pattern_parser(lexer).parse();
        VM vm(code);

        auto input = 
            "name: Something\n"
            "Andother: ButOther\n"
            "another: another\n"
            "\n"
            "\n"_v;
        
        auto result = vm.run(input, input.size());

        EXPECT_EQ(result.size(), 6);
        EXPECT_EQ(result["first[0]"], "name"_v);
        EXPECT_EQ(result["first[1]"], "Andother"_v);
        EXPECT_EQ(result["first[2]"], "another"_v);

        EXPECT_EQ(result["second[0]"], "Something"_v);
        EXPECT_EQ(result["second[1]"], "ButOther"_v);
        EXPECT_EQ(result["second[2]"], "another"_v);
    }

    TEST(Matching, conditional_iteration)
    {
        const char *pattern = "$number(1)<(number)$first(variable): $second(variable)\n>\n";

        Lexer lexer(pattern);
        Pattern code = Pattern_parser(lexer).parse();
        VM vm(code);

        auto input = 
            "name: Something\n"
            "Andother: ButOther\n"
            "another: another\n"
            "\n"
            "\n"_v;
        input.insert(input.begin(), 3);
        
        auto result = vm.run(input, input.size());

        EXPECT_EQ(result.size(), 7);
        EXPECT_EQ(result["number"], std::vector<unsigned char>{3});

        EXPECT_EQ(result["first[0]"], "name"_v);
        EXPECT_EQ(result["first[1]"], "Andother"_v);
        EXPECT_EQ(result["first[2]"], "another"_v);

        EXPECT_EQ(result["second[0]"], "Something"_v);
        EXPECT_EQ(result["second[1]"], "ButOther"_v);
        EXPECT_EQ(result["second[2]"], "another"_v);
    }

    TEST(Matching, conditional_iteration_less)
    {
        const char *pattern = "$number(1)<(number)$first(variable)\n>";

        Lexer lexer(pattern);
        Pattern code = Pattern_parser(lexer).parse();
        VM vm(code);

        auto input = 
            "First line of string\n"
            "Another line but this line are not enough\n"
            "The as mentioned this pattern expcted to having at least 4 lines\n"
            ""_v;
        input.insert(input.begin(), 4);
        
        auto result = vm.run(input, input.size());

        EXPECT_EQ(result.size(), 1);
    }
}