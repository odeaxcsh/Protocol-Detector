#if !defined(VIRTUAL_MACHINE)
#define VIRTUAL_MACHINE

#include "bytecode.hpp"

#include <map>
#include <string>
#include <vector>
#include <stack>
#include <utility>

typedef struct {
    int bit = 0,
        byte = 0;

    void inc_bit()
    {
        bit = (bit + 1) % 8;
        if(bit == 0)
            ++byte;
    }

    void inc_byte()
    {
        ++byte;
    }

} Index;

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

    typedef struct {
        bool match = false;

        Index index = {0, 0};

        int pc = 0;

        std::map<std::string, Index> var_starts;
        std::map<std::string, Index> var_ends;

        std::map<std::string, int> iteration_counter;
    } machine_state;

    bool match(machine_state &state);
    bool bit_match(machine_state &state);
    void jump(machine_state &state);
    void split(machine_state &state);
    void save_start(machine_state &state);
    void save_end(machine_state &state);
    void add_iterate(machine_state &state);

    std::vector<unsigned char> text;
    int length = 0;

    std::stack<machine_state> queue;
    const static std::string counter_name;
};

#endif
