#include "virtual_machine.hpp"
#include "bytecode.hpp"

#include "unittesting_utils.hpp"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <vector>
#include <bitset>

namespace
{
    using namespace testing;

    TEST(VirtualMachine, Jump)
    {
        Pattern code = {
            {Opcode::Jump, {"3"}},
            {Opcode::Match, {"SOMETHING"}},
            {Opcode::Jump, {"2"}},
            {Opcode::Jump, {"-1"}},
        };

        VM vm(code);

        auto output = vm.run("", 0);

        EXPECT_THAT(output, IsEmpty());
    }

    TEST(VirtualMachine, Capturing)
    {
        Pattern code = {
            {Opcode::Save_start, {"variable"}},
            {Opcode::Match, {"ANY", "3"}},
            {Opcode::Save_end, {"variable"}},
        };

        VM vm(code);

        auto output = vm.run("ABCD", 4);

        EXPECT_EQ(output.size(), 1);
        EXPECT_THAT(output["variable"], ElementsAre('A', 'B', 'C'));
    }

    TEST(VirtualMachine, Split)
    {
        Pattern code = {
            // Jump to main code
            {Opcode::Jump, {"2"}},

            // Loop section
            {Opcode::Jump, {"0"}},

            // main
            {Opcode::Split, {"-1", "1"}},
            {Opcode::Split, {"1", "-2"}},
        };

        VM vm(code);
        auto output = vm.run("", 0);

        EXPECT_EQ(output.size(), 0);
    }

    TEST(VirtualMachine, Return)
    {
        Pattern code = {
            {Opcode::Halt, {}},
            {Opcode::Return, {}},
            {Opcode::Jump, {"-2"}},
        };

        VM vm(code);
        auto output = vm.run("", 0);

        EXPECT_EQ(output.size(), 0);
    }


    TEST(VirtualMachine, Iteration)
    {
        Pattern code = {
                {Opcode::Save_start, {"name"}},
                {Opcode::Match, {"ANY", "2"}},
                {Opcode::Save_end, {"name"}},
                {Opcode::Add_iterate, {"name"}},
                {Opcode::Split, {"-4", "1"}},
                {Opcode::Match, {"."}}
        };

        VM vm(code);
        auto output = vm.run("ABACAD.", 7);

        EXPECT_EQ(output.size(), 4);
        EXPECT_THAT(output["name0"], ElementsAre('A', 'B'));
        EXPECT_THAT(output["name1"], ElementsAre('A', 'C'));
        EXPECT_THAT(output["name2"], ElementsAre('A', 'D'));
    }

    TEST(ViraulMachine, Binary_matcher)
    {
        auto binary_to_char = [](std::string binary) {
            return static_cast<char>(std::bitset<8>(binary).to_ulong());
        };

        Pattern code = {
            {Opcode::Save_start, {"upper"}},
            {Opcode::Bit_match, {"ANY", "4"}},
            {Opcode::Save_end, {"upper"}},
            {Opcode::Save_start, {"lower"}},
            {Opcode::Bit_match, {"ANY", "4"}},
            {Opcode::Save_end, {"lower"}},
        };

        VM vm(code);
        auto output = vm.run(std::string(1, binary_to_char("10101001")).c_str(), 1);

        EXPECT_THAT(output["upper"], ElementsAre(binary_to_char("10100000")));
        EXPECT_THAT(output["lower"], ElementsAre(binary_to_char("00001001")));
    }
}
