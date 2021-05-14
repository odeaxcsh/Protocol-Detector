#if !defined(VIRTUAL_MACHINE)
#define VIRTUAL_MACHINE

#include "bytecode.hpp"

#include <map>
#include <string>
#include <vector>
#include <stack>
#include <utility>

class VM
{
public:
    VM() = default;
    VM(const Pattern &);

    VM &load_code(const Pattern &);
    std::map<std::string, std::vector<unsigned char>> run(const char *, int end,Index start={0, 0});
    std::map<std::string, std::vector<unsigned char>> run(std::vector<unsigned char> &, int end,Index start={0, 0});

    Index get_last_index() const { return last_match; }

private:
    Pattern code;
    Index last_match;
    int end;

    bool match(machine_state &state);
    bool bit_match(machine_state &state);
    void jump(machine_state &state);
    void split(machine_state &state);
    void save_start(machine_state &state);
    void save_end(machine_state &state);

    std::vector<unsigned char> text;
    int length = 0;

    std::stack<machine_state> queue;
    const static std::string counter_name;
};

#endif
