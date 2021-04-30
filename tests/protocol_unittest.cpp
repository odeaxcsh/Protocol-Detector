#include "protocol.hpp"
#include "lexer.hpp"
#include "pattern_parser.hpp"
#include "bytecode.hpp"
#include "unittesting_utils.hpp"

#include "gtest/gtest.h"

#include <json/json.h>
#include <vector>

namespace
{
    Json::Value convert_to_json(const std::string &str)
    {
        Json::Value json;
        Json::Reader reader;
        reader.parse(str, json);
        return json;
    };

    TEST(Protocol, Basic_json)
    {
        const std::string rawJson = R"(
          {
            "name": "sample",
            "layer": 3,
            "pattern": "THIS IS A TEST",
            "pattern-ignores": ""
            "requirements": []
          }
        )";

        Protocol protocol(convert_to_json(rawJson));
        Pattern expected = {
            {Opcode::Match, {"THIS IS A TEST"}}
        };
        std::vector<std::vector<std::string>> expected_req = {{}, {}, {}};

        EXPECT_EQ("sample", protocol.get_name());
        EXPECT_EQ(3, protocol.get_layer());
        EXPECT_EQ(expected, protocol.get_pattern());
        EXPECT_EQ(expected_req, protocol.get_requirements());
    }
};
