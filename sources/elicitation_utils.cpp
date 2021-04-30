#include "elicitation_utils.hpp"

int get_int(const std::vector<unsigned char> &bytes)
{
    int value = 0;
    for(auto byte : bytes) {
        value = value * 256 + byte;
    }
    return value;
}

std::string get_str(const std::vector<unsigned char> &bytes)
{
    std::string str;
    for(auto byte : bytes)
        str += static_cast<char>(byte);
    return str;
}