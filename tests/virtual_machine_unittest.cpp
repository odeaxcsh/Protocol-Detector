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
            new Bytecode{Opcode::Jump, {"3"}},
            new Bytecode{Opcode::Match, {"SOMETHING"}},
            new Bytecode{Opcode::Jump, {"2"}},
            new Bytecode{Opcode::Jump, {"-1"}},
        };

        VM vm(code);

        auto output = vm.run("", 0);

        EXPECT_THAT(output, IsEmpty());
    }

    TEST(VirtualMachine, Capturing)
    {
        Pattern code = {
            new Bytecode{Opcode::Save_start, {"variable"}},
            new Bytecode{Opcode::Match, {"ANY", "3"}},
            new Bytecode{Opcode::Save_end, {"variable"}},
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
            new Bytecode{Opcode::Jump, {"2"}},

            // Loop section
            new Bytecode{Opcode::Jump, {"0"}},

            // main
            new Bytecode{Opcode::Split, {"1", "-1"}},
            new Bytecode{Opcode::Split, {"1", "-2"}},
        };

        VM vm(code);
        auto output = vm.run("", 0);

        EXPECT_EQ(output.size(), 0);
    }

    TEST(VirtualMachine, Return)
    {
        Pattern code = {
            new Bytecode{Opcode::Halt, {}},
            new Bytecode{Opcode::Return, {}},
            new Bytecode{Opcode::Jump, {"-2"}},
        };

        VM vm(code);
        auto output = vm.run("", 0);

        EXPECT_EQ(output.size(), 0);
    }


    TEST(VirtualMachine, Iteration)
    {
        Pattern code = {
                new Bytecode{Opcode::Save_start, {"name"}},
                new Bytecode{Opcode::Match, {"ANY", "2"}},
                new Bytecode{Opcode::Save_end, {"name"}},
                new Bytecode_add_iterate{Opcode::Add_iterate, {"name"}, ""},
                new Bytecode{Opcode::Split, {"-4", "1"}},
                new Bytecode{Opcode::Match, {"."}}
        };

        VM vm(code);
        auto output = vm.run("ABACAD.", 7);

        EXPECT_EQ(output.size(), 3);
        EXPECT_THAT(output["name[0]"], ElementsAre('A', 'B'));
        EXPECT_THAT(output["name[1]"], ElementsAre('A', 'C'));
        EXPECT_THAT(output["name[2]"], ElementsAre('A', 'D'));
    }

    TEST(VirtualMachine, LimitedIteration)
    {
        Pattern code = {
                new Bytecode{Opcode::Save_start, {"name"}},
                new Bytecode{Opcode::Match, {"ANY", "2"}},
                new Bytecode{Opcode::Save_end, {"name"}},
                new Bytecode_add_iterate{Opcode::Add_iterate, {"name"}, "2"},
                new Bytecode{Opcode::Split, {"-4", "1"}},
        };

        VM vm(code);
        auto output = vm.run("ABACAD.", 7);

        EXPECT_EQ(output.size(), 2);
        EXPECT_THAT(output["name[0]"], ElementsAre('A', 'B'));
        EXPECT_THAT(output["name[1]"], ElementsAre('A', 'C'));
    }

    TEST(ViraulMachine, Binary_matcher)
    {
        auto binary_to_char = [](std::string binary) {
            return static_cast<char>(std::bitset<8>(binary).to_ulong());
        };

        Pattern code = {
            new Bytecode{Opcode::Save_start, {"upper"}},
            new Bytecode{Opcode::Bit_match, {"ANY", "4"}},
            new Bytecode{Opcode::Save_end, {"upper"}},
            new Bytecode{Opcode::Save_start, {"lower"}},
            new Bytecode{Opcode::Bit_match, {"ANY", "4"}},
            new Bytecode{Opcode::Save_end, {"lower"}},
        };

        VM vm(code);
        auto output = vm.run(std::string(1, binary_to_char("10101001")).c_str(), 1);

        EXPECT_THAT(output["upper"], ElementsAre(binary_to_char("10100000")));
        EXPECT_THAT(output["lower"], ElementsAre(binary_to_char("00001001")));
    }
}
